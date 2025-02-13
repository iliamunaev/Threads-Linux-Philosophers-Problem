#include "philo.h"

// static void	free_sim(t_sim *sim)
// {
// 	if (!sim)
// 		return;
// 	if (sim->forks_m)
// 		free(sim->forks_m);
// 	if (sim->ph_threads)
// 		free(sim->ph_threads);
// 	free(sim);
// }

static void	free_init_fail(t_sim *sim)
{
	free(sim->ph_threads);
	sim->ph_threads = NULL;
	free(sim->forks_m);
	sim->forks_m = NULL;
	free(sim);
}

static t_sim	*alloc_sim_struct(char **av)
{
	t_sim	*sim;

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
	return (sim);
}

static int	init_main_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->is_dead_m, NULL) != 0 ||
		pthread_mutex_init(&sim->log_m, NULL) != 0 ||
		pthread_mutex_init(&sim->last_meal_time_m, NULL) != 0 ||
		pthread_mutex_init(&sim->number_must_eat_m, NULL) != 0)
	{
		print_err("Error: init_sim() could not init a main mutex.");
		return (-1);
	}
	return (0);
}

static int	init_fork_mutexes(t_sim *sim)
{
	long	i;

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
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	alloc_forks_and_threads(t_sim *sim)
{
	sim->forks_m = malloc(sim->ph_count * sizeof(pthread_mutex_t));
	if (!sim->forks_m)
	{
		print_err("Error: malloc of forks_m failed.");
		return (-1);
	}
	sim->ph_threads = malloc(sim->ph_count * sizeof(pthread_t));
	if (!sim->ph_threads)
	{
		free(sim->forks_m);
		sim->forks_m = NULL;
		print_err("Error: malloc of ph_threads failed.");
		return (-1);
	}
	return (0);
}

t_sim	*init_sim(char **av)
{
	t_sim *sim;

	sim = alloc_sim_struct(av);
	if (!sim)
		return (NULL);
	if (alloc_forks_and_threads(sim) < 0)
	{
		free(sim);
		return (NULL);
	}
	if (init_main_mutexes(sim) < 0)
	{
		free_init_fail(sim);
		return (NULL);
	}
	if (init_fork_mutexes(sim) < 0)
	{
		pthread_mutex_destroy(&sim->is_dead_m);
		pthread_mutex_destroy(&sim->log_m);
		pthread_mutex_destroy(&sim->last_meal_time_m);
		pthread_mutex_destroy(&sim->number_must_eat_m);
		free_init_fail(sim);
		return (NULL);
	}
	return (sim);
}
