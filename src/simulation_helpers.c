/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:09 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 17:07:26 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

int	lock_forks(t_philo *philo)
{
	if (philo->num % 2 == 0)
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
	else
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

static void	actually_print(t_philo_state status, t_philo *philo, long logtime)
{
	if ((status == TAKE_FORK_1 || status == TAKE_FORK_2) && !sim_finished(philo->program_ptr))
		printf("%ld %i has taken a fork\n", logtime, philo->num);
	else if (status == EATING && !sim_finished(philo->program_ptr))
		printf("%ld %i is eating\n", logtime, philo->num);
	else if (status == SLEEPING && !sim_finished(philo->program_ptr))
		printf("%ld %i is sleeping\n", logtime, philo->num);
	else if (status == THINKING)
		printf("%ld %i is thinking\n", logtime, philo->num);
	else if (status == DEAD)
		printf("%ld %i died\n", logtime, philo->num);
	return ;
}

int	log_status(t_philo_state status, t_philo *philo)
{
	long	time;
	long	program_start;
	long	logtime;

	if (get_bool(&philo->philo_mutex, &philo->full))
		return (0);
	time = get_time_ms();
	program_start = get_long(&philo->program_ptr->program_mutex, &philo->program_ptr->start_time);
	logtime = time - program_start;
	if (handle_mutex(&philo->program_ptr->write_mutex, LOCK) != 0)
		return (1);
	actually_print(status, philo, logtime);
	if (handle_mutex(&philo->program_ptr->write_mutex, UNLOCK) != 0)
		return (1);
	return (0);
}
