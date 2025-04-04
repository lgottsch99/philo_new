/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/04 17:09:12 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

static int	single_philo(t_program *program)
{
	if (pthread_create(&program->philos[0]->thread, NULL, &lonely, (void *)program->philos[0]) != 0)
	{
		printf("Error creating thread\n");
		return (1);
	}
	return (0);
}

static int	init_threads(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->num_philos)
	{
		if (pthread_create(&program->philos[i]->thread, NULL, &routine, (void *)program->philos[i]) != 0)
		{
			printf("Error creating thread\n");
			return (1);
		}
		i++;
	}
	return (0);
}

static void	create_monitor_join(t_program *program)
{
	int	y;

	y = 0;
	if (pthread_create(&program->monitor, NULL, &monitor, program) != 0)
		return ;
	if (handle_mutex(&program->program_mutex, LOCK) != 0)
		return ;
	program->start_time = get_time_ms();
	if (handle_mutex(&program->program_mutex, UNLOCK) != 0)
		return ;
	while (y < program->num_philos)
	{
		if (pthread_join(program->philos[y]->thread, NULL) != 0)
			return ;
		y++;
	}
	set_bool(&program->program_mutex, true, &program->end_sim);
	if (pthread_join(program->monitor, NULL) != 0)
		return ;
	return ;
}

void	start_sim(t_program *program)
{
	int	y;
	int error;

	error = 0;
	y = 0;
	if (program->times_to_eat == 0)
		return ;
	else if (program->num_philos == 1)
		error = single_philo(program);
	else
	{
		if (init_threads(program) != 0)
		{
			error = 1;
			return ;
		}
	}
	if (error != 1)
		create_monitor_join(program);
	return ;
}
