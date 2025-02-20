/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:20:32 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/20 23:43:11 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_err(char *msg)
{
	while (*msg)
		write(STDERR_FILENO, msg++, 1);
	write(STDERR_FILENO, "\n", 1);
}

uint64_t	get_timestamp(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return (((uint64_t)tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL));
}

void	log_action(t_ph *ph, const char *msg)
{
	uint64_t	time;

	time = get_timestamp();
	pthread_mutex_lock(&ph->sim->mtx_log);
	printf("%u %zu %s\n", (unsigned int)time, ph->index + 1, msg);
	pthread_mutex_unlock(&ph->sim->mtx_log);
}

size_t	left(t_ph *ph)
{
	return (ph->index);
}

size_t	right(t_ph *ph)
{
	return ((ph->index + 1) % ph->sim->ph_count);
}
