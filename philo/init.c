#include "philo.h"
t_sim *init_sim(char **av)
{
    t_sim   *sim;
    long    i;

    sim = malloc(sizeof(t_sim));
    if (!sim)
    {
        print_err("Error: malloc of t_sim failed.");
        return (NULL);
    }
    sim->ph_count = ft_atol(av[1]);
    sim->all_dead = false;
    sim->forks_m = NULL;
    sim->ph_threads = NULL;
    sim->forks_m = malloc(sim->ph_count * sizeof(pthread_mutex_t));
    if (!sim->forks_m)
    {
        free(sim);
        print_err("Error: malloc of forks_m failed.");
        return (NULL);
    }
    sim->ph_threads = malloc(sim->ph_count * sizeof(pthread_t));
    if (!sim->ph_threads)
    {
        free(sim->forks_m);
        free(sim);
        print_err("Error: malloc of ph_threads failed.");
        return NULL;
    }
    if (pthread_mutex_init(&sim->is_dead_m, NULL) != 0 ||
        pthread_mutex_init(&sim->log_m, NULL) != 0 ||
        pthread_mutex_init(&sim->last_meal_time_m, NULL) != 0 ||
        pthread_mutex_init(&sim->number_must_eat_m, NULL) != 0)
    {
        print_err("Error: init_sim() could not init a main mutex.");

        free(sim->ph_threads);
        free(sim->forks_m);
        free(sim);
        return (NULL);
    }
    {
        i = 0;
        while (i < sim->ph_count)
        {
            if (pthread_mutex_init(&sim->forks_m[i], NULL) != 0)
            {
                print_err("Error: init_sim() could not init fork mutex.");
                i--;
                while (i >= 0)
                {
                    pthread_mutex_destroy(&sim->forks_m[i]);
                    i--;
                }
                pthread_mutex_destroy(&sim->is_dead_m);
                pthread_mutex_destroy(&sim->log_m);
                pthread_mutex_destroy(&sim->last_meal_time_m);
                pthread_mutex_destroy(&sim->number_must_eat_m);
                free(sim->ph_threads);
                free(sim->forks_m);
                free(sim);
                return (NULL);
            }
            i++;
        }
    }
    return (sim);
}


t_ph *init_ph(t_sim *sim, char **av)
{
    long    i;
    long ph_count;
    t_ph *ph;

    ph_count = sim->ph_count;
    ph = malloc(ph_count * sizeof(t_ph));
    if (!ph)
    {
        print_err("Error: init_ph() malloc failed.");
        return (NULL);
    }
    i = 0;
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
    return (ph);
}

void cleanup(t_sim *sim, t_ph *ph)
{
    long i;

    if (!sim)
        return ;
    pthread_mutex_destroy(&sim->is_dead_m);
    pthread_mutex_destroy(&sim->log_m);
    pthread_mutex_destroy(&sim->last_meal_time_m);
    pthread_mutex_destroy(&sim->number_must_eat_m);
    i = 0;
    while (i < sim->ph_count)
    {
        pthread_mutex_destroy(&sim->forks_m[i]);
        i++;
    }
    if (sim->forks_m)
        free(sim->forks_m);
    if (sim->ph_threads)
        free(sim->ph_threads);
    if (ph)
        free(ph);
    free(sim);
}
