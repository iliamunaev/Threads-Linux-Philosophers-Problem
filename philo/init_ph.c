#include "philo.h"

static void	fill_ph_data(t_ph *p, t_sim *sim, char **av, long i)
{
	p->sim = sim;
	p->index = i;
	p->last_meal_time = get_timestamp();
	p->time_to_die = ft_atol(av[2]);
	p->time_to_eat = ft_atol(av[3]);
	p->time_to_sleep = ft_atol(av[4]);
	if (av[5])
		p->number_must_eat = ft_atol(av[5]);
	else
		p->number_must_eat = -1;
	p->is_dead = false;
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
		fill_ph_data(&ph[i], sim, av, i);
		i++;
	}
	return (ph);
}
