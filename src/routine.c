#include "../includes/philos.h"


// void		update_last_meal_to_now(t_philo *philo)
// {
// 	long	current_time;
// 	long	program_start;
// 	long	new_end_meal;
	
// 	current_time = get_time_ms();
// 	program_start = get_long(&philo->program_ptr->program_mutex, &philo->program_ptr->start_time);
// 	// calc current time since start of program
// 	new_end_meal = current_time;
// 	//update in philo struct

// }

void	think(t_philo *philo)//TODO 
{
	log_status(THINKING, philo);

	//check if even necessary
	if (philo->program_ptr->num_philos % 2 == 0) //even num: system already fair
		return ;
	else
		usleep(1000); //add smallll delay to give other philo chance to grab fork
	return ;
	}

/*
	1 grab forks

	2 log status
	update last meal time
	countn meals
	evtl bool full?

	3 release forks
*/
void	eat(t_philo *philo)
{
	int	time_to_eat; 

	time_to_eat = get_int(&philo->program_ptr->program_mutex, &philo->program_ptr->time_eat);
	//1
	lock_forks(philo);
	//2
	//update last meal time
	set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal);//thread safe
	
	philo->times_eaten++;
	log_status(EATING, philo);
	// precise_usleep(philo->program_ptr->time_eat, philo->program_ptr);
	// usleep(time_to_eat);
	my_sleep(time_to_eat);

	//check if philo full
	if (philo->program_ptr->times_to_eat > 0 && philo->times_eaten == philo->program_ptr->times_to_eat)
		set_bool(&philo->philo_mutex, true, &philo->full);
	//
	unlock_forks(philo);
}


void	*routine(void *data)
{
	t_philo *philo;
	long	time;
	int	time_sleep;

	philo = (t_philo *)data;

	time_sleep = get_int(&philo->program_ptr->program_mutex, &philo->program_ptr->time_sleep);

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

	//if uneven num philos desync tiny bit (bc otehrwise not fair, depending on OS)
	// desync_philos(philo);

	//start routine
	while (!sim_finished(philo->program_ptr))
	{
		// printf("in routine loop\n");
		//check if eaten enough?
		if (philo->full)//TODO thread safe
			break ;
		//Eat
		eat(philo);
		
		//sleep
		log_status(SLEEPING, philo);
		// precise_usleep(philo->program_ptr->time_sleep, philo->program_ptr);//maybe change time into philo (but constants no race conditions!)
		// usleep(time_sleep);
		my_sleep(time_sleep);

		//think
		think(philo);

	}
	return (NULL);
}

