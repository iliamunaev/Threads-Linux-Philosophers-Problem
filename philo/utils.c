#include "philo.h"

int	ft_atol(const char *str)
{
	int	result;
	int	sign;

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
	int	val;

	if (ac < 5 || ac > 6)
	{
		print_err("Usage: ./philo philos die_t eat_t sleep_t [num_meals]");
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
	return (((uint64_t)tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL));
}

void	log_action(t_ph *ph, const char *msg)
{
	uint64_t	time;

	time = get_timestamp();
	pthread_mutex_lock(&ph->sim->mtx_log);
	printf("%u %zu %s\n", (unsigned int)time, ph->index + 1, msg);
	pthread_mutex_unlock(&ph->sim->mtx_log);
}

size_t	left(t_ph *ph)
{
	return (ph->index);
}

size_t	right(t_ph *ph)
{
	return ((ph->index + 1) % ph->sim->ph_count);
}
