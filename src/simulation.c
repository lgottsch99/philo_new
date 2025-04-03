/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 15:03:20 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/03 16:59:57 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

static int	single_philo(t_program *program)
{
	if (pthread_create(&program->philos[0]->thread, NULL, &lonely, (void *)program->philos[0]) != 0)
	{
		printf("Error creating thread\n");
		//destroy and free everything
		return (1);
	}
	return (0);
}

static int	init_threads(t_program *program)
{
	// printf("in init philo threads\n");
	int	i;

	i = 0;
	while (i < program->num_philos)
	{
		if (pthread_create(&program->philos[i]->thread, NULL, &routine, (void *)program->philos[i]) != 0)
		{
			printf("Error creating thread\n");
			//destroy and free everything
			return (1);
		}
		// pthread_detach(program->philos[i]->thread);
		printf("created thread %i\n", i + 1);
		i++;
	}
	return (0);
}


void	start_sim(t_program *program)
{
	// printf("start simulation\n");
	int	y;
	int error;

	error = 0;
	y = 0;
	if (program->times_to_eat == 0)
		return ;
	else if (program->num_philos == 1)
		error = single_philo(program);
	else
	{	//create philo threads
		if (init_threads(program) != 0)
		{
			error = 1;
			return ;
		}
	}
	if (error != 1)
	{
		// create monitor thread
		if (pthread_create(&program->monitor, NULL, &monitor, program) != 0) //todo secure
			return ;
	
		//get sim start time
		if (handle_mutex(&program->program_mutex, LOCK) != 0)
			return ;
		program->start_time = get_time_ms(); //maybe need to lock by mutex?
		if (handle_mutex(&program->program_mutex, UNLOCK) != 0)
			return ;

		while (y < program->num_philos)
		{
			if (pthread_join(program->philos[y]->thread, NULL) != 0)
				return ;
			y++;
		}
	

		//reached here? all threads finished//full
		// printf("ALL PHILOS FULL\n");
		set_bool(&program->program_mutex, true, &program->end_sim); //need to set this so monitor stops
		if (pthread_join(program->monitor, NULL) != 0)
			return ;
	
	}

	// printf("start time : %ld\n\n", program->start_time);

	// for (int i = 0; i < program->num_philos; i++)
	// {
	// 	print_philo(program->philos[i]);
	// }

	return ;
}