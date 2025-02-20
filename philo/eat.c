#include "philo.h"

int	eat(t_ph *ph)
{
	uint64_t	start;
	uint64_t	finish;

	if (take_forks(ph) == -1)
		return (-1);

	log_action(ph, "is eating");

	start = get_timestamp();

	while (get_timestamp() - start < (uint64_t)ph->time_to_eat)
	{
		usleep(1000);
		if (check_death_during_action(ph) == -1)
		{
			put_forks(ph);
			return (-1);
		}
	}
	finish = get_timestamp();
	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	ph->last_meal_time = finish;
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);
	put_forks(ph);
	return (0);
}

