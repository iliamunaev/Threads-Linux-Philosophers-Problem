/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:13:39 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/20 23:17:37 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_single_fork(t_ph *ph)
{
	pthread_mutex_lock(&ph->sim->mtx_forks[ph->index]);
	log_action(ph, "has taken a fork");
	usleep(ph->time_to_die * 1000);
	pthread_mutex_unlock(&ph->sim->mtx_forks[ph->index]);
	pthread_mutex_lock(&ph->sim->mtx_is_dead);
	if (!ph->sim->all_dead && !ph->is_dead)
	{
		ph->sim->all_dead = true;
		ph->is_dead = true;
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
		log_action(ph, "died");
	}
	else
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
	return (-1);
}

static int	get_lower_fork_id(t_ph *ph)
{
	int	l;
	int	r;

	l = left(ph);
	r = right(ph);
	if (l < r)
		return (l);
	return (r);
}

static int	get_higher_fork_id(t_ph *ph)
{
	int	l;
	int	r;

	l = left(ph);
	r = right(ph);
	if (l > r)
		return (l);
	return (r);
}

int	take_forks(t_ph *ph)
{
	int	fork1;
	int	fork2;

	if (ph->sim->ph_count == 1)
		return (take_single_fork(ph));
	fork1 = get_lower_fork_id(ph);
	fork2 = get_higher_fork_id(ph);
	pthread_mutex_lock(&ph->sim->mtx_forks[fork1]);
	log_action(ph, "has taken a fork");
	pthread_mutex_lock(&ph->sim->mtx_forks[fork2]);
	log_action(ph, "has taken a fork");
	return (0);
}

void	put_forks(t_ph *ph)
{
	int	fork1;
	int	fork2;

	if (ph->sim->ph_count == 1)
	{
		pthread_mutex_unlock(&ph->sim->mtx_forks[ph->index]);
		return ;
	}
	fork1 = get_lower_fork_id(ph);
	fork2 = get_higher_fork_id(ph);
	pthread_mutex_unlock(&ph->sim->mtx_forks[fork2]);
	pthread_mutex_unlock(&ph->sim->mtx_forks[fork1]);
}
