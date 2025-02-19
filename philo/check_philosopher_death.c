#include "philo.h"

int	check_philosopher_death(t_ph *ph, uint64_t last_meal)
{
	if (get_timestamp() - last_meal >= (uint64_t)ph->time_to_die)
	{
		pthread_mutex_lock(&ph->sim->mtx_is_dead);
		if (!ph->sim->all_dead)
		{
			ph->sim->all_dead = true;
			ph->is_dead = true;
			log_action(ph, "died");
		}
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
		return (-1);
	}
	return (0);
}

int	check_death_during_sleep(t_ph *ph)
{
	uint64_t	time_since_meal;

	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	time_since_meal = get_timestamp() - ph->last_meal_time;
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);
	if (time_since_meal >= (uint64_t)ph->time_to_die)
	{
		pthread_mutex_lock(&ph->sim->mtx_is_dead);
		if (!ph->sim->all_dead && !ph->is_dead)
		{
			ph->sim->all_dead = true;
			ph->is_dead = true;
			pthread_mutex_unlock(&ph->sim->mtx_is_dead);
			log_action(ph, "died");
			return (-1);
		}
		pthread_mutex_unlock(&ph->sim->mtx_is_dead);
		return (-1);
	}
	return (0);
}
