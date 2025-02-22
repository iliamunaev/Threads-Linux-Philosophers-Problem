/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:21:08 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/22 10:11:30 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Initializes mutexes for philosopher synchronization.
 *
 * This function allocates memory for an array of mutexes representing forks
 * and initializes them. It also initializes global mutexes used for printing
 * and managing shared data access.
 *
 * @param data A pointer to the program's data structure.
 * @return int Returns EXIT_SUCCESS (0) if all mutexes are initialized successfully,
 *         otherwise returns EXIT_FAILURE (1) on failure.
 */
static int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->forks)
		return (EXIT_FAILURE);
	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (EXIT_FAILURE);
		i++;
	}
	if (pthread_mutex_init(&data->mtx_print, NULL) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&data->mtx_global, NULL) != 0)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&data->mtx_threads, NULL) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes philosopher structures.
 *
 * This function allocates memory for an array of philosophers and sets initial values
 * for each philosopher's ID, meal count, last meal timestamp, and a reference
 * to the shared program data.
 *
 * @param data A pointer to the program's data structure.
 * @return int Returns EXIT_SUCCESS (0) if successful, otherwise returns EXIT_FAILURE (1).
 */
static int	init_philos(t_data *data)
{
	int	i;

	data->philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philo)
		return (EXIT_FAILURE);
	i = 0;
	while (i < data->nb_philo)
	{
		data->philo[i].id = i;
		data->philo[i].eat_count = 0;
		data->philo[i].last_meal = 0;
		data->philo[i].data = data;
		i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Initializes all necessary data structures and resources.
 *
 * This function sets default values for the simulation state, initializes
 * mutexes, and sets up philosopher structures. If any initialization step fails,
 * the function returns an error.
 *
 * @param data A pointer to the program's data structure.
 * @return int Returns EXIT_SUCCESS (0) if successful, otherwise returns EXIT_FAILURE (1).
 */
int	init_all(t_data *data)
{
	data->died = 0;
	data->forks = NULL;
	data->philo = NULL;
	data->threads_created = 0;
	if (init_mutexes(data))
		return (EXIT_FAILURE);
	if (init_philos(data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
