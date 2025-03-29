//HEADER

#include "../includes/philos.h"

//fake to take forks, sleep until monitor kills him
void *lonely(void *data)
{
	t_philo *philo;
	long	time;

	philo = (t_philo *)data;

	// wait until all philos ready
	printf("%i waiting for start\n", philo->num);
	time = get_time_ms();
	while (time <= philo->philo_start)
	{
		usleep(100);
		time = get_time_ms();
	}

	printf("%i starting\n", philo->num);
	//sync w monitor ->increase a program counter
	add_program_counter(&philo->program_ptr->program_mutex, &philo->program_ptr->running_philos);

	//set last meal time
	set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal);//thread safe

	log_status(TAKE_FORK_1, philo); //fake to take fork

	while (!sim_finished(philo->program_ptr)) //wait until killed
		usleep(100);


	return (NULL);
}