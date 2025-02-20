/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:29:59 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 00:11:05 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_mutexes(t_sim *sim)
{
	pthread_mutex_destroy(&sim->mtx_is_dead);
	pthread_mutex_destroy(&sim->mtx_log);
	pthread_mutex_destroy(&sim->mtx_last_meal_time);
	pthread_mutex_destroy(&sim->mtx_meal_count);
}

static void	destroy_forks(t_sim *sim)
{
	int	i;

	if (!sim->mtx_forks)
		return;
	i = 0;
	while (i < sim->ph_count)
		pthread_mutex_destroy(&sim->mtx_forks[i++]);
	free(sim->mtx_forks);
	sim->mtx_forks = NULL;
}

static void	free_resources(t_sim **sim, t_ph **ph)
{
	if (*sim)
	{
		// Free meal_count if it was allocated
		if ((*sim)->meal_count)
		{
			free((*sim)->meal_count);
			(*sim)->meal_count = NULL;
		}
		// Free threads array if allocated
		if ((*sim)->ph_threads)
		{
			free((*sim)->ph_threads);
			(*sim)->ph_threads = NULL;
		}
		free(*sim);
		*sim = NULL;
	}
	if (*ph)
	{
		free(*ph);
		*ph = NULL;
	}
}

void	cleanup_exit(t_sim **sim, t_ph **ph, int status)
{
	if (!*sim)
		exit(status);
	destroy_mutexes(*sim);
	destroy_forks(*sim);
	free_resources(sim, ph);
	exit(status);
}
