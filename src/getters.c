/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:02:28 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 16:47:44 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

/*
Functions getting variables in a thread safe way, using mutexes
Also wrappers for code readability
*/

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

bool	get_bool(pthread_mutex_t *mutex, bool *value)
{
	int	ret;

	handle_mutex(mutex, LOCK);
	ret = *value;
	handle_mutex(mutex, UNLOCK);
	return (ret);
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
