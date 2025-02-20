/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:43:29 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/20 23:43:30 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_input_valid(int ac, char **av)
{
	int	i;
	int	val;

	if (ac < 5 || ac > 6)
	{
		print_err("Usage: ./philo philos die_t eat_t sleep_t [num_meals]");
		return (false);
	}
	i = 1;
	while (i < ac)
	{
		val = ft_atoi(av[i]);
		if (val <= 0)
		{
			print_err("Error: arguments must be positive integers.");
			return (false);
		}
		i++;
	}
	return (true);
}
