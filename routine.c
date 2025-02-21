#include "philo.h"

/*
** Printing function, wraps a mutex lock so that no two messages overlap.
*/
void print_action(t_data *data, int id, char *str)
{
	long timestamp;

	pthread_mutex_lock(&data->print_mutex);
	if (!data->died)
	{
		timestamp = get_time() - data->start_time;
		// Philosophers are 1-based in the output
		printf("%ld %d %s\n", timestamp, id + 1, str);
	}
	pthread_mutex_unlock(&data->print_mutex);
}

/*
** Helper for a philosopher to pick up the two forks and eat.
*/
static void eat(t_philo *philo)
{
	t_data *d = philo->data;

	// The order of locking forks often helps avoid deadlocks:
	// to reduce jitter, we lock left first for odd IDs, right first for even, etc.
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&d->forks[philo->id]);
		print_action(d, philo->id, "has taken a fork");
		if (d->nb_philo > 1)
		{
			pthread_mutex_lock(&d->forks[(philo->id + 1) % d->nb_philo]);
			print_action(d, philo->id, "has taken a fork");
		}
	}
	else
	{
		pthread_mutex_lock(&d->forks[(philo->id + 1) % d->nb_philo]);
		print_action(d, philo->id, "has taken a fork");
		pthread_mutex_lock(&d->forks[philo->id]);
		print_action(d, philo->id, "has taken a fork");
	}

	// Eat
	pthread_mutex_lock(&d->meal_check_mutex);
	print_action(d, philo->id, "is eating");
	philo->last_meal = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&d->meal_check_mutex);

	my_sleep(d->time_to_eat);

	// Put forks down
	pthread_mutex_unlock(&d->forks[philo->id]);
	if (d->nb_philo > 1)
		pthread_mutex_unlock(&d->forks[(philo->id + 1) % d->nb_philo]);
}

/*
** The main routine each philosopher thread runs.
*/
void *routine(void *void_philo)
{
	t_philo *philo = (t_philo *)void_philo;
	t_data  *d = philo->data;

	// Special case: if only 1 philosopher, he'll never get a second fork => dies
	if (d->nb_philo == 1)
	{
		pthread_mutex_lock(&d->forks[philo->id]);
		print_action(d, philo->id, "has taken a fork");
		my_sleep(d->time_to_die);
		print_action(d, philo->id, "died");
		pthread_mutex_unlock(&d->forks[philo->id]);
		d->died = 1;
		return (NULL);
	}
	
	print_action(d, philo->id, "is thinking");

	// Small offset for even philosopher to reduce forks collision at start
	if (philo->id % 2 == 0)
		usleep(100);

	while (!d->died)
	{
		// Eat
		eat(philo);
		if (d->must_eat_count != -1 && philo->eat_count >= d->must_eat_count)
			break ;

		// Sleep
		print_action(d, philo->id, "is sleeping");
		my_sleep(d->time_to_sleep);

		// Think
		print_action(d, philo->id, "is thinking");
	}
	return (NULL);
}

/*
** A loop in the main thread that regularly checks if someone died
** or if everyone finished eating enough times.
*/
void check_dead(t_data *data)
{
	while (!data->died)
	{
		int i = 0;
		while (i < data->nb_philo && !data->died)
		{
			pthread_mutex_lock(&data->meal_check_mutex);
			// If time since last meal exceeds time_to_die, he dies
			if ((get_time() - data->philo[i].last_meal) > data->time_to_die)
			{
				print_action(data, i, "died");
				data->died = 1;
			}
			pthread_mutex_unlock(&data->meal_check_mutex);

			// Check if everyone ate enough times
			if (data->must_eat_count != -1)
			{
				int j = 0;
				int finished = 1;
				while (j < data->nb_philo)
				{
					if (data->philo[j].eat_count < data->must_eat_count)
						finished = 0;
					j++;
				}
				if (finished)
					data->died = 1;
			}
			i++;
		}
		if (!data->died)
			usleep(1000);
	}
}
