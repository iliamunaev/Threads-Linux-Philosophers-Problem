/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:12:49 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 19:43:39 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Prints a philosopher's action with timestamp synchronization.
 *
 * This function prints a formatted message indicating a philosopher's action,
 * such as "is eating," "is sleeping," or "has taken a fork." It ensures
 * synchronized output using a mutex to prevent concurrent printing issues.
 *
 * If the action is not "died," it first checks whether a philosopher has already
 * died to prevent unnecessary messages after termination.
 *
 * @param data A pointer to the program's shared data structure.
 * @param id The ID of the philosopher performing the action.
 * @param str A string describing the action.
 * @return void This function does not return a value.
 */
void	print_action(t_data *data, int id, char *str)
{
	if (ft_strcmp(str, "died") != 0)
	{
		pthread_mutex_lock(&data->mtx_global);
		if (data->died)
		{
			pthread_mutex_unlock(&data->mtx_global);
			return ;
		}
		pthread_mutex_unlock(&data->mtx_global);
	}
	pthread_mutex_lock(&data->mtx_print);
	printf("%ld %d %s\n", get_time() - data->start_time, id + 1, str);
	pthread_mutex_unlock(&data->mtx_print);
}
