/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:20:55 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/01 18:18:41 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	neg;
	int	num;

	num = 0;
	neg = 1;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			neg = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = num * 10 + (nptr[i] - 48);
		i++;
	}
	return (num * neg);
}

long	get_time_ms()
{
	struct timeval time;
	long	sec;
	long	microsec;
	long	millisec;

	gettimeofday(&time, NULL);
	sec = time.tv_sec;
	microsec = time.tv_usec;
	millisec = (sec * 1000) + (microsec / 1000);
	return (millisec);
}


int	mutex_error(int status)
{
	if (status == 0)
		return (0);
	else
		return (1);
}

//all mutex fts return 0 if ok, else error 
int	handle_mutex(pthread_mutex_t *mutex, t_opcode code)
{
	int 	stat;

	stat = 0;
	if (code == INIT)
		stat = mutex_error(pthread_mutex_init(mutex, NULL));
	else if (code == LOCK)
		stat = mutex_error(pthread_mutex_lock(mutex));
	else if (code == UNLOCK)
		stat = mutex_error(pthread_mutex_unlock(mutex));
	else if (code == DESTROY)
		stat = mutex_error(pthread_mutex_destroy(mutex));
	else
		printf("wrong code for mutex\n");
	return (stat);
}

int	log_status(t_philo_state status, t_philo *philo)
{
	long	time;
	long	program_start;

	time = get_time_ms();
	if (get_bool(&philo->philo_mutex, &philo->full)) // thread safe?
		return (0);

	program_start = get_long(&philo->program_ptr->program_mutex, &philo->program_ptr->start_time);

	if (handle_mutex(&philo->program_ptr->write_mutex, LOCK) != 0)
		return (1);

	if ((status == TAKE_FORK_1 || status == TAKE_FORK_2) && !sim_finished(philo->program_ptr))
		printf("%ld %i has taken a fork\n", time - program_start - OFFSET_TIME, philo->num);
	else if (status == EATING && !sim_finished(philo->program_ptr))
		printf("%ld %i is eating\n", time - program_start - OFFSET_TIME, philo->num);
	else if (status == SLEEPING && !sim_finished(philo->program_ptr))
		printf("%ld %i is sleeping\n", time - program_start - OFFSET_TIME, philo->num);
	else if (status == THINKING)
		printf("%ld %i is thinking\n", time - program_start - OFFSET_TIME, philo->num);
	else if (status == DEAD)
		printf("%ld %i died\n", time - program_start - OFFSET_TIME, philo->num);

	if (handle_mutex(&philo->program_ptr->write_mutex, UNLOCK) != 0)
		return (1);
	return (0);
}

void	print_philo(t_philo *philo)//rm
{
	printf("num: %i\n", philo->num);
	printf("times eaten: %i\n", philo->times_eaten);
	printf("end last meal: %li\n", philo->end_last_meal);
	printf("philo start : %ld\n", philo->philo_start);
	printf("mutex own fork: %p\n", (void *)philo->mutex_own_fork);
	printf("mutex right fork: %p\n", (void *)philo->mutex_fork_right);
	printf("pprogram ptr: %p\n", philo->program_ptr);
	printf("\n");
}

