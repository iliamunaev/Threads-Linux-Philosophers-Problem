/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:04:08 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/20 07:23:38 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <stdio.h>

typedef struct s_sim
{
	long			ph_count;
	bool			all_dead;
	pthread_t		*ph_threads;
	pthread_mutex_t	*mtx_forks;
	pthread_mutex_t	mtx_is_dead;
	pthread_mutex_t	mtx_log;
	pthread_mutex_t	mtx_last_meal_time;
}	t_sim;

typedef struct s_ph
{
	size_t	index;
	uint64_t	last_meal_time;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		num_meals_to_eat;
	bool		is_dead;
	t_sim		*sim;
}	t_ph;

bool		is_input_valid(int ac, char **av);
long		ft_atol(const char *str);
void		print_err(char *msg);
void		*philosopher(void *arg);
void		think(t_ph *ph);
int			eat(t_ph *ph);
int			go_sleep(t_ph *ph);
uint64_t	get_timestamp(void);
void		log_action(t_ph *ph, const char *msg);
size_t		left(t_ph *ph);
size_t		right(t_ph *ph);
void	cleanup_exit(t_sim **sim, t_ph **ph, int status);
t_ph		*init_ph(t_sim *sim, char **av);
t_sim		*init_sim(char **av);
int			check_death_during_action(t_ph *ph);
int			check_philosopher_death(t_ph *ph, uint64_t last_meal);
int			go_sleep(t_ph *ph);
int			eat(t_ph *ph);
void		put_forks(t_ph *ph);
int			take_forks(t_ph *ph);

#endif // PHILO_H
