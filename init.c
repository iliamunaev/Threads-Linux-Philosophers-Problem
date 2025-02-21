#include "philo.h"

/*
** Initialize all the simulation data, create mutexes, etc.
*/
int init_all(t_data *data)
{
	int i;

	data->died = 0;
	data->forks = NULL;
	data->philo = NULL;

	// Allocate forks and initialize mutexes
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (1);
		i++;
	}

	// Initialize the printing and meal checking mutex
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->meal_check_mutex, NULL) != 0)
		return (1);

	// Allocate philosophers
	data->philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philo)
		return (1);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philo[i].id = i;
		data->philo[i].eat_count = 0;
		data->philo[i].last_meal = 0;
		data->philo[i].data = data;
		i++;
	}
	return (0);
}
