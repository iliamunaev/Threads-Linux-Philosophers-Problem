/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:16:15 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 09:54:13 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	perform_cycle(t_ph *ph)
{
	uint64_t	last_meal;

	pthread_mutex_lock(&ph->sim->mtx_is_dead);
	if (ph->sim->all_dead || ph->is_dead)
	{
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
		return (-1);
	}
	pthread_mutex_unlock(&ph->sim->mtx_is_dead);
	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	last_meal = ph->last_meal_time;
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);
	if (check_philosopher_death(ph, last_meal) == -1)
		return (-1);
	if (eat(ph) == -1)
		return (-1);
	if (go_sleep(ph) == -1)
		return (-1);
	// think(ph);
	return (0);
}

void	*philo(void *arg)
{
	t_ph	*ph;
	int		is_stopped;

	ph = (t_ph *)arg;
	while (true)
	{
		pthread_mutex_lock(&ph->sim->mtx_is_dead);
		is_stopped = (ph->sim->all_dead || ph->is_dead);
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
		if (is_stopped)
			return (NULL);
		if (ph->sim->num_meals_to_eat > 0)
		{
			pthread_mutex_lock(&ph->sim->mtx_meal_count);
			if (ph->sim->meal_count[ph->index] >= ph->sim->num_meals_to_eat)
			{
				pthread_mutex_unlock(&ph->sim->mtx_meal_count);
				return (NULL);
			}
			pthread_mutex_unlock(&ph->sim->mtx_meal_count);
		}
		if (perform_cycle(ph) == -1)
			return (NULL);
	}
	return (NULL);
}
