#include "philo.h"

t_sim *init_sim(char **av)
{
    t_sim *sim = malloc(sizeof(t_sim));
    if (!sim)
    {
        print_err("Error: malloc of t_sim failed.");
        return (NULL);
    }
    sim->ph_count = ft_atol(av[1]);
    sim->all_dead = false;

    sim->forks_m = NULL;
    sim->ph_threads = NULL;

    /* 1) Allocate forks_m  */
    sim->forks_m = malloc(sim->ph_count * sizeof(pthread_mutex_t));
    if (!sim->forks_m)
    {
        free(sim);
        print_err("Error: malloc of forks_m failed.");
        return (NULL);
    }

    /* 2) Allocate ph_threads  */
    sim->ph_threads = malloc(sim->ph_count * sizeof(pthread_t));
    if (!sim->ph_threads)
    {
        free(sim->forks_m);
        free(sim);
        print_err("Error: malloc of ph_threads failed.");
        return (NULL);
    }

    /* 3) Initialize the main mutexes */
    if (pthread_mutex_init(&sim->is_dead_m, NULL) != 0 ||
        pthread_mutex_init(&sim->log_m, NULL) != 0 ||
        pthread_mutex_init(&sim->last_meal_time_m, NULL) != 0 ||
        pthread_mutex_init(&sim->number_must_eat_m, NULL) != 0)
    {
        print_err("Error: init_sim() could not init a main mutex.");
        goto fail_main_mutex;
    }

    /* 4) Initialize each fork mutex  */
    for (long i = 0; i < sim->ph_count; i++)
    {
        if (pthread_mutex_init(&sim->forks_m[i], NULL) != 0)
        {
            print_err("Error: init_sim() could not init fork mutex.");
            /* Destroy any fork mutexes that were successfully created before i */
            while (--i >= 0)
                pthread_mutex_destroy(&sim->forks_m[i]);
            goto fail_main_mutex;
        }
    }
    return sim;

fail_main_mutex:
    /* Destroy the main mutexes that might be initialized */
    pthread_mutex_destroy(&sim->is_dead_m);
    pthread_mutex_destroy(&sim->log_m);
    pthread_mutex_destroy(&sim->last_meal_time_m);
    pthread_mutex_destroy(&sim->number_must_eat_m);

    free(sim->ph_threads);
    free(sim->forks_m);
    free(sim);
    return NULL;
}

t_ph *init_ph(t_sim *sim, char **av)
{
    long ph_count = sim->ph_count;
    t_ph *ph = malloc(ph_count * sizeof(t_ph));
    if (!ph)
    {
        print_err("Error: init_ph() malloc failed.");
        return NULL;
    }

    long i = 0;
    while (i < ph_count)
    {
        ph[i].sim = sim;
        ph[i].index = i;
        ph[i].last_meal_time = get_timestamp();
        ph[i].time_to_die = ft_atol(av[2]);
        ph[i].time_to_eat = ft_atol(av[3]);
        ph[i].time_to_sleep = ft_atol(av[4]);
        if (av[5])
            ph[i].number_must_eat = ft_atol(av[5]);
        else
            ph[i].number_must_eat = -1;
        ph[i].is_dead = false;
        i++;
    }
    return ph;
}

/*
** Destroy all mutexes and free memory.
*/
void cleanup(t_sim *sim, t_ph *ph)
{
    if (!sim)
        return;

    /* Destroy main mutexes */
    pthread_mutex_destroy(&sim->is_dead_m);
    pthread_mutex_destroy(&sim->log_m);
    pthread_mutex_destroy(&sim->last_meal_time_m);
    pthread_mutex_destroy(&sim->number_must_eat_m);

    /* Destroy fork mutexes */
    for (long i = 0; i < sim->ph_count; i++)
        pthread_mutex_destroy(&sim->forks_m[i]);

    /* Free arrays */
    if (sim->forks_m)
        free(sim->forks_m);
    if (sim->ph_threads)
        free(sim->ph_threads);

    /* Free ph (array of philosopher structs) */
    if (ph)
        free(ph);

    /* Finally free the sim struct itself */
    free(sim);
}
