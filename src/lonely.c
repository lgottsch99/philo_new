/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lonely.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:01:55 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 16:39:39 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

/*
routine for single philo
taking one fork and waiting until monitor kills him
*/
void	*lonely(void *data)
{
	t_philo *philo;

	philo = (t_philo *)data;
	if (add_program_counter(&philo->program_ptr->program_mutex,
			&philo->program_ptr->running_philos) != 0)
		return (NULL);
	if (set_long(&philo->philo_mutex, get_time_ms(), &philo->end_last_meal) != 0)
		return (NULL);
	if (log_status(TAKE_FORK_1, philo) != 0)
		return (NULL);
	while (!sim_finished(philo->program_ptr))
		precise_usleep(100);
	return (NULL);
}
