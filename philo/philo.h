#ifndef PHILO_H
# define PHILO_H

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>



typedef struct s_philo
{
	uint32_t		index;			// Philosopher index
	pthread_t		thread_id;		// Thread ID for each philosopher
	pthread_mutex_t	*fork_left;		// Pointer to left fork
	pthread_mutex_t	*fork_right;	// Pointer to right fork
	struct s_sim	*sim;			// Pointer to global simulation struct
} t_philo;

typedef struct s_sim
{
	uint32_t		philosopher_count;	// Number of philosophers
	t_philo			*philos;			// Array of philosopher structures
	pthread_mutex_t	log_mutex;			// for print statements
	pthread_mutex_t	*forks;				// Array of mutexes for forks
} t_sim;

bool	is_input_valid(int ac, char **av);


#endif // PHILO_H
