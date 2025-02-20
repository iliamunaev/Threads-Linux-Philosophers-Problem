#include "philo.h"

static int	perform_cycle(t_ph *ph)
{
	uint64_t	last_meal;

	// ✅ Immediately check if simulation has stopped
	pthread_mutex_lock(&ph->sim->mtx_is_dead);
	if (ph->sim->all_dead)
	{
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
		printf("[DEBUG] Philosopher %ld detected simulation stop. Exiting.\n", ph->index + 1);
		return (-1);
	}
	pthread_mutex_unlock(&ph->sim->mtx_is_dead);

	// ✅ Check if this philosopher has already died
	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	last_meal = ph->last_meal_time;
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);

	if (check_philosopher_death(ph, last_meal) == -1)
	{
		printf("[DEBUG] Philosopher %ld detected death.\n", ph->index + 1);
		return (-1);
	}

	// ✅ Before taking forks, check if we should exit
	pthread_mutex_lock(&ph->sim->mtx_is_dead);
	if (ph->sim->all_dead)
	{
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
		printf("[DEBUG] Philosopher %ld stopping before taking forks.\n", ph->index + 1);
		return (-1);
	}
	pthread_mutex_unlock(&ph->sim->mtx_is_dead);

	if (eat(ph) == -1)
	{
		printf("[DEBUG] Philosopher %ld failed to eat.\n", ph->index + 1);
		return (-1);
	}

	if (go_sleep(ph) == -1)
	{
		printf("[DEBUG] Philosopher %ld failed during sleep.\n", ph->index + 1);
		return (-1);
	}

	think(ph);
	return (0);
}

void	*philo(void *arg)
{
	t_ph	*ph = (t_ph *)arg;
	int		is_dead;

	while (true)
	{
		//  Stop immediately if simulation has ended
		pthread_mutex_lock(&ph->sim->mtx_is_dead);
		is_dead = ph->sim->all_dead || ph->is_dead;
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);

		if (is_dead)
		{
			printf("[DEBUG] Philosopher %ld detected simulation stop. Exiting.\n", ph->index + 1);
			return (NULL);
		}

		// Check if philosopher has eaten enough
		pthread_mutex_lock(&ph->sim->mtx_meal_count);
		if (ph->sim->num_meals_to_eat > 0 && ph->sim->meal_count[ph->index] >= ph->sim->num_meals_to_eat)
		{
			pthread_mutex_unlock(&ph->sim->mtx_meal_count);
			printf("[DEBUG] Philosopher %ld is full. Stopping.\n", ph->index + 1);
			return (NULL);
		}
		pthread_mutex_unlock(&ph->sim->mtx_meal_count);

		// ✅ Ensure cycle exits immediately if needed
		if (perform_cycle(ph) == -1)
		{
			printf("[DEBUG] Philosopher %ld exiting due to cycle failure.\n", ph->index + 1);
			return (NULL);
		}
	}
	return (NULL);
}
