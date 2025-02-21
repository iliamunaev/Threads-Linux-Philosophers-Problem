#include "philo.h"
#include <string.h>  // For strcmp

void print_action(t_data *data, int id, char *str)
{
    // Allow the death message to be printed even if data->died is set.
    if (strcmp(str, "died") != 0)
    {
        pthread_mutex_lock(&data->mtx_global);
        if (data->died)
        {
            pthread_mutex_unlock(&data->mtx_global);
            return;
        }
        pthread_mutex_unlock(&data->mtx_global);
    }

    pthread_mutex_lock(&data->mtx_print);
    printf("%ld %d %s\n", get_time() - data->start_time, id + 1, str);
    pthread_mutex_unlock(&data->mtx_print);
}

static void eat(t_philo *philo)
{
    t_data *d = philo->data;

    // Determine fork order to avoid deadlocks
    int first_fork = philo->id;
    int second_fork = (philo->id + 1) % d->nb_philo;

    if (philo->id % 2 == 0) // Even philosophers reverse order
    {
        first_fork = (philo->id + 1) % d->nb_philo;
        second_fork = philo->id;
    }

    pthread_mutex_lock(&d->forks[first_fork]);
    print_action(d, philo->id, "has taken a fork");

    pthread_mutex_lock(&d->forks[second_fork]);
    print_action(d, philo->id, "has taken a fork");

    // Update state under lock then print outside the lock.
    pthread_mutex_lock(&d->mtx_global);
    philo->last_meal = get_time();
    philo->eat_count++;
    pthread_mutex_unlock(&d->mtx_global);

    print_action(d, philo->id, "is eating");

    acting(d->time_to_eat);

    // Put down forks in the same order they were picked up.
    pthread_mutex_unlock(&d->forks[second_fork]);
    pthread_mutex_unlock(&d->forks[first_fork]);
}

void *routine(void *void_philo)
{
    t_philo *philo = (t_philo *)void_philo;
    t_data  *d = philo->data;

	print_action(d, philo->id, "is thinking");

    if (philo->id % 2 == 0)
        usleep(100); // Small delay for even philosophers

    while (1)
    {
        pthread_mutex_lock(&d->mtx_global);
        if (d->died)
        {
            pthread_mutex_unlock(&d->mtx_global);
            break;
        }
        pthread_mutex_unlock(&d->mtx_global);

        eat(philo);

        pthread_mutex_lock(&d->mtx_global);
        if (d->must_eat_count != -1 && philo->eat_count >= d->must_eat_count)
        {
            pthread_mutex_unlock(&d->mtx_global);
            break;
        }
        pthread_mutex_unlock(&d->mtx_global);

        print_action(d, philo->id, "is sleeping");
        acting(d->time_to_sleep);
        print_action(d, philo->id, "is thinking");
    }
    return NULL;
}

void check_dead(t_data *data)
{
    while (1)
    {
        pthread_mutex_lock(&data->mtx_global);
        if (data->died)
        {
            pthread_mutex_unlock(&data->mtx_global);
            break;
        }

        int i = 0;
        while (i < data->nb_philo)
        {
            if ((get_time() - data->philo[i].last_meal) > data->time_to_die)
            {
                data->died = 1;
                pthread_mutex_unlock(&data->mtx_global);
                print_action(data, i, "died");
                return;
            }
            i++;
        }
        pthread_mutex_unlock(&data->mtx_global);
        usleep(1000);
    }
}
