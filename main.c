#include "philo.h"

/*
** Cleanup function: frees allocated memory.
*/
static void free_all(t_data *data)
{

	if (data->forks)
		free(data->forks);
	if (data->philo)
		free(data->philo);
}

int main(int argc, char **argv)
{
	t_data	data;
	int		i;

	/*
	** 1) Parse arguments into data structure
	*/
	if (parse_args(argc, argv, &data) == 1)
		return (1);

	/*
	** 2) Initialize everything (mutexes, arrays, etc.)
	*/
	if (init_all(&data) == 1)
	{
		free_all(&data);
		return (1);
	}

	/*
	** 3) Launch philosopher threads
	*/
	data.start_time = get_time();
	i = 0;
	while (i < data.nb_philo)
	{
		data.philo[i].last_meal = data.start_time;
		if (pthread_create(&data.philo[i].thread_id, NULL,
				&routine, &data.philo[i]) != 0)
		{
			free_all(&data);
			return (1);
		}
		i++;
	}

	/*
	** 4) Monitor for any deaths or for the "must eat" completion
	*/
	check_dead(&data);

	/*
	** 5) Join all threads
	*/
	i = 0;
	while (i < data.nb_philo)
	{
		pthread_join(data.philo[i].thread_id, NULL);
		i++;
	}
	// statrt test
	int j = 0;
	while (j < data.nb_philo)
	{
		printf("Philosopher %d ate %d times\n", j + 1, data.philo[j].eat_count);
		j++;
	}

	// end test

	/*
	** 6) Destroy all mutexes, free memory
	*/
	i = 0;
	while (i < data.nb_philo)
		pthread_mutex_destroy(&data.forks[i++]);
	pthread_mutex_destroy(&data.print_mutex);
	pthread_mutex_destroy(&data.meal_check_mutex);
	free_all(&data);
	return (0);
}



