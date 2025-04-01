/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:23:14 by lgottsch          #+#    #+#             */
/*   Updated: 2025/04/01 17:07:48 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

void	pre_init(t_program *program)
{
	program->num_philos = 0;
	program->end_sim = false;
	program->time_die = 0;
	program->time_eat = 0;
	program->time_sleep = 0;
	program->times_to_eat = -1;
	program->start_time = 0;
	program->fork_mutexes = NULL;
	program->philos = NULL;
}

int	main(int argc, char *argv[])
{
	t_program	program;

	pre_init(&program);
	//1. check valid input
	if (check_input_valid(argc, argv) == 0)
	{
		// printf("input valid\n");
		//2. init structs (but not create philos yet!)
		if (init_program(&program, argv) != 0)
		{
			free_program(&program);
			return (1);
		}

		//3. start sim (create threads)
		start_sim(&program);
		free_program(&program);
	}
	return (0);
}
