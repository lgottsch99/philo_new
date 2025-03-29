/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:51:55 by lgottsch          #+#    #+#             */
/*   Updated: 2025/03/28 16:44:52 by lgottsch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philos.h"

static int	check_digits(char *arg)
{
	int	i;
	int	len;

	if (!arg)
		return (1);
	i = 0;
	len = 0;
	while (arg[len] != '\0')
		len++;
	while (i < len)
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

///all numeric (and > 0 except arg nr 5) args
	//for each arg:
		//check if only digits
		//if yes check atoi > 0? (excepils.c ./src/free.c ./src/init.c ./src/routine.c ./src/monitor.c -o philo
int	check_input_valid(int argc, char *argv[])
{
	int	i;

	i = 1;
	if (argc < 5 || argc > 6)
	{
		printf("Usage: <numPhilos> <timeToDie> <timeToEat> <timeToSleep> [<numberOfTimesToEat>]\n");
		return (1);
	}
	while (i < argc)
	{
		if (argv[1] && argv[1][0] == '0')
		{
			printf("Num Philos needs to be > 0!\n");
			return (1);

		}
		if (check_digits(argv[i]) == 1)
		{
			printf("Input invalid: only positive numeric args allowed!\n");
			return (1);
		}
		i++;
	}
	return (0);
}
