/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:13:31 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/22 10:31:51 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/**
 * @brief Handles the process of taking forks for a philosopher.
 *
 * This function ensures that the philosopher takes two forks in a
 * specific order to avoid deadlocks. If there is only one philosopher,
 * they take a single fork and wait until they die (as per the classic
 * philosopher problem).
 *
 * @param philo A pointer to the philosopher structure.
 * @param first_fork The index of the first fork to pick up.
 * @param second_fork The index of the second fork to pick up.
 * @return int Returns 0 if successful, otherwise 1 if only one philosopher exists.
 */
static int	take_forks(t_philo *philo, int first_fork, int second_fork)
{
	t_data	*d;

	d = philo->data;
	if (d->nb_philo == 1)
	{
		pthread_mutex_lock(&d->forks[0]);
		print_action(d, 0, "has taken a fork");
		usleep(d->time_to_die * 1000);
		pthread_mutex_unlock(&d->forks[0]);
		return (1);
	}
	pthread_mutex_lock(&d->forks[first_fork]);
	print_action(d, philo->id, "has taken a fork 1"); // test
	pthread_mutex_lock(&d->forks[second_fork]);
	print_action(d, philo->id, "has taken a fork 2"); // test
	return (0);
}

/**
 * @brief Handles the eating process for a philosopher.
 *
 * This function allows a philosopher to take two forks (using `take_forks`),
 * update their last meal time, and increment their meal count. After eating,
 * the forks are released.
 *
 * @param philo A pointer to the philosopher structure.
 * @return void This function does not return a value.
 */
static void	eat(t_philo *philo)
{
	t_data	*d;
	int		first_fork;
	int		second_fork;

	d = philo->data;
	first_fork = philo->id;
	second_fork = (philo->id + 1) % d->nb_philo;
	if (philo->id % 2 == 0)
	{
		first_fork = (philo->id + 1) % d->nb_philo;
		second_fork = philo->id;
	}
	if (take_forks(philo, first_fork, second_fork))
		return ;
	pthread_mutex_lock(&d->mtx_global);
	philo->last_meal = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&d->mtx_global);
	print_action(d, philo->id, "is eating");
	acting(d->time_to_eat);
	pthread_mutex_unlock(&d->forks[second_fork]);
	pthread_mutex_unlock(&d->forks[first_fork]);
}

/**
 * @brief Checks if any philosopher has died.
 *
 * This function safely reads the `died` flag from shared data using a mutex.
 * If a philosopher has died, it returns a nonzero value.
 *
 * @param d A pointer to the program's shared data structure.
 * @return int Returns 1 if a philosopher has died, otherwise 0.
 */
static int	is_died(t_data *d)
{
	int	died;

	pthread_mutex_lock(&d->mtx_global);
	died = d->died;
	pthread_mutex_unlock(&d->mtx_global);
	return (died);
}

/**
 * @brief Checks if a philosopher has eaten the required number of times.
 *
 * This function verifies whether a philosopher has reached the mandatory meal
 * count set by the program. It ensures safe access to shared data using a mutex.
 *
 * @param philo A pointer to the philosopher structure.
 * @return int Returns 1 if the philosopher has eaten enough times, otherwise 0.
 */
static int	check_eat_count(t_philo *philo)
{
	int		done;
	t_data	*d;

	done = 0;
	d = philo->data;
	pthread_mutex_lock(&d->mtx_global);
	if (d->must_eat_count != -1 && philo->eat_count >= d->must_eat_count)
		done = 1;
	pthread_mutex_unlock(&d->mtx_global);
	return (done);
}

static int	all_philosophers_ate_enough(t_data *data)
{
	int	i;
	int	count;

	if (data->must_eat_count == -1)
		return (0);
	i = 0;
	count = 0;
	while (i < data->nb_philo)
	{
		// Locking if needed when reading shared data
		pthread_mutex_lock(&data->mtx_global);
		if (data->philo[i].eat_count >= data->must_eat_count)
			count++;
		pthread_mutex_unlock(&data->mtx_global);
		i++;
	}
	return (count == data->nb_philo);
}
/**
 * @brief Monitors philosophers to check if any have died.
 *
 * This function continuously checks the time since each philosopher's last meal
 * and compares it to the `time_to_die` threshold. If a philosopher has exceeded
 * this time, the simulation ends, and a death message is printed.
 *
 * @param data A pointer to the program's shared data structure.
 * @return void This function does not return a value.
 */
void	check_dead(t_data *data)
{
	int	i;

	while (1)
	{
		if (all_philosophers_ate_enough(data))
			return ;
		pthread_mutex_lock(&data->mtx_global);
		if (data->died)
		{
			pthread_mutex_unlock(&data->mtx_global);
			break ;
		}
		i = 0;
		while (i < data->nb_philo)
		{
			if ((get_time() - data->philo[i].last_meal) > data->time_to_die)
			{
				data->died = 1;
				pthread_mutex_unlock(&data->mtx_global);
				print_action(data, i, "died");
				// if (data->nb_philo == 1)
				// 	pthread_mutex_unlock(&data->forks[0]);
				return ;
			}
			i++;
		}
		pthread_mutex_unlock(&data->mtx_global);
		usleep(100);
	}
}

/**
 * @brief The routine executed by each philosopher thread.
 *
 * Each philosopher alternates between thinking, eating, and sleeping.
 * The function ensures philosophers do not start at the same time to prevent
 * synchronization issues. It checks for death conditions and meal limits
 * in each iteration.
 *
 * @param void_philo A pointer to the philosopher structure cast as void.
 * @return void* Always returns NULL when the routine ends.
 */
void	*routine(void *void_philo)
{
	t_philo	*philo;
	t_data	*d;

	philo = (t_philo *)void_philo;
	d = philo->data;

	while (1)
	{
		pthread_mutex_lock(&d->mtx_threads);
		int ready = d->threads_created;
		pthread_mutex_unlock(&d->mtx_threads);
		if (ready)
			break;
		usleep(1000);
	}
	pthread_mutex_lock(&d->mtx_global);
    philo->last_meal = d->start_time;
    pthread_mutex_unlock(&d->mtx_global);

	print_action(d, philo->id, "is thinking");
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (is_died(d))
			break ;
		eat(philo);
		if (check_eat_count(philo))
			break ;
		print_action(d, philo->id, "is sleeping");
		acting(d->time_to_sleep);
		print_action(d, philo->id, "is thinking");
		usleep(1000);
	}

	return (NULL);
}
