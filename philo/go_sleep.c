#include "philo.h"

int go_sleep(t_ph *ph)
{
	uint64_t	start;

	log_action(ph, "is sleeping");
	start = get_timestamp();
	while (get_timestamp() - start < (uint64_t)ph->time_to_sleep)
	{
		usleep(500);
		if (check_death_during_action(ph) == -1)
			return (-1);
	}
	return (0);
}
