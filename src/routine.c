/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/03 18:14:28 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

void	think(t_philo *philo)
{
	if (log_status(THINKING, philo) != 0)
	{
		set_bool(&philo->program_ptr->program_mutex, true, &philo->program_ptr->end_sim);
		return ;
	}
	// if (philo->program_ptr->num_philos % 2 == 0)
	// 	return ;
	// else
	// {
	// 	// if (philo->num % 2 != 0)
	// 	usleep(100);
	// }
	return ;
}

int	eat(t_philo *philo)
{
	int	time_to_eat; 

	time_to_eat = get_int(&philo->program_ptr->program_mutex, &philo->program_ptr->time_eat);
	if (time_to_eat == -9999)
		return (1);

	// printf("time to eat: %i\n", time_to_eat);
	//1
	if (lock_forks(philo) != 0)
		return (1);
	//2
	//update last meal time
	if (set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal) != 0)//thread safe
		return (1);
	philo->times_eaten++;
	if (log_status(EATING, philo) != 0)
		return (1);

	precise_usleep(time_to_eat * 1000); //convert milli to micros.

	//update last meal time
	if (set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal) != 0)//thread safe
		return (1);
	//check if philo full
	if (philo->program_ptr->times_to_eat > 0 && philo->times_eaten == philo->program_ptr->times_to_eat)
	{
		if (set_bool(&philo->philo_mutex, true, &philo->full) != 0)
			return (1);
	}
	//
	if (unlock_forks(philo) != 0)
		return (1);
	return (0);
}

void	*routine(void *data)
{
	t_philo *philo;
	// long	time;
	int	time_sleep;

	philo = (t_philo *)data;

	time_sleep = get_int(&philo->program_ptr->program_mutex, &philo->program_ptr->time_sleep);
	if (time_sleep == -9999)
		return (NULL);
// wait until all philos ready
	printf("%i waiting for start\n", philo->num);
	// time = get_time_ms();
	// while (time <= philo->philo_start)
	// {
	// 	precise_usleep(50);
	// 	time = get_time_ms();
	// }

		//sync w monitor ->increase a program counter
	if (add_program_counter(&philo->program_ptr->program_mutex, &philo->program_ptr->running_philos) != 0)
		return (NULL);
	//spinlock until all ready
	while (!all_threads_running(philo->program_ptr))//spinlock OK
		;
		
	// printf("%i starting\n", philo->num);
	//sync w monitor ->increase a program counter
	// if (add_program_counter(&philo->program_ptr->program_mutex, &philo->program_ptr->running_philos) != 0)
	// 	return (NULL);

	//set first last meal time
	if (set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal) != 0)
		return (NULL);

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
		if (eat(philo) != 0)
			break ;
		
		//sleep
		if (!sim_finished(philo->program_ptr))
		{
			if (log_status(SLEEPING, philo) != 0)
				break ;
			precise_usleep(time_sleep * 1000);
		}
		//think
		if (!sim_finished(philo->program_ptr))
			think(philo);

	}
	return (NULL);
}

