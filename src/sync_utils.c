/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:26 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/01 18:16:50 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"


/* make monitor thread wait until all philos run
*/
bool	all_threads_running(t_program *program)// check if ok
{
	int	counter_value;

	handle_mutex(&program->program_mutex, LOCK);
	counter_value = program->running_philos;
	handle_mutex(&program->program_mutex, UNLOCK);
	if (counter_value == program->num_philos)	
		return (true);
	else
		return (false);
}

int add_program_counter(pthread_mutex_t *mutex, int *counter)
{
	if (handle_mutex(mutex, LOCK) != 0)
		return (1);
	*counter = *counter + 1;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (1);
	return (0);
}

// Improved version of sleep function
int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

// Gets the current time in milliseconds

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}