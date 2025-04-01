/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:02:28 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/01 17:59:01 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

//wrappers to shorten main code
int	get_int(pthread_mutex_t *mutex, int *value)
{
	int	ret;

	if (handle_mutex(mutex, LOCK) != 0)
		return (-9999);
	ret = *value;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (-9999);
	return (ret);
}

int	set_int(pthread_mutex_t *mutex, int value, int *dest)
{
	if (handle_mutex(mutex, LOCK) != 0)
		return (1);
	*dest = value;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (1);
	return (0);	
}

bool	get_bool(pthread_mutex_t *mutex, bool *value)
{
	int	ret;

	handle_mutex(mutex, LOCK);
	ret = *value;
	handle_mutex(mutex, UNLOCK);
	return (ret);
}

int	set_bool(pthread_mutex_t *mutex, bool value, bool *dest)
{
	if (handle_mutex(mutex, LOCK) != 0)
		return (1);
	*dest = value;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (1);
	return (0);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	if (handle_mutex(mutex, LOCK) != 0)
		return (-9999);
	ret = *value;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (-9999);
	return (ret);
}

int	set_long(pthread_mutex_t *mutex, long value, long *dest)
{
	if (handle_mutex(mutex, LOCK) != 0)
		return (1); 
	*dest = value;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (1);
	return (0);
}

//wrapper to clearly see what happens
bool sim_finished(t_program *program)
{
	bool	ret;

	ret = get_bool(&program->program_mutex, &program->end_sim);
	return (ret);
}
