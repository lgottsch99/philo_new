/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:31:22 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 16:35:10 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

/*
Functions setting variables in a thread safe way, using mutexes
Also wrappers for code readability
*/

int	set_int(pthread_mutex_t *mutex, int value, int *dest)
{
	if (handle_mutex(mutex, LOCK) != 0)
		return (1);
	*dest = value;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (1);
	return (0);	
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

int	set_long(pthread_mutex_t *mutex, long value, long *dest)
{
	if (handle_mutex(mutex, LOCK) != 0)
		return (1); 
	*dest = value;
	if (handle_mutex(mutex, UNLOCK) != 0)
		return (1);
	return (0);
}

