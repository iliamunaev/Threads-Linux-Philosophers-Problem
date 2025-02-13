#include "philo.h"

static int	create_philo_threads(t_sim *sim, t_ph *ph)
{
	long	i;
	long	j;

	i = 0;
	while (i < sim->ph_count)
	{
		if (pthread_create(&sim->ph_threads[i], NULL, &philosopher, &ph[i]) != 0)
		{
			print_err("Error: main() pthread_create philosopher failed.");
			j = 0;
			while (j < i)
			{
				pthread_join(sim->ph_threads[j], NULL);
				j++;
			}
			cleanup(sim, ph);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

static void	join_philo_threads(t_sim *sim)
{
	long	i;

	i = 0;
	while (i < sim->ph_count)
	{
		if (pthread_join(sim->ph_threads[i], NULL) != 0)
			print_err("Error: main() pthread_join failed.");
		i++;
	}
}

int	main(int ac, char **av)
{
	t_sim	*sim;
	t_ph	*ph;

	if (!is_input_valid(ac, av))
		return (EXIT_FAILURE);
	sim = init_sim(av);
	if (!sim)
		return (EXIT_FAILURE);
	ph = init_ph(sim, av);
	if (!ph)
	{
		cleanup(sim, NULL);
		return (EXIT_FAILURE);
	}
	if (create_philo_threads(sim, ph) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	join_philo_threads(sim);
	cleanup(sim, ph);
	return (EXIT_SUCCESS);
}
