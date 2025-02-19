#include "philo.h"

static int	perform_cycle(t_ph *ph)
{
	uint64_t	last_meal;

	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	last_meal = ph->last_meal_time;
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);
	if (check_philosopher_death(ph, last_meal) == -1)
		return (-1);
	if (eat(ph) == -1)
		return (-1);
	if (go_sleep(ph) == -1)
		return (-1);
	think(ph);
	return (0);
}

void	*philosopher(void *arg)
{
	t_ph	*ph;

	ph = (t_ph *)arg;
	while (true)
	{
		pthread_mutex_lock(&ph->sim->mtx_is_dead);
		if (ph->sim->all_dead || ph->is_dead)
		{
			pthread_mutex_unlock(&ph->sim->mtx_is_dead);
			return (NULL);
		}
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
		if (perform_cycle(ph) == -1)
			return (NULL);
	}
	return (NULL);
}
