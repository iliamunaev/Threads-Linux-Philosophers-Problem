#include "philo.h"


void print_action(t_data *data, int id, char *str)
{
    int local_died;

    // 🔒 Lock global mutex before reading `died`
    // pthread_mutex_lock(&data->mtx_global);
    local_died = data->died;
    // pthread_mutex_unlock(&data->mtx_global);

    if (local_died)
        return;  // Stop printing if someone is dead

    pthread_mutex_lock(&data->mtx_print);
    long timestamp = get_time() - data->start_time;
    printf("%ld %d %s\n", timestamp, id + 1, str);
    pthread_mutex_unlock(&data->mtx_print);
}



/*
** Helper for a philosopher to pick up the two forks and eat.
*/
static void eat(t_philo *philo)
{
	t_data *d = philo->data;


	// The order of locking forks often helps avoid deadlocks:
	// to reduce jitter, we lock left first for odd IDs, right first for even, etc.
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&d->forks[philo->id]);
		print_action(d, philo->id, "has taken a fork");
		if (d->nb_philo > 1)
		{
			pthread_mutex_lock(&d->forks[(philo->id + 1) % d->nb_philo]);
			print_action(d, philo->id, "has taken a fork");
		}
	}
	else
	{
		pthread_mutex_lock(&d->forks[(philo->id + 1) % d->nb_philo]);
		print_action(d, philo->id, "has taken a fork");
		pthread_mutex_lock(&d->forks[philo->id]);
		print_action(d, philo->id, "has taken a fork");
	}

	// Eat
	pthread_mutex_lock(&d->mtx_global);
	print_action(d, philo->id, "is eating");
	philo->last_meal = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&d->mtx_global);

	my_sleep(d->time_to_eat);

	// Put forks down
	pthread_mutex_unlock(&d->forks[philo->id]);
	if (d->nb_philo > 1)
		pthread_mutex_unlock(&d->forks[(philo->id + 1) % d->nb_philo]);
}

void *routine(void *void_philo)
{
    t_philo *philo = (t_philo *)void_philo;
    t_data  *d = philo->data;

    print_action(d, philo->id, "is thinking");

    if (philo->id % 2 == 0)
        usleep(100);

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
        my_sleep(d->time_to_sleep);
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
        int finished = 1;
        while (i < data->nb_philo && !data->died)
        {
            long now = get_time();

            if ((now - data->philo[i].last_meal) > data->time_to_die)
            {
                // ✅ Lock before writing `died`
                data->died = 1;

                pthread_mutex_unlock(&data->mtx_global);
                print_action(data, i, "died");
                return;
            }

            if (data->must_eat_count != -1 && data->philo[i].eat_count < data->must_eat_count)
                finished = 0;

            i++;
        }

        if (!data->died && data->must_eat_count != -1 && finished)
            data->died = 1;  // ✅ Lock before writing `died`

        pthread_mutex_unlock(&data->mtx_global);
        usleep(1000);
    }
}
