/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:26 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/03 17:02:45 by lgottsch         ###   ########.fr       */
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


static long get_elapsed_time_microseconds(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
}

void precise_usleep(long usec) {
    struct timeval start, current;
    long elapsed;
    long rem;

    gettimeofday(&start, NULL);
    do {
        gettimeofday(&current, NULL);
        elapsed = get_elapsed_time_microseconds(start, current);
        rem = usec - elapsed;

        if (rem > 1000) 
            usleep(rem / 2);
        
    } while (elapsed < usec);
}