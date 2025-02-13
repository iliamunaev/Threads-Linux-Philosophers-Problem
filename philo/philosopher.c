#include "philo.h"


void think(t_ph *ph)
{
	log_action(ph, "is thinking");
	// usleep(1000); // Arbitrary short wait
}

static int take_forks(t_ph *ph)
{
	// If there is only one philosopher, they can never get two forks
	if (ph->sim->ph_count == 1)
	{
		pthread_mutex_lock(&ph->sim->forks_m[left(ph)]);
		log_action(ph, "has taken a fork");
		// Starve this philosopher by waiting for time_to_die
		usleep(ph->time_to_die * 1000);
		pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
		return -1;
	}

	/* Even index: lock left then right; odd index: lock right then left */
	if (ph->index % 2 == 0)
	{
		pthread_mutex_lock(&ph->sim->forks_m[left(ph)]);
		if (pthread_mutex_lock(&ph->sim->forks_m[right(ph)]) != 0)
		{
			/* If for some reason we fail, unlock the left */
			pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
			return -1;
		}
	}
	else
	{
	   usleep(500); // small delay to reduce race for odd philosophers
		pthread_mutex_lock(&ph->sim->forks_m[right(ph)]);
		if (pthread_mutex_lock(&ph->sim->forks_m[left(ph)]) != 0)
		{
			pthread_mutex_unlock(&ph->sim->forks_m[right(ph)]);
			return -1;
		}
	}
	log_action(ph, "has taken a fork");
	log_action(ph, "has taken a fork");
	return 0;
}


static void put_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
	{
		pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
		return;
	}
	pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
	pthread_mutex_unlock(&ph->sim->forks_m[right(ph)]);
}
int eat(t_ph *ph)
{
	if (take_forks(ph) == -1)
		return -1;

	pthread_mutex_lock(&ph->sim->last_meal_time_m);
	ph->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&ph->sim->last_meal_time_m);

	log_action(ph, "is eating");


	usleep(ph->time_to_eat * 1000);

	// After finishing the meal
	pthread_mutex_lock(&ph->sim->last_meal_time_m);
	ph->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&ph->sim->last_meal_time_m);
	put_forks(ph);

	return 0;
}

int go_sleep(t_ph *ph)
{
	log_action(ph, "is sleeping");
	uint64_t start = get_timestamp();

	while (get_timestamp() - start < (uint64_t)ph->time_to_sleep)
	{
		usleep(100);
		pthread_mutex_lock(&ph->sim->is_dead_m);
		if (ph->is_dead)
		{
			pthread_mutex_unlock(&ph->sim->is_dead_m);
			log_action(ph, "died");
			return -1;
		}
		pthread_mutex_unlock(&ph->sim->is_dead_m);
	}

	return 0;
}



void *philosopher(void *arg)
{
	t_ph *ph = (t_ph *)arg;

	while (true)
	{
		pthread_mutex_lock(&ph->sim->is_dead_m);
		if (ph->sim->all_dead || ph->is_dead)
		{
			pthread_mutex_unlock(&ph->sim->is_dead_m);
			return NULL;
		}
		pthread_mutex_unlock(&ph->sim->is_dead_m);
		pthread_mutex_lock(&ph->sim->last_meal_time_m);
		uint64_t last_meal = ph->last_meal_time;
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
			return NULL;
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
			return NULL;
		}

		if(go_sleep(ph) == -1)
			return NULL;

		think(ph);
	}

	return NULL;
}
