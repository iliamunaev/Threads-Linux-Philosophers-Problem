#include "philo.h"

long	ft_atol(const char *str)
{
	long	result;
	long	sign;

	result = 0;
	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = (result * 10) + (*str - '0');
		str++;
	}
	return (sign * result);
}

void	print_err(char *msg)
{
	while (*msg)
		write(STDERR_FILENO, msg++, 1);
	write(STDERR_FILENO, "\n", 1);
}

bool	is_input_valid(int ac, char **av)
{
	int		i;
	long	val;

	if (ac < 5 || ac > 6)
	{
		print_err("Usage: ./philo nb_philo t_die t_eat t_sleep [nb_must_eat]");
		return (false);
	}
	i = 1;
	while (i < ac)
	{
		val = ft_atol(av[i]);
		if (val <= 0)
		{
			print_err("Error: arguments must be positive integers.");
			return (false);
		}
		i++;
	}
	return (true);
}

uint64_t	get_timestamp(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
		return (0);
	return ((uint64_t)tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
}

void	log_action(t_ph *ph, const char *msg)
{
	uint64_t	time;

	time = get_timestamp();
	pthread_mutex_lock(&ph->sim->log_m);
	printf("%llu %zu %s\n", (unsigned long long)time, ph->index + 1, msg);
	pthread_mutex_unlock(&ph->sim->log_m);
}

size_t	left(t_ph *ph)
{
	return (ph->index);
}

size_t	right(t_ph *ph)
{
	return ((ph->index + 1) % ph->sim->ph_count);
}

void	cleanup(t_sim *sim, t_ph *ph)
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
