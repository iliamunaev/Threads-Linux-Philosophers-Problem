/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:21:59 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/12 14:00:49 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void think(t_ph *ph)
{
	ph->state = thinking;	
	print_state(ph->index + 1, "is thinking");
	
	if (is_free_forks(ph))
	{
		ph->last_meal_time = get_timestamp();
		take_forks(ph);
		eat(ph);
		put_forks(ph);
	}
	
}

void sleep(t_ph *ph)
{
	print_state(ph->index + 1, "is sleeping");	
	usleep(ph->time_to_sleep * 1000);
}

void eat(t_ph *ph)
{
	ph->state = eating;
	print_state(ph->index + 1, "is eating");
	usleep(ph->time_to_eat * 1000);
}

bool is_free_forks(t_ph *ph)
{
    size_t index_l = left(ph);
    size_t index_r = right(ph);
    t_ph *ph_l = &ph->sim->ph[index_l];
    t_ph *ph_r = &ph->sim->ph[index_r];
    return (ph_l->state != eating && ph_r->state != eating);
}

void take_forks(t_ph *ph)
{
    pthread_mutex_lock(ph->fork_l);
    pthread_mutex_lock(ph->fork_r);
}

void put_forks(t_ph *ph)
{
    pthread_mutex_unlock(ph->fork_l);
    pthread_mutex_unlock(ph->fork_r);
}

void *philosopher(void *arg)
{
    t_ph *ph = (t_ph *)arg;
    
    while (1)
    {
        pthread_mutex_lock(&ph->death_mutex);
        if (ph->is_dead || get_timestamp() - ph->last_meal_time >= ph->time_to_die)
        {
            ph->is_dead = true;
            pthread_mutex_unlock(&ph->death_mutex);
            log_action(ph, "has died");
            return NULL;
        }
        pthread_mutex_unlock(&ph->death_mutex);

        think(ph);

        if (is_free_forks(ph))
        {
            ph->last_meal_time = get_timestamp();
            take_forks(ph);
            eat(ph);
            put_forks(ph);
        }

        sleep(ph);
        usleep(1000); // Prevent CPU overuse ???
    }
    return ;
}


