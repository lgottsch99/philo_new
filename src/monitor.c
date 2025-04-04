/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:02:53 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 16:52:05 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

/*
checks if philo should die based on time gone by since beginning of last meal.
If a philo is full it just returns (philo itself stops when full).
*/
bool	philo_dead(t_philo *philo)
{
	long	time_gone_by;

	if (get_bool(&philo->philo_mutex, &philo->full))
		return (false);
	time_gone_by = get_time_ms() - get_long(&philo->philo_mutex,
			&philo->end_last_meal);
	if (time_gone_by > philo->program_ptr->time_die)
		return (true);
	else
		return (false);
}

/*
Monitoring thread to constantly check if any philo surpassed time to die.
Only starts at start of sim (when all philos are ready). Stops in case
philo dead or all full.
*/
void *monitor(void *data)
{
	t_program	*program;
	int			i;

	program = (t_program *)data;
	while (!all_threads_running(program))
		;
	precise_usleep(1000); //needed? test
	while (!sim_finished(program))
	{
		i = 0;
		while (i < program->num_philos && !sim_finished(program))
		{
			if (philo_dead(program->philos[i]))
			{
				set_bool(&program->program_mutex, true, &program->end_sim);
				log_status(DEAD, program->philos[i]);
			}
			i++;
		}
		precise_usleep(1000); //needed? test
	}
	return (NULL);
}
