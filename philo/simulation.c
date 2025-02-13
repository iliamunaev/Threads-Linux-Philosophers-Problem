
#include "philo.h"

void think(t_ph *ph)
{
	log_action(ph, "is thinking");
	usleep(1000);
}

int take_forks(t_ph *ph)
{
    if (ph->sim->ph_count == 1)
    {
        pthread_mutex_lock(&ph->sim->forks_m[left(ph)]);
        log_action(ph, "has taken a fork (but cannot eat)");

        // Simulate starvation by waiting until they die
        usleep(ph->time_to_die * 1000);
        pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
        return -1;
    }

    if (ph->index % 2 == 0)
    {
        pthread_mutex_lock(&ph->sim->forks_m[left(ph)]);
        if (pthread_mutex_lock(&ph->sim->forks_m[right(ph)]) != 0)  // 🔥 Check second lock
        {
            pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);  // ✅ Release first lock
            return -1;
        }
    }
    else
    {
        usleep(500);
        pthread_mutex_lock(&ph->sim->forks_m[right(ph)]);
        if (pthread_mutex_lock(&ph->sim->forks_m[left(ph)]) != 0)  // 🔥 Check second lock
        {
            pthread_mutex_unlock(&ph->sim->forks_m[right(ph)]);  // ✅ Release first lock
            return -1;
        }
    }
    return 0;
}



void put_forks(t_ph *ph)
{
	if (ph->sim->ph_count == 1)
	{
		// Only one fork was taken, so only unlock one
		pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
		return;
	}
	pthread_mutex_unlock(&ph->sim->forks_m[left(ph)]);
	pthread_mutex_unlock(&ph->sim->forks_m[right(ph)]);
}
void eat(t_ph *ph)
{
    int forks = take_forks(ph);

    if (forks != -1)
    {
        log_action(ph, "is eating");

        pthread_mutex_lock(&ph->sim->is_dead_m);
        ph->last_meal_time = get_timestamp();
        pthread_mutex_unlock(&ph->sim->is_dead_m);

        usleep(ph->time_to_eat * 1000);

        // ✅ Ensure forks are released after eating
        put_forks(ph);
    }
}

void go_sleep(t_ph *ph)
{
	log_action(ph, "is sleeping");
	usleep(ph->time_to_sleep * 1000);
}
void *philosopher(void *arg)
{
    t_ph *ph = (t_ph *)arg;
    if (!ph || !ph->sim)
        return NULL;

    while (1)
    {
        // ✅ Check if the philosopher is dead
        pthread_mutex_lock(&ph->sim->is_dead_m);
        if (ph->is_dead || ph->sim->all_dead)
        {
            pthread_mutex_unlock(&ph->sim->is_dead_m);
            break;
        }
        pthread_mutex_unlock(&ph->sim->is_dead_m);

        // ✅ Check starvation condition
        pthread_mutex_lock(&ph->sim->last_meal_time_m);
        uint64_t last_meal = ph->last_meal_time;
        pthread_mutex_unlock(&ph->sim->last_meal_time_m);

        pthread_mutex_lock(&ph->sim->time_to_die_m);
        long time_to_die = ph->time_to_die;
        pthread_mutex_unlock(&ph->sim->time_to_die_m);

        if ((get_timestamp() - last_meal) >= (uint64_t)time_to_die)
        {
            pthread_mutex_lock(&ph->sim->is_dead_m);
            ph->is_dead = true;
            ph->sim->all_dead = true;
            pthread_mutex_unlock(&ph->sim->is_dead_m);

            log_action(ph, "died");
            return NULL;
        }

        eat(ph);
        go_sleep(ph);
        think(ph);
    }

    pthread_mutex_lock(&ph->sim->is_dead_m);
    ph->is_dead = true;
    ph->sim->all_dead = true;
    pthread_mutex_unlock(&ph->sim->is_dead_m);

    log_action(ph, "died");
    return NULL;
}
