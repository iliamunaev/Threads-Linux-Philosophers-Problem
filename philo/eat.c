/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 23:15:30 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 09:54:27 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// static int	lock_forks_and_log(t_ph *ph)
// {
// 	if (take_forks(ph) == -1)
// 		return (-1);
// 	log_action(ph, "is eating");
// 	return (0);
// }

// static int	eat_loop(t_ph *ph, uint64_t start, uint64_t *finish)
// {
// 	while (get_timestamp() - start < (uint64_t)ph->time_to_eat)
// 	{
// 		usleep(1000);
// 		if (check_death_during_action(ph) == -1)
// 			return (-1);
// 	}
// 	*finish = get_timestamp();
// 	return (0);
// }

static void	update_meal_record(t_ph *ph, uint64_t finish)
{
	pthread_mutex_lock(&ph->sim->mtx_meal_count);
	if (ph->sim->meal_count)
		ph->sim->meal_count[ph->index]++;
	pthread_mutex_unlock(&ph->sim->mtx_meal_count);
	pthread_mutex_lock(&ph->sim->mtx_last_meal_time);
	ph->last_meal_time = finish;
	pthread_mutex_unlock(&ph->sim->mtx_last_meal_time);
}

#include <unistd.h> // for usleep

static void ask_host(t_ph *ph)
{
	log_action(ph, "is thinking");

    while (1)
    {
        pthread_mutex_lock(&ph->sim->mtx_host);
        if (ph->sim->active_eaters < ph->sim->ph_count - 1)
        {
            ph->sim->active_eaters++;
            pthread_mutex_unlock(&ph->sim->mtx_host);
            break;  // Allowed to proceed
        }
        pthread_mutex_unlock(&ph->sim->mtx_host);

        // Sleep a bit before re-checking to avoid busy loop
        usleep(1000);
    }
}
static void leave_host(t_sim *sim)
{
    pthread_mutex_lock(&sim->mtx_host);
    sim->active_eaters--;
    pthread_mutex_unlock(&sim->mtx_host);
}

int eat(t_ph *ph)
{
    uint64_t start, finish;

    // 1) Ask the host for permission to eat
	if (ph->sim->ph_count != 1)
		ask_host(ph);

    // 2) Now pick up forks (left then right, or odd/even logic)
    if (take_forks(ph) == -1)
    {
        leave_host(ph->sim);  // if taking forks fails, free slot
        return -1;
    }

    // 3) Eat for ph->time_to_eat
    log_action(ph, "is eating");
    start = get_timestamp();
    while (get_timestamp() - start < (uint64_t)ph->time_to_eat)
    {
        usleep(1000);
        if (check_death_during_action(ph) == -1)
        {
            put_forks(ph);
            leave_host(ph->sim);
            return -1;
        }
    }
    finish = get_timestamp();

    // 4) Update last_meal_time, meal_count, etc.
    update_meal_record(ph, finish);

    // 5) Put down forks
    put_forks(ph);

    // 6) Notify host that a philosopher slot is now free
    leave_host(ph->sim);

    return 0;
}
