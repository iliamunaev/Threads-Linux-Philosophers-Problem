/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads_mutexes.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:37:54 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 09:15:56 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_fork_mutexes(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->ph_count)
	{
		if (pthread_mutex_init(&sim->mtx_forks[i], NULL) != 0)
		{
			print_err("Error: init_sim() could not init fork mutex.");
			i--;
			while (i >= 0)
			{
				if (pthread_mutex_destroy(&sim->mtx_forks[i]) != 0)
					return (EXIT_FAILURE);
				i--;
			}
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	init_main_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->mtx_is_dead, NULL) != 0
		|| pthread_mutex_init(&sim->mtx_log, NULL) != 0
		|| pthread_mutex_init(&sim->mtx_last_meal_time, NULL) != 0
		|| pthread_mutex_init(&sim->mtx_meal_count, NULL) != 0
		|| pthread_mutex_init(&sim->mtx_host, NULL) != 0)
	{
		print_err("Error: init_sim() could not init a main mutex.");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	alloc_forks_and_threads(t_sim *sim)
{
	sim->mtx_forks = malloc(sim->ph_count * sizeof(pthread_mutex_t));
	if (!sim->mtx_forks)
	{
		print_err("Error: malloc of mtx_forks failed.");
		return (EXIT_FAILURE);
	}
	sim->ph_threads = malloc(sim->ph_count * sizeof(pthread_t));
	if (!sim->ph_threads)
	{
		print_err("Error: malloc of ph_threads failed.");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
