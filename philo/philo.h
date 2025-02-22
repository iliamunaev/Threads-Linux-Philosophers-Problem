/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:16:41 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/22 10:10:52 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;          // Philosopher ID (0-based internally)
	int				eat_count;   // How many times this philosopher has eaten
	long			last_meal;   // Timestamp (ms) of last meal
	pthread_t		thread_id;   // Thread of this philosopher
	struct s_data	*data;       // Pointer back to the shared simulation data
}	t_philo;

typedef struct s_data
{
	int				nb_philo;        // Number of philosophers
	long			time_to_die;     // ms before a philosopher dies if not eating
	long			time_to_eat;     // ms a philosopher spends eating
	long			time_to_sleep;   // ms a philosopher spends sleeping
	int				must_eat_count;  // If >= 0, how many times each philosopher must eat
	int				died;            // Flag to indicate someone died or sim should stop
	long			start_time;      // Timestamp when the simulation started
	int				threads_created;
	pthread_mutex_t	*forks;          // Array of mutexes, one per fork
	pthread_mutex_t	mtx_print;     // Mutex for printing
	pthread_mutex_t	mtx_global;// Mutex for checking/updating last_meal and eat_count
	pthread_mutex_t mtx_threads; 
	t_philo			*philo;          // Array of philosopher structs
}	t_data;

// Parsing & Init
int		parse_args(int argc, char **argv, t_data *data);
int		parse_and_init(t_data *data, int argc, char **argv);
int		init_all(t_data *data);

// Time & Sleep
long	get_time(void);
void	acting(long time_in_ms);

// Routine & Threads
void	*routine(void *void_philo);
void	check_dead(t_data *data);

// Utils
void	print_action(t_data *data, int id, char *str);
void	free_all(t_data *data);
int		ft_strcmp(char *s1, char *s2);

#endif
