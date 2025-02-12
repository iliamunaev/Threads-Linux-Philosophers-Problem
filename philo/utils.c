/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 12:28:06 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/12 13:53:32 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_timestamp(void)
{
    struct timeval	tv;
	int	result;
	long	time;
	
    result = gettimeofday(&tv, NULL);
	if(result != 0)
		return (-1);
		
	// Convert to milliseconds
	time = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (time);  
}

void print_state(size_t i, const char *msg)
{
    printf("%ld %zu %s\n", get_timestamp(), i, msg);
}

size_t left(t_ph *ph)
{
	size_t	index;

	index = (ph->index + ph->ph_count - 1) % ph->ph_count;
	return (index);
}

size_t right(t_ph *ph)
{
	size_t	index;
	
	index = (ph->index + 1) % ph->ph_count;
	return (index);
}
