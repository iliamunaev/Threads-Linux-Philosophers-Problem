/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:17:56 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/22 10:12:24 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Creates threads for all philosophers.
 *
 * Threads are created after the simulation start time and each philosopher’s
 * last_meal have been set to that same reference value.
 */
static int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&data->philo[i].thread_id, NULL,
				&routine, &data->philo[i]) != 0)
		{
			free_all(data);
			return (EXIT_FAILURE);
		}
		i++;
	}
	pthread_mutex_lock(&data->mtx_threads);
	data->threads_created = 1;
	pthread_mutex_unlock(&data->mtx_threads);
	return (EXIT_SUCCESS);
}

/**
 * @brief Waits for all philosopher threads to complete execution.
 */
static void	wait_for_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
		pthread_join(data->philo[i++].thread_id, NULL);
}

/**
 * @brief Prints the number of times each philosopher ate (for testing).
 */
static void	print_stats(t_data *data)
{
	int j;

	j = 0;
	while (j < data->nb_philo)
	{
		printf("Philosopher %d ate %d times\n", j + 1, data->philo[j].eat_count);
		j++;
	}
}

/**
 * @brief Destroys all mutexes used in the simulation.
 */
static void	destroy_mutexes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
		pthread_mutex_destroy(&data->forks[i++]);
	pthread_mutex_destroy(&data->mtx_print);
	pthread_mutex_destroy(&data->mtx_global);
}

/**
 * @brief Main entry point for the simulation.
 */
int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (parse_and_init(&data, argc, argv))
		return (EXIT_FAILURE);

	// Set simulation start time here before threads are created.
	data.start_time = get_time();

	// Initialize each philosopher's last_meal to the simulation start time.
	i = 0;
	while (i < data.nb_philo)
	{
		data.philo[i].last_meal = data.start_time;
		i++;
	}

	if (create_threads(&data))
		return (EXIT_FAILURE);

	// Optional: Give threads a brief moment to start running.
	usleep(1000);

	// Begin death-checking (this function should use mutexes when reading shared data).
	check_dead(&data);
	wait_for_threads(&data);
	print_stats(&data); // For testing purposes.
	destroy_mutexes(&data);
	free_all(&data);
	return (EXIT_SUCCESS);
}
