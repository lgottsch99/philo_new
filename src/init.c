//HEADER

#include "../includes/philos.h"

static t_philo	*malloc_philo(int i, t_program *program)
{
	// printf("in malloc philo\n");
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo) * 1);
	if (!philo)
		return (NULL);
	// thread     started after init
	philo->num = i + 1; //todo: check if ok
	philo->times_eaten = 0;
	philo->full = false;
	philo->philo_start = program->start_time + 2000; //enough time?
	philo->mutex_own_fork = &program->fork_mutexes[i];
	if (i + 1 < program->num_philos)
		philo->mutex_fork_right = &program->fork_mutexes[i + 1];
	else
		philo->mutex_fork_right = &program->fork_mutexes[0];
	handle_mutex(&philo->philo_mutex, INIT);
	philo->program_ptr = program;

	return (philo);
}



static t_philo	**init_philos(t_program *program)
{
	// printf("in init structs\n");
	t_philo **array;
	int		i;

	i = 0;
	//malloc space for philos
		//malloc array of t philo * ptrs
	array = (t_philo **)malloc (sizeof(t_philo *) * program->num_philos); //mlloc n philo ptrs
	if (!array)
	{
		printf("Malloc error\n");
		return (NULL);
	}
		//malloc each philo struct
	while (i < program->num_philos)
	{
		array[i] = malloc_philo(i, program);
		if (!array[i])
		{
			//free all before
			// free_philos(array, i);
			printf("Malloc Philo error\n");
			return (NULL);
		}
		printf("created philo no %i\n", array[i]->num);
		// print_philo(array[i]);
		i++;
	}
	return (array);
}



pthread_mutex_t	*init_fork_mutex(int num_philos)
{
	pthread_mutex_t *mutexes;
	pthread_mutex_t	*tmp;
	int				i;
	//malloc space for all

	mutexes = NULL;
	mutexes = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num_philos);
	if (! mutexes)
		return (NULL);
	tmp = mutexes;
	i = 0;
	//loop and init mutex for each
	while (i < num_philos)
	{
		// if (pthread_mutex_init(&mutexes[i], NULL) == -1)
		if (handle_mutex(tmp, INIT) != 0)
		{
			// free everything
			return (NULL);
		}
		i++;
		tmp++;
	}
	tmp = NULL;
	return (mutexes);
}


int	init_program(t_program *program, char *argv[])
{
	printf("init program\n");

	program->num_philos = ft_atoi(argv[1]);
	// printf("num philos: %i\n", program->num_philos);
	program->time_die = ft_atoi(argv[2]);
	program->time_eat = ft_atoi(argv[3]);
	program->time_sleep = ft_atoi(argv[4]);
	if (argv[5])
		program->times_to_eat = ft_atoi(argv[5]);
	program->all_threads_ready = false;
	if (handle_mutex(&program->program_mutex, INIT) != 0)
		return (1);
	if (handle_mutex(&program->write_mutex, INIT) != 0)
		return (1);
	//start time  later in sim
	program->start_time = get_time_ms(); //maybe need to lock by mutex?

	program->fork_mutexes = init_fork_mutex(program->num_philos);
	if (!program->fork_mutexes)
	{
		//free and exit
		return (1);
	}

	program->philos = init_philos(program);
	if (!program->philos)
	{
		//free and exit
		return (1);
	}

	if (argv[5])
		program->times_to_eat = ft_atoi(argv[5]);
	else
		program->times_to_eat = -1;
	program->time_die = ft_atoi(argv[2]);
	program->running_philos = 0;
	return (0);
}