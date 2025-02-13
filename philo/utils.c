/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:28:06 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/13 15:54:36 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t get_timestamp(void)
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL) != 0)
		return 0;
	return (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
}

void log_action(t_ph *ph, const char *msg)
{
	uint64_t time = get_timestamp();
	pthread_mutex_lock(&ph->sim->log_m);
	printf("%lu %zu %s\n", time, ph->index + 1, msg);
	pthread_mutex_unlock(&ph->sim->log_m);
}

size_t right(t_ph *ph)
{
	return ((ph->index + 1) % ph->sim->ph_count);
}


size_t	left(t_ph *ph)
{
	size_t	index;

	index = ph->index % ph->sim->ph_count;
	return (index);
}

void	print_err(char *msg)
{
	while(*msg)
	{
		write(STDERR_FILENO, msg, 1);
		msg++;
	}
	write(STDERR_FILENO, "\n", 1);
}
