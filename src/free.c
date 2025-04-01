/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:02:19 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/01 17:14:41 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

void	free_philo(t_philo *philo)
{
	if (philo)
	{
		handle_mutex(&philo->philo_mutex, DESTROY);
		philo->program_ptr = NULL;
	}
}

void	destroy_fork_mutex(t_program *program)
{
	int	i;
	pthread_mutex_t	*tmp;

	tmp = program->fork_mutexes;
	i = 0;
	while (i < program->num_philos)
	{
		handle_mutex(tmp, DESTROY);
		i++;
		tmp++;
	}
	tmp = NULL;
}

void	free_program(t_program *program)
{
	printf("freeing\n");
	int	i;

	i = 0;
	handle_mutex(&program->program_mutex, DESTROY);
	handle_mutex(&program->write_mutex, DESTROY);

	destroy_fork_mutex(program);
	free(program->fork_mutexes);
	program->fork_mutexes = NULL;

	//loop thru philos and free each
	if (program->philos)
	{
		while(i < program->num_philos)
		{
			if (program->philos[i])
			{
				free_philo(program->philos[i]);
				free(program->philos[i]);
				program->philos[i] = NULL;
			}
			i++;
		}
		//free philos ptr
		free(program->philos);
		program->philos = NULL;
	}


}