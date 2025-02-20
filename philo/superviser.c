/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   superviser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:42:07 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/20 23:42:08 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*superviser(void *arg)
{
	t_sim	*sim;
	int		i;
	int		full_philosophers;

	sim = (t_sim *)arg;
	if (sim->num_meals_to_eat <= 0)
		return (NULL);
	while (true)
	{
		full_philosophers = 0;
		pthread_mutex_lock(&sim->mtx_meal_count);
		i = 0;
		while (i < sim->ph_count)
		{
			if (sim->meal_count[i] >= sim->num_meals_to_eat)
				full_philosophers++;
			i++;
		}
		pthread_mutex_unlock(&sim->mtx_meal_count);
		if (full_philosophers == sim->ph_count)
		{
			pthread_mutex_lock(&sim->mtx_is_dead);
			sim->all_dead = true;
			pthread_mutex_unlock(&sim->mtx_is_dead);
			printf("[DEBUG] All philosophers have eaten enough. Stopping simulation.\n"); // debuging
			return (NULL);
		}
		pthread_mutex_lock(&sim->mtx_is_dead);
		if (sim->all_dead)
		{
			pthread_mutex_unlock(&sim->mtx_is_dead);
			return (NULL);
		}
		pthread_mutex_unlock(&sim->mtx_is_dead);
		usleep(1000);
	}
}
