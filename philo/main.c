
#include "philo.h"

t_sim	*init_sim(char **av)
{
	t_sim	*sim;

	sim = malloc(sizeof(t_sim));
	if (!sim)
	{
		print_err("Error: init_sim() malloc sim struct failed.");
		return (NULL);
	}
	sim->ph_count = ft_atol(av[1]);
	sim->all_dead = false;
	sim->ph_threads = malloc(sim->ph_count * sizeof(pthread_t));
	if (!sim->ph_threads)
	{
		for (long i = 0; i < sim->ph_count; i++)
			pthread_mutex_destroy(&sim->forks_m[i]);

		free(sim->ph_threads);
		free(sim->forks_m);
		free(sim);

		print_err("Error: init_sim() malloc ph_threads failed.");
		return (NULL);
	}
	pthread_mutex_init(&sim->is_dead_m, NULL);
	pthread_mutex_init(&sim->log_m, NULL);

	pthread_mutex_init(&sim->last_meal_time_m, NULL);
	pthread_mutex_init(&sim->time_to_die_m, NULL);
	pthread_mutex_init(&sim->time_to_eat_m, NULL);
	pthread_mutex_init(&sim->time_to_sleep_m, NULL);
	pthread_mutex_init(&sim->number_must_eat_m, NULL);



	sim->forks_m= malloc(sim->ph_count * sizeof(pthread_mutex_t));
	if (!sim->forks_m)
	{
		free (sim->ph_threads);
		free (sim);
		print_err("Error: init_sim() malloc ph_threads failed.");
		return (NULL);
	}
	for (long i = 0; i < sim->ph_count; i++)
    {
        if (pthread_mutex_init(&sim->forks_m[i], NULL) != 0)
        {
            print_err("Error: Failed to initialize fork mutex.");
            return NULL;
        }
    }
	return (sim);
}
t_ph *init_ph(t_sim *sim, char **av)
{
	long	ph_count;
	t_ph	*ph;
	long	i;

	ph_count = ft_atol(av[1]);
	ph = malloc(ph_count * sizeof(t_ph));
	if (!ph)
	{
		print_err("Error: init_ph() malloc failed.");
		return NULL;
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
		ph[i].number_must_eat = av[5] ? ft_atol(av[5]) : -1;
		ph[i].is_dead = false;
		i++;
	}
	return (ph);
}

void cleanup(t_sim *sim, t_ph *ph)
{
    if (!sim)
        return;

    // Destroy mutexes
    pthread_mutex_destroy(&sim->is_dead_m);
    pthread_mutex_destroy(&sim->log_m);
    pthread_mutex_destroy(&sim->last_meal_time_m);
    pthread_mutex_destroy(&sim->time_to_die_m);
    pthread_mutex_destroy(&sim->time_to_eat_m);
    pthread_mutex_destroy(&sim->time_to_sleep_m);
    pthread_mutex_destroy(&sim->number_must_eat_m);

    for (long i = 0; i < sim->ph_count; i++)
        pthread_mutex_destroy(&sim->forks_m[i]);

    // Free dynamically allocated memory
    if (sim->forks_m)
        free(sim->forks_m);
    if (sim->ph_threads)
        free(sim->ph_threads);
    if (ph)
        free(ph);
    free(sim);
}

int main(int ac, char **av)
{
	(void)ac;
	// if (!is_input_valid(ac, av))
	// 	return (EXIT_FAILURE);

	t_sim	*sim;
	t_ph	*ph;

	sim = init_sim(av);
	if(!sim)
	{
		print_err("Error: main() init sim failed.");
		return (EXIT_FAILURE);
	}
	ph = init_ph(sim, av);
	if (!ph)
	{
		free(sim->ph_threads);
		free(sim->forks_m);
		free(sim);
		print_err("Error: main() init ph failed.");
		return (EXIT_FAILURE);
	}

	long i = 0;
	while (i < sim->ph_count)
	{
		ph[i].index = i;
		if (pthread_create(&sim->ph_threads[i], NULL, &philosopher, &ph[i]) != 0)
		{
			print_err("Error: main() pthread_create philosopher failed.");
			// free all
			return (EXIT_FAILURE);
		}
		i++;
	}
	i = 0;
	while (i < sim->ph_count)
	{
		if (pthread_join(sim->ph_threads[i], NULL) != 0)
		{
			print_err("Error: main() pthread_join failed.");
			return (EXIT_FAILURE);
		}
		i++;
	}

	cleanup(sim, ph);
	return (EXIT_SUCCESS);
}

