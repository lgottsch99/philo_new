
#include "../includes/philos.h"

void	single_philo(t_program *program)
{
	if (pthread_create(&program->philos[0]->thread, NULL, &lonely, (void *)program->philos[0]) != 0)
	{
		printf("Error creating thread\n");
		//destroy and free everything
		return;
	}
}

int	init_threads(t_program *program)
{
	printf("in init philo threads\n");
	int	i;

	i = 0;
	while (i < program->num_philos)
	{
		if (pthread_create(&program->philos[i]->thread, NULL, &routine, (void *)program->philos[i]) != 0)
		{
			printf("Error creating thread\n");
			//destroy and free everything
			return (1);
		}
		// pthread_detach(program->philos[i]->thread);
		printf("created thread %i\n", i + 1);
		i++;
	}
	return (0);
}



void	start_sim(t_program *program)
{ printf("start simulation\n");
/*
check params:

times to eat = 0 ? return
philo = 1? just run once and let him die
else
start philo threads + monitor
sync start

join all
*/
if (program->times_to_eat == 0)
	return ;
else if (program->num_philos == 1)
	single_philo(program);
	//TODO handle 1 philo (just let him die)
else
{	//create philo threads
	if (init_threads(program) != 0)
	{
		//free everything ?
		return ;
	}
}
// create monitor thread
pthread_create(&program->monitor, NULL, &monitor, program); //todo secure


//get sim start time
program->start_time = get_time_ms(); //maybe need to lock by mutex?
printf("start time : %ld\n\n", program->start_time);

for (int i = 0; i < program->num_philos; i++)
{
	print_philo(program->philos[i]);
}

//now all threads ready
// printf("setting start bool \n");
// set_bool(&program->program_mutex, true, &program->all_threads_ready); //start flag for sim to run
// set_all_ready(program);
//join all philos
for (int y = 0; y < program->num_philos; y++)
{
	pthread_join(program->philos[y]->thread, NULL);
}

//reached here? all threads finished//full
printf("ALL PHILOS FULL\n");
set_bool(&program->program_mutex, true, &program->end_sim); //need to set this so monitor also stops
pthread_join(program->monitor, NULL);



	return ;

}