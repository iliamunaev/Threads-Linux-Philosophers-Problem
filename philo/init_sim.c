/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:37:44 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 09:16:20 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	*init_meal_count(t_sim *sim)
{
	int	*meal_arr;
	int	i;

	meal_arr = malloc(sim->ph_count * sizeof(int)); // mem leak tested
	if (!meal_arr)
		return (NULL);
	i = 0;
	while (i < sim->ph_count)
	{
		meal_arr[i] = 0;
		i++;
	}
	return (meal_arr);
}

static int	setup_meal_count(t_sim *sim, char **av)
{
	if (!av[5])
	{

		sim->num_meals_to_eat = -1;
		return (0);
	}
	sim->num_meals_to_eat = ft_atoi(av[5]);
	sim->meal_count = init_meal_count(sim);
	if (!sim->meal_count)
	{
		free(sim);
		print_err("Error: malloc of meal_count failed.");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void	init_sim_defaults(t_sim *sim, char **av)
{
	sim->ph_count = ft_atoi(av[1]);
	sim->all_dead = false;
	sim->all_satiated = false;
	sim->mtx_forks = NULL;
	sim->meal_count = NULL;
	sim->ph_threads = NULL;
	sim->active_eaters = 0;
}

static t_sim	*alloc_sim_struct(char **av)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
	{
		print_err("Error: malloc of t_sim failed.");
		return (NULL);
	}
	init_sim_defaults(sim, av);
	if (setup_meal_count(sim, av) == EXIT_FAILURE)
		return (NULL);
	return (sim);
}

t_sim	*init_sim(char **av)
{
	t_sim	*sim;

	sim = alloc_sim_struct(av);
	if (!sim)
	return (NULL);
	if (alloc_forks_and_threads(sim) == EXIT_FAILURE)
		return (NULL);
	if (init_main_mutexes(sim) == EXIT_FAILURE)
		return (NULL);
	if (init_fork_mutexes(sim) == EXIT_FAILURE)
		return (NULL);
	return (sim);
}
