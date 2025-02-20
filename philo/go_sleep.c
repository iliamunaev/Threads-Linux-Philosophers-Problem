#include "philo.h"

int	go_sleep(t_ph *ph)
{
	uint64_t	start;

	start = get_timestamp();
	log_action(ph, "is sleeping");
	while (get_timestamp() - start < (uint64_t)ph->time_to_sleep)
	{
		usleep(100);
		if (check_death_during_action(ph) == -1)
			return (-1);
	}
	return (0);
}
