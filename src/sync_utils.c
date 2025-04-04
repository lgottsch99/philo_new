/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:26 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 16:37:41 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

/* makes sim wait until all philos ready to run
*/
bool	all_threads_running(t_program *program)
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

/*
adds to a var that keeps track of threads that are ready to run
*/
int add_program_counter(pthread_mutex_t *mutex, int *counter)
{
	if (handle_mutex(mutex, LOCK) != 0)
		return (1);
	*counter = *counter + 1;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (1);
	return (0);
}

/*
L at end of number: type safety
L ensures math is done using longs, prevents int overflow
*/
static long get_elapsed_time_microseconds(struct timeval start, struct timeval end)
{
	long	elapsed;

	elapsed = (end.tv_sec - start.tv_sec) * 1000000L + (end.tv_usec - start.tv_usec);
    return (elapsed);
}

/*
more precise than normal usleep
mix between spinlock and sleep
*/
void precise_usleep(long microsec)
{
    struct timeval start;
	struct timeval current;
    long elapsed;
    long rem;

	elapsed = 0;
    gettimeofday(&start, NULL);
	while (elapsed < microsec)
    {
        gettimeofday(&current, NULL);
        elapsed = get_elapsed_time_microseconds(start, current);
        rem = microsec - elapsed;
        if (rem > 1000) 
            usleep(rem / 2);   
    }
}

/*
wrapper for readability
*/
bool sim_finished(t_program *program)
{
	bool	ret;

	ret = get_bool(&program->program_mutex, &program->end_sim);
	return (ret);
}
