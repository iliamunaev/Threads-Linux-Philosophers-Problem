#include "philo.h"

void think(t_ph *ph)
{
	log_action(ph, "is thinking");
	usleep(10);
}

static int	take_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
	{
		pthread_mutex_lock(&ph->sim->forks_m[left(ph)]);
		log_action(ph, "has taken a fork");
		usleep(ph->time_to_die * 1000);
		pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
		return (-1);
	}
	if (ph->index % 2 == 0)
	{
		if(pthread_mutex_lock(&ph->sim->forks_m[left(ph)]) == 0)
			log_action(ph, "has taken a fork");
		if (pthread_mutex_lock(&ph->sim->forks_m[right(ph)]) != 0)
		{
			pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
			return (0);
		}
	}
	else
	{
		usleep(500);
		if(pthread_mutex_lock(&ph->sim->forks_m[right(ph)]) == 0)
			log_action(ph, "has taken a fork");
		if (pthread_mutex_lock(&ph->sim->forks_m[left(ph)]) != 0)
		{
			pthread_mutex_unlock(&ph->sim->forks_m[right(ph)]);
			return (0);
		}
	}
	log_action(ph, "has taken a fork");
	return (0);
}

static void	put_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
	{
		pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
		return ;
	}
	pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
	pthread_mutex_unlock(&ph->sim->forks_m[right(ph)]);
}
int	eat(t_ph *ph)
{
	if (take_forks(ph) == -1)
		return (-1);
	pthread_mutex_lock(&ph->sim->last_meal_time_m);
	ph->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&ph->sim->last_meal_time_m);
	log_action(ph, "is eating");
	usleep(ph->time_to_eat * 1000);
	pthread_mutex_lock(&ph->sim->last_meal_time_m);
	ph->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&ph->sim->last_meal_time_m);
	put_forks(ph);
	return (0);
}
int go_sleep(t_ph *ph)
{
	uint64_t	start;

	start = get_timestamp();
	log_action(ph, "is sleeping");

	while (get_timestamp() - start < (uint64_t)ph->time_to_sleep)
	{
		usleep(1000); // Sleep 1 ms

		// Check if philosopher died during sleep
		pthread_mutex_lock(&ph->sim->last_meal_time_m);
		uint64_t time_since_meal = get_timestamp() - ph->last_meal_time;
		pthread_mutex_unlock(&ph->sim->last_meal_time_m);

		if (time_since_meal >= (uint64_t)ph->time_to_die)
		{
			pthread_mutex_lock(&ph->sim->is_dead_m);
			if (!ph->sim->all_dead && !ph->is_dead)
			{
				ph->sim->all_dead = true;
				ph->is_dead = true;
				pthread_mutex_unlock(&ph->sim->is_dead_m);

				log_action(ph, "died");
				return -1;
			}
			pthread_mutex_unlock(&ph->sim->is_dead_m);
			return -1;
		}
	}
	return 0;
}


void	*philosopher(void *arg)
{
	t_ph		*ph;
	uint64_t	last_meal;

	ph = (t_ph *)arg;
	while (true)
	{
		pthread_mutex_lock(&ph->sim->is_dead_m);
		if (ph->sim->all_dead || ph->is_dead)
		{
			pthread_mutex_unlock(&ph->sim->is_dead_m);
			return (NULL);
		}
		pthread_mutex_unlock(&ph->sim->is_dead_m);
		pthread_mutex_lock(&ph->sim->last_meal_time_m);
		last_meal = ph->last_meal_time;
		pthread_mutex_unlock(&ph->sim->last_meal_time_m);
		if (get_timestamp() - last_meal >= (uint64_t)ph->time_to_die)
		{
			pthread_mutex_lock(&ph->sim->is_dead_m);
			if (!ph->sim->all_dead)
			{
				ph->sim->all_dead = true;
				ph->is_dead = true;
				log_action(ph, "died");
			}
			pthread_mutex_unlock(&ph->sim->is_dead_m);
			return (NULL);
		}
		if (eat(ph) == -1)
		{
			pthread_mutex_lock(&ph->sim->is_dead_m);
			if (!ph->sim->all_dead)
			{
				ph->sim->all_dead = true;
				ph->is_dead = true;
				log_action(ph, "died");
			}
			pthread_mutex_unlock(&ph->sim->is_dead_m);
			return (NULL);
		}
		if(go_sleep(ph) == -1)
			return (NULL);
		think(ph);
	}
	return (NULL);
}
