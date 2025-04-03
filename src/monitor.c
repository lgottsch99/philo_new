/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:02:53 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/03 19:12:50 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

//check if time last meal to now > time die (but maybe hilo is full)
bool	philo_dead(t_philo *philo)
{
	long	time_gone_by;

	if (get_bool(&philo->philo_mutex, &philo->full)) //philo is full and iis gonna end on its own 
		return (false);

	time_gone_by = get_time_ms() - get_long(&philo->philo_mutex, &philo->end_last_meal);
	if (time_gone_by > philo->program_ptr->time_die)
		return (true);
	else
		return (false);

}


void *monitor(void *data)
{
	t_program	*program;
	int			i;

	program = (t_program *)data;
	//sync: all philos should run before start to monitor
	while (!all_threads_running(program))//spinlock OK
		;
	
	precise_usleep(100);
	// printf("MONITOR STARTS -----------\n");
	while (!sim_finished(program))
	{

		i = 0;
		//const check if elapsed time > time die for ALL philos
		while (i < program->num_philos && !sim_finished(program))
		{
			//check if died
			if (philo_dead(program->philos[i]))
			{
				set_bool(&program->program_mutex, true, &program->end_sim);
				log_status(DEAD, program->philos[i]);
			}
			i++;
		}
	}
	return (NULL);
}