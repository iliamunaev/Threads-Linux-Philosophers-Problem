
#include "philo.h"

void think(t_ph *ph)
{
	log_action(ph, "is thinking");
	usleep(1000);
}
static int	take_single_fork(t_ph *ph)
{
	pthread_mutex_lock(&ph->sim->forks_m[left(ph)]);
	log_action(ph, "has taken a fork");
	usleep(ph->time_to_die * 1000);
	pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
	return (-1);
}

static int	lock_even_forks(t_ph *ph)
{
	pthread_mutex_lock(&ph->sim->forks_m[left(ph)]);
	if (pthread_mutex_lock(&ph->sim->forks_m[right(ph)]) != 0)
	{
		pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
		return (-1);
	}
	return (0);
}

static int	lock_odd_forks(t_ph *ph)
{
	usleep(500);
	pthread_mutex_lock(&ph->sim->forks_m[right(ph)]);
	if (pthread_mutex_lock(&ph->sim->forks_m[left(ph)]) != 0)
	{
		pthread_mutex_unlock(&ph->sim->forks_m[right(ph)]);
		return (-1);
	}
	return (0);
}

int	take_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
		return (take_single_fork(ph));
	if (ph->index % 2 == 0)
		return (lock_even_forks(ph));
	return (lock_odd_forks(ph));
}

void put_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
	{
		pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
		return ;
	}
	pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
	pthread_mutex_unlock(&ph->sim->forks_m[right(ph)]);
}

void	eat(t_ph *ph)
{
	int	forks;

	forks = take_forks(ph);
	if (forks == -1)
		return ;
	log_action(ph, "is eating");
	pthread_mutex_lock(&ph->sim->last_meal_time_m);
	ph->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&ph->sim->last_meal_time_m);
	usleep(ph->time_to_eat * 1000);
	put_forks(ph);
}

void go_sleep(t_ph *ph)
{
	log_action(ph, "is sleeping");
	usleep(ph->time_to_sleep * 1000);
}

static int	check_global_death(t_ph *ph)
{
	pthread_mutex_lock(&ph->sim->is_dead_m);
	if (ph->is_dead || ph->sim->all_dead)
	{
		pthread_mutex_unlock(&ph->sim->is_dead_m);
		return (1);
	}
	pthread_mutex_unlock(&ph->sim->is_dead_m);
	return (0);
}

static int	check_starvation(t_ph *ph)
{
	int64_t	last_meal;

	pthread_mutex_lock(&ph->sim->last_meal_time_m);
	last_meal = ph->last_meal_time;
	pthread_mutex_unlock(&ph->sim->last_meal_time_m);

	if ((get_timestamp() - last_meal) >= (uint64_t)ph->time_to_die)
	{
		pthread_mutex_lock(&ph->sim->is_dead_m);
		ph->is_dead = true;
		ph->sim->all_dead = true;
		pthread_mutex_unlock(&ph->sim->is_dead_m);

		log_action(ph, "died");
		return (1);
	}
	return (0);
}

void	*philosopher(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	if (!ph || !ph->sim)
		return (NULL);
	while (1)
	{
		if (check_global_death(ph))
			break ;
		if (check_starvation(ph))
			return (NULL);
		eat(ph);
		go_sleep(ph);
		think(ph);
	}
	pthread_mutex_lock(&ph->sim->is_dead_m);
	ph->is_dead = true;
	ph->sim->all_dead = true;
	pthread_mutex_unlock(&ph->sim->is_dead_m);
	log_action(ph, "died");
	return (NULL);
}
