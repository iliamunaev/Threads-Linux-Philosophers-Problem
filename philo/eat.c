/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:15:30 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/20 23:17:59 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	lock_forks_and_log(t_ph *ph)
{
	if (take_forks(ph) == -1)
		return (-1);
	log_action(ph, "is eating");
	return (0);
}

static int	eat_loop(t_ph *ph, uint64_t start, uint64_t *finish)
{
	while (get_timestamp() - start < (uint64_t)ph->time_to_eat)
	{
		usleep(1000);
		if (check_death_during_action(ph) == -1)
			return (-1);
	}
	*finish = get_timestamp();
	return (0);
}

static void	update_meal_record(t_ph *ph, uint64_t finish)
{
	pthread_mutex_lock(&ph->sim->mtx_meal_count);
	if (ph->sim->meal_count)
		ph->sim->meal_count[ph->index]++;
	pthread_mutex_unlock(&ph->sim->mtx_meal_count);
	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	ph->last_meal_time = finish;
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);
}

int	eat(t_ph *ph)
{
	uint64_t	start;
	uint64_t	finish;

	if (lock_forks_and_log(ph) == -1)
		return (-1);
	start = get_timestamp();
	if (eat_loop(ph, start, &finish) == -1)
	{
		put_forks(ph);
		return (-1);
	}
	update_meal_record(ph, finish);
	put_forks(ph);
	return (0);
}
