/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:01 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 17:08:17 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

/*
	calculates the time a philo should think dynamically,
	based on how close he is to dying. /2 at end is buffer to make sure 
	he has enough time to grab forks and start eat.
*/
static void	think(t_philo *philo, int i)
{
	long	min_think;
	long	time_passed_since_last_meal;

	min_think = 0;
	if (i == 0)
	{
		if (log_status(THINKING, philo) != 0)
		{
			set_bool(&philo->program_ptr->program_mutex, true,
					&philo->program_ptr->end_sim);
			return ;
		}
	}
	handle_mutex(&philo->philo_mutex, LOCK); //todo safe
	time_passed_since_last_meal = (get_time_ms() - philo->end_last_meal);
	handle_mutex(&philo->philo_mutex, UNLOCK); //to do safe
	min_think = (philo->time_die - time_passed_since_last_meal
			- philo->time_eat) / 2;
	if (min_think < 0)
		min_think = 0;
	// else if (min_think > 600) //test cap needed?
	// 	min_think = 200;
	precise_usleep((min_think) * 1000);
	return ;
}

static int	eat(t_philo *philo)
{
	if (lock_forks(philo) != 0)
		return (1);
	if (set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal) != 0)
		return (1);
	philo->times_eaten++;
	if (log_status(EATING, philo) != 0)
		return (1);
	precise_usleep(philo->time_eat * 1000);
	if (philo->program_ptr->times_to_eat > 0
			&& philo->times_eaten == philo->program_ptr->times_to_eat)
	{
		if (set_bool(&philo->philo_mutex, true, &philo->full) != 0)
			return (1);
	}
	if (unlock_forks(philo) != 0)
		return (1);
	return (0);
}

/*
	Eat - Sleep - Think Routine
*/
static int	routine_loop(t_philo *philo)
{
	if (get_bool(&philo->philo_mutex, &philo->full))
		return (1);
	if (eat(philo) != 0)
		return (1);
	if (!sim_finished(philo->program_ptr))
	{
		if (log_status(SLEEPING, philo) != 0)
			return (1);
		precise_usleep(philo->time_sleep * 1000);
	}
	if (!sim_finished(philo->program_ptr))
		think(philo, 0);
	return (0);
}

/*
	Adds to counter when ready, spinlocks until all other philos ready. 
	Makes odd philos "think" (wait) first for asymmetry in taking forks 
	and preventing starvation.
	Setting last mealtime before start: to prevent philos dying at time = 0 when they shouldnt.
	Also after start: for correct actual start time.
*/
void	*routine(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;

	if (add_program_counter(&philo->program_ptr->program_mutex, &philo->program_ptr->running_philos) != 0)
		return (NULL);
	if (set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal) != 0)
		return (NULL);
	while (!all_threads_running(philo->program_ptr))
		;
	if (set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal) != 0)
		return (NULL);
	if (philo->num % 2)
		think(philo, 1);

	//start routine
	while (!sim_finished(philo->program_ptr))
	{
		if (routine_loop(philo) == 1)
			break ;
	}
	return (NULL);
}

