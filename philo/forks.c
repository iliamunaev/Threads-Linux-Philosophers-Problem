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

int take_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
		return (take_single_fork(ph));
	if (ph->index % 2 == 0)
		usleep(1000);
	pthread_mutex_lock(&ph->sim->mtx_forks[left(ph)]);
	log_action(ph, "has taken a fork");
	pthread_mutex_lock(&ph->sim->mtx_forks[right(ph)]);
	log_action(ph, "has taken a fork");
	return (0);
}


void put_forks(t_ph *ph)
{
	int	left_id;
	int	right_id;

	if (ph->sim->ph_count == 1)
	{
		pthread_mutex_unlock(&ph->sim->mtx_forks[ph->index]);
		return ;
	}
	left_id = left(ph);
	right_id = right(ph);
	if (left_id > right_id)
	{
		int tmp = left_id;
		left_id = right_id;
		right_id = tmp;
	}
	pthread_mutex_unlock(&ph->sim->mtx_forks[right_id]);
	pthread_mutex_unlock(&ph->sim->mtx_forks[left_id]);
}
