/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 00:04:08 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 09:14:09 by imunaev-         ###   ########.fr       */
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
	int				ph_count;
	bool			all_dead;
	bool			all_satiated;
	int				num_meals_to_eat;
	int				*meal_count;
	int             active_eaters;   // How many philosophers are currently eating
    pthread_mutex_t mtx_host;        // Protects active_eaters
	pthread_t		*ph_threads;
	pthread_t		monitor_thread;
	pthread_mutex_t	*mtx_forks;
	pthread_mutex_t	mtx_is_dead;
	pthread_mutex_t	mtx_log;
	pthread_mutex_t	mtx_last_meal_time;
	pthread_mutex_t	mtx_meal_count;
}	t_sim;

typedef struct s_ph
{
	size_t		index;
	uint64_t	last_meal_time;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	bool		is_dead;
	t_sim		*sim;
}	t_ph;

bool		is_input_valid(int ac, char **av);
void		print_err(char *msg);
void		*philo(void *arg);
void		think(t_ph *ph);
int			eat(t_ph *ph);
int			go_sleep(t_ph *ph);
uint64_t	get_timestamp(void);
void		log_action(t_ph *ph, const char *msg);
size_t		left(t_ph *ph);
size_t		right(t_ph *ph);
void		cleanup_exit(t_sim **sim, t_ph **ph, int status);
t_ph		*init_ph(t_sim *sim, char **av);
t_sim		*init_sim(char **av);
int			check_death_during_action(t_ph *ph);
int			check_philosopher_death(t_ph *ph, uint64_t last_meal);
int			go_sleep(t_ph *ph);
int			eat(t_ph *ph);
void		put_forks(t_ph *ph);
int			take_forks(t_ph *ph);
void		*superviser(void *arg);
int			ft_atoi(const char *str);
int			alloc_forks_and_threads(t_sim *sim);
int			init_main_mutexes(t_sim *sim);
int			init_fork_mutexes(t_sim *sim);

#endif // PHILO_H
