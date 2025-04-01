/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:09 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/01 18:16:48 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

int	lock_forks(t_philo *philo)
{
	if (philo->num % 2 == 0)//if even grab own fork first
	{
		if (handle_mutex(philo->mutex_own_fork, LOCK) != 0)
			return (1);
		if (log_status(TAKE_FORK_1, philo) != 0)
			return (1);
		if (handle_mutex(philo->mutex_fork_right, LOCK) != 0)
			return (1);
		if (log_status(TAKE_FORK_2, philo) != 0)
			return (1);
	}
	else //uneven grab right fork first
	{
		if (handle_mutex(philo->mutex_fork_right, LOCK) != 0)
			return (1);
		if (log_status(TAKE_FORK_2, philo) != 0)
			return (1);
		if (handle_mutex(philo->mutex_own_fork, LOCK) != 0)
			return (1);
		if (log_status(TAKE_FORK_1, philo) != 0)
			return (1);
	}
	return (0);
}

int	unlock_forks(t_philo *philo)
{
	if (handle_mutex(philo->mutex_own_fork, UNLOCK) != 0)
		return (1);
	if (handle_mutex(philo->mutex_fork_right, UNLOCK) != 0)
		return (1);
	return (0);
}
