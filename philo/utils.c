/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:28:06 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/12 14:16:11 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint64_t	get_timestamp(void)
{
    struct timeval	tv;
	
    if (gettimeofday(&tv, NULL) != 0)
        return (0);
    return (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
}

void	print_state(t_ph *ph, const char *msg)
{
    uint64_t time;
    
	time = get_timestamp();
    pthread_mutex_lock(&ph->sim->log_mutex);
    printf("%llu %zu %s\n", time, ph->index + 1, msg);
    pthread_mutex_unlock(&ph->sim->log_mutex);
}

size_t	left(t_ph *ph)
{
	size_t	index;

	index = (ph->index + ph->sim->ph_count - 1) % ph->sim->ph_count;
	return (index);
}

size_t	right(t_ph *ph)
{
	size_t	index;
	
	index = (ph->index + 1) % ph->sim->ph_count;
	return (index);
}
