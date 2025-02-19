#include "philo.h"

static int	fill_ph_data(t_ph *ph, t_sim *sim, char **av, long i)
{
	uint64_t	time;

	ph->sim = sim;
	ph->index = i;
	time = get_timestamp();
	if (time == 0)
	{
		print_err("Error: init_ph() get_timestamp failed.");
		return (EXIT_FAILURE);
	}
	ph->last_meal_time = time;
	ph->time_to_die = ft_atol(av[2]);
	ph->time_to_eat = ft_atol(av[3]);
	ph->time_to_sleep = ft_atol(av[4]);
	if (av[5])
		ph->num_meals_to_eat = ft_atol(av[5]);
	else
		ph->num_meals_to_eat = -1;
	ph->is_dead = false;
	return (EXIT_SUCCESS);
}

t_ph	*init_ph(t_sim *sim, char **av)
{
	long	ph_count;
	long	i;
	t_ph	*ph;

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
		if (fill_ph_data(&ph[i], sim, av, i) == EXIT_FAILURE)
		{
			print_err("Error: init_ph() fill data failed.");
			return (NULL);
		}
		i++;
	}
	return (ph);
}
