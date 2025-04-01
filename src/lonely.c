/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lonely.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:01:55 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/01 18:29:39 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

//fake to take forks, sleep until monitor kills him
void	*lonely(void *data)
{
	t_philo *philo;
	long	time;

	philo = (t_philo *)data;

	// wait until all philos ready
	// printf("%i waiting for start\n", philo->num);
	time = get_current_time();
	while (time <= philo->philo_start)
	{
		ft_usleep(100);
		time = get_current_time();
	}

	// printf("%i starting\n", philo->num);
	//sync w monitor ->increase a program counter
	if (add_program_counter(&philo->program_ptr->program_mutex, &philo->program_ptr->running_philos) != 0)
		return (NULL); //just stop

	//set last meal time
	if (set_long(&philo->philo_mutex, get_current_time(), &philo->end_last_meal) != 0)//thread safe
		return (NULL);

	if (log_status(TAKE_FORK_1, philo) != 0) //fake to take fork
		return (NULL);
		
	while (!sim_finished(philo->program_ptr)) //wait until killed
		ft_usleep(100);
	return (NULL);
}