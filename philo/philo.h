#ifndef PHILO_H
# define PHILO_H

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>

typedef struct s_ph
{
	size_t			index;
	uint64_t		last_meal_time;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			number_must_eat;
	bool			is_dead;
	struct s_sim	*sim;
}	t_ph;

typedef struct s_sim
{
	long			ph_count;
	bool			all_dead;
	pthread_t		*ph_threads;
	pthread_t		supervisor;

	pthread_mutex_t	last_meal_time_m;
	pthread_mutex_t	time_to_die_m;
	pthread_mutex_t	time_to_eat_m;
	pthread_mutex_t	time_to_sleep_m;
	pthread_mutex_t	number_must_eat_m;	
	pthread_mutex_t	is_dead_m;
	pthread_mutex_t	log_m;
	pthread_mutex_t	*forks_m;


}	t_sim;


bool	is_input_valid(int ac, char **av);

void *philosopher(void *arg);

long	ft_atol(const char *str);
void	print_err(char *msg);
void log_action(t_ph *ph, const char *msg);
uint64_t get_timestamp(void);
size_t	left(t_ph *ph);
size_t	right(t_ph *ph);

#endif // PHILO_H
