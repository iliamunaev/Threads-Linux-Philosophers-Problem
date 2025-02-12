#ifndef PHILO_H
# define PHILO_H

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>


typedef enum e_state
{
	thinking,
	eating,
	sleeping,

} t_state;

typedef struct s_ph
{
    size_t          index;         // Philosopher index (not a pointer)
    pthread_t       tid;           // Thread ID for each philosopher
    t_state         state;         // Philosopher state

    pthread_mutex_t *fork_l;       // Pointer to left fork
    pthread_mutex_t *fork_r;       // Pointer to right fork

    uint64_t        last_meal_time; // Use uint64_t for timestamps
    long            time_to_die;
    long            time_to_eat;
    long            time_to_sleep;
    volatile bool   is_dead;        // Ensure correct behavior in threads

    struct s_sim    *sim;          // Pointer to global simulation struct
} t_ph;

typedef struct s_sim
{
    size_t          ph_count;      // Number of philosophers
    t_ph           *ph;            // Array of philosopher structures
    pthread_mutex_t log_mutex;     // For print statements
    pthread_mutex_t *forks;        // Array of mutexes for forks
} t_sim;


bool	is_input_valid(int ac, char **av);


#endif // PHILO_H