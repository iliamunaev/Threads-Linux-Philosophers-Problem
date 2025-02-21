/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:17:56 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 19:35:44 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Creates threads for all philosophers.
 *
 * This function initializes and starts a thread for each philosopher.
 * Each thread runs the `routine` function, and its start time is recorded.
 * If thread creation fails, all allocated resources are freed before
 * returning a failure code.
 *
 * @param data A pointer to the program's data structure.
 * @return int Returns EXIT_SUCCESS (0) if all threads are created successfully,
 *         otherwise returns EXIT_FAILURE (1).
 */
static int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		data->philo[i].last_meal = data->start_time;
		if (pthread_create(&data->philo[i].thread_id, NULL,
				&routine, &data->philo[i]) != 0)
		{
			free_all(data);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Waits for all philosopher threads to complete execution.
 *
 * This function joins all philosopher threads, ensuring that the main
 * thread waits for them to finish before proceeding.
 *
 * @param data A pointer to the program's data structure.
 * @return void This function does not return a value.
 */
static void	wait_for_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
		pthread_join(data->philo[i++].thread_id, NULL);
}

// test
static void	print_stats(t_data *data)
{
	int j;

	j = 0;
	while (j < data->nb_philo)
	{
		printf("Philosopher %d ate %d times\n",
			   j + 1, data->philo[j].eat_count);
		j++;
	}
}
// end test

/**
 * @brief Destroys all mutexes used in the program.
 *
 * This function releases mutex resources associated with philosopher forks,
 * as well as global mutexes used for synchronization.
 *
 * @param data A pointer to the program's data structure.
 * @return void This function does not return a value.
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
 * @brief Entry point for the philosopher simulation.
 *
 * This function initializes program data, creates philosopher threads,
 * and continuously checks for simulation conditions such as philosopher death.
 * After execution, it ensures that all threads complete and resources are freed.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of argument strings.
 * @return int Returns EXIT_SUCCESS (0) if the simulation runs successfully,
 *         otherwise returns EXIT_FAILURE (1) in case of errors.
 */
int	main(int argc, char **argv)
{
	t_data	data;

	if (parse_and_init(&data, argc, argv))
		return (EXIT_FAILURE);
	if (create_threads(&data))
		return (EXIT_FAILURE);
	check_dead(&data);
	wait_for_threads(&data);
	print_stats(&data); // test
	destroy_mutexes(&data);
	free_all(&data);
	return (EXIT_SUCCESS);
}
