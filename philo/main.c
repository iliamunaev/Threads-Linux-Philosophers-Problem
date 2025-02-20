#include "philo.h"

static int	create_philo_threads(t_sim *sim, t_ph *ph)
{
	long	i;
	long	j;

	i = 0;
	while (i < sim->ph_count)
	{
		if (pthread_create(&sim->ph_threads[i], NULL, &philo, &ph[i]) != 0)
		{
			print_err("Error: main() pthread_create philo failed.");
			j = 0;
			while (j < i)
			{
				if (pthread_join(sim->ph_threads[j], NULL) != 0)
					return (EXIT_FAILURE);
				j++;
			}
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

static int	join_philo_threads(t_sim *sim)
{
	long	i;

	i = 0;
	while (i < sim->ph_count)
	{
		printf("[DEBUG] Joining philosopher thread %ld...\n", i + 1);
		if (pthread_join(sim->ph_threads[i], NULL) != 0)
		{
			print_err("Error: main() pthread_join failed.");
			return (EXIT_FAILURE);
		}
		printf("[DEBUG] join_philo_threads() Philosopher thread %ld joined successfully.\n", i + 1);
		i++;
	}
	printf("[DEBUG] join_philo_threads() ALL success Philosopher exit successfully.\n");

	return (EXIT_SUCCESS);
}


int	main(int ac, char **av)
{
	t_sim	*sim;
	t_ph	*ph;

	if (!is_input_valid(ac, av))
		return (EXIT_FAILURE);
	sim = init_sim(av);
	if (!sim)
		cleanup_exit(&sim, NULL, EXIT_FAILURE);
	ph = init_ph(sim, av);
	if (!ph)
		cleanup_exit(&sim, NULL, EXIT_FAILURE);
	if (create_philo_threads(sim, ph) == EXIT_FAILURE)
		cleanup_exit(&sim, &ph, EXIT_FAILURE);
	if (av[5])
	{
		if (pthread_create(&sim->monitor_thread, NULL, &superviser, sim) != 0)
			cleanup_exit(&sim, &ph, EXIT_FAILURE);
		pthread_join(sim->monitor_thread, NULL);
	}
	if (join_philo_threads(sim) == EXIT_FAILURE)
		cleanup_exit(&sim, &ph, EXIT_FAILURE);
	cleanup_exit(&sim, &ph, EXIT_SUCCESS);
}
