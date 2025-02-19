#include "philo.h"

static int	take_single_fork(t_ph *ph)
{
	pthread_mutex_lock(&ph->sim->mtx_forks[left(ph)]);
	log_action(ph, "has taken a fork");
	usleep(ph->time_to_die * 1000);
	pthread_mutex_unlock(&ph->sim->mtx_forks[left(ph)]);
	return (-1);
}

static int	lock_fork(pthread_mutex_t *fork_mutex, t_ph *ph)
{
	if (pthread_mutex_lock(fork_mutex) == 0)
	{
		log_action(ph, "has taken a fork");
		return (1);
	}
	return (0);
}

static int	take_forks_even(t_ph *ph)
{
	if (!lock_fork(&ph->sim->mtx_forks[left(ph)], ph))
		return (0);
	if (!lock_fork(&ph->sim->mtx_forks[right(ph)], ph))
	{
		pthread_mutex_unlock(&ph->sim->mtx_forks[left(ph)]);
		return (0);
	}
	return (1);
}

static int	take_forks_odd(t_ph *ph)
{
	usleep(500);
	if (!lock_fork(&ph->sim->mtx_forks[right(ph)], ph))
		return (0);
	if (!lock_fork(&ph->sim->mtx_forks[left(ph)], ph))
	{
		pthread_mutex_unlock(&ph->sim->mtx_forks[right(ph)]);
		return (0);
	}
	return (1);
}

int	take_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
		return take_single_fork(ph);

	if (ph->index % 2 == 0)
	{
		if (!take_forks_even(ph))
			return (0);
	}
	else
	{
		if (!take_forks_odd(ph))
			return (0);
	}
	return (1);
}

void	put_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
	{
		pthread_mutex_unlock(&ph->sim->mtx_forks[left(ph)]);
		return ;
	}
	pthread_mutex_unlock(&ph->sim->mtx_forks[left(ph)]);
	pthread_mutex_unlock(&ph->sim->mtx_forks[right(ph)]);
}
