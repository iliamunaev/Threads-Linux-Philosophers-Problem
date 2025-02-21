/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ph.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:19:25 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 08:43:39 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	fill_ph_data(t_ph *ph, t_sim *sim, char **av, int i)
{
	uint64_t	time;

	ph->sim = sim;
	ph->index = i;
	time = get_timestamp();
	if (time == 0)
	{
		print_err("Error: init_ph() get_timestamp failed.");
		return (EXIT_FAILURE);
	}
	ph->last_meal_time = time;
	ph->time_to_die = ft_atoi(av[2]);
	ph->time_to_eat = ft_atoi(av[3]);
	ph->time_to_sleep = ft_atoi(av[4]);
	ph->is_dead = false;
	return (EXIT_SUCCESS);
}

t_ph	*init_ph(t_sim *sim, char **av)
{
	int		ph_count;
	int		i;
	t_ph	*ph;

	ph_count = sim->ph_count;
	ph = malloc(ph_count * sizeof(t_ph));
	// ph = NULL;
	if (!ph)
	{
		print_err("Error: init_ph() malloc failed.");
		return (NULL);
	}
	i = 0;
	while (i < ph_count)
	{
		if (fill_ph_data(&ph[i], sim, av, i) == EXIT_FAILURE)
		{
			print_err("Error: init_ph() fill data failed.");
			return (NULL);
		}
		i++;
	}
	return (ph);
}
