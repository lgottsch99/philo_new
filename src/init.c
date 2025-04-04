/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:02:35 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 16:47:41 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

static t_philo	*malloc_philo(int i, t_program *program, char *argv[])
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo) * 1);
	if (!philo)
		return (NULL);
	philo->num = i + 1;
	philo->times_eaten = 0;
	philo->full = false;
	philo->philo_start = program->start_time + OFFSET_TIME;
	philo->mutex_own_fork = &program->fork_mutexes[i];
	if (i + 1 < program->num_philos)
		philo->mutex_fork_right = &program->fork_mutexes[i + 1];
	else
		philo->mutex_fork_right = &program->fork_mutexes[0];
	if (handle_mutex(&philo->philo_mutex, INIT) != 0) //test
	{
		free(philo);
		return (NULL);
	}
	philo->program_ptr = program;
	philo->time_die = ft_atoi(argv[2]);
	philo->time_eat = ft_atoi(argv[3]);
	philo->time_sleep = ft_atoi(argv[4]);
	return (philo);
}

static void	init_philo_error(t_philo **array, int i)
{
	while(i >= 0)
	{
		free_philo(array[i]);
		i--;
	}
	free(array);
	array = NULL;
	printf("Malloc Philo error\n");
	return ;
}


static t_philo	**init_philos(t_program *program, char *argv[])
{
	t_philo **array;
	int		i;

	i = 0;
	array = (t_philo **)malloc (sizeof(t_philo *) * program->num_philos);
	if (!array)
	{
		printf("Malloc error\n");
		return (NULL);
	}
	while (i < program->num_philos)
	{
		array[i] = malloc_philo(i, program, argv);
		if (!array[i])
		{
			init_philo_error(array, i); //test
			// while(i >= 0)
			// {
			// 	free_philo(array[i]);
			// 	i--;
			// }
			// free(array);
			// array = NULL;
			// printf("Malloc Philo error\n");
			return (NULL);
		}
		i++;
	}
	return (array);
}

pthread_mutex_t	*init_fork_mutex(int num_philos)
{
	pthread_mutex_t *mutexes;
	pthread_mutex_t	*tmp;
	int				i;

	mutexes = NULL;
	mutexes = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num_philos);
	if (!mutexes)
		return (NULL);
	tmp = mutexes;
	i = 0;
	while (i < num_philos)
	{
		if (handle_mutex(tmp, INIT) != 0)
		{
			free(mutexes);
			mutexes = NULL;
			tmp = NULL;
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
	program->num_philos = ft_atoi(argv[1]);
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
	program->start_time = get_time_ms();
	program->fork_mutexes = init_fork_mutex(program->num_philos);
	if (!program->fork_mutexes)
		return (1);
	program->philos = init_philos(program, argv);
	if (!program->philos)
		return (1);
	if (argv[5])
		program->times_to_eat = ft_atoi(argv[5]);
	else
		program->times_to_eat = -1;
	program->time_die = ft_atoi(argv[2]);
	program->running_philos = 0;
	return (0);
}
