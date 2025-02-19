#include "philo.h"

static void	destroy_mutexes(t_sim *sim)
{
	if (pthread_mutex_destroy(&sim->mtx_is_dead) != 0)
		return ;
	if (pthread_mutex_destroy(&sim->mtx_log) != 0)
		return ;
	if (pthread_mutex_destroy(&sim->mtx_last_meal_time) != 0)
		return ;
}
static void	destroy_forks(t_sim *sim)
{
	long	i;

	if (!sim->mtx_forks)
		return ;
	i = 0;
	while (i < sim->ph_count)
	{
		pthread_mutex_destroy(&sim->mtx_forks[i]);
		i++;
	}
	free(sim->mtx_forks);
}

static void	free_resources(t_sim **sim, t_ph **ph)
{
	if (*sim)
	{
		if ((*sim)->ph_threads)
			free((*sim)->ph_threads);
		free(*sim);
		*sim = NULL;
	}
	if (*ph)
	{
		free(*ph);
		*ph = NULL;
	}
}

void	cleanup_exit(t_sim **sim, t_ph **ph, int status)
{
	if (!*sim)
		return ;
	destroy_mutexes(*sim);
	destroy_forks(*sim);
	free_resources(sim, ph);
	exit (status);
}
