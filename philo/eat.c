#include "philo.h"

int	eat(t_ph *ph)
{
	if (take_forks(ph) == -1)
		return (-1);
	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	ph->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);
	log_action(ph, "is eating");
	usleep(ph->time_to_eat * 1000);
	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	ph->last_meal_time = get_timestamp();
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);
	put_forks(ph);
	return (0);
}
