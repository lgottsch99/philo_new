/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:20:55 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 17:06:45 by lgottsch         ###   ########.fr       */
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
/*
	Returns current time in millisec
*/
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

//wrapper to handle mutex error
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

/*
	Used for debugging 
*/
void	print_philo(t_philo *philo)
{
	printf("num: %i\n", philo->num);
	printf("times eaten: %i\n", philo->times_eaten);
	printf("end last meal: %li\n", philo->end_last_meal);
	printf("philo start : %ld\n", philo->philo_start);
	printf("mutex own fork: %p\n", (void *)philo->mutex_own_fork);
	printf("mutex right fork: %p\n", (void *)philo->mutex_fork_right);
	printf("program ptr: %p\n", philo->program_ptr);
	printf("\n");
}

