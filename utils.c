#include "philo.h"

/*
** Simple custom atoi with basic overflow checks for positive values.
*/
static int ft_atoi(const char *str)
{
	long	res = 0;
	int		i = 0;

	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		if (res > 2147483647)
			return (-1);
		i++;
	}
	return ((int)res);
}

/*
** Parse command-line arguments into the data struct.
*/
int parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments.\n");
		return (1);
	}
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat_count = -1;
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);

	if (data->nb_philo < 1 || data->time_to_die < 1
		|| data->time_to_eat < 1 || data->time_to_sleep < 1
		|| (argc == 6 && data->must_eat_count < 1))
	{
		printf("Error: invalid arguments.\n");
		return (1);
	}
	return (0);
}

/*
** Return current time in ms.
*/
long get_time(void)
{
	struct timeval	tv;
	long			ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (ms);
}

/*
** A custom "sleep" that spins in tiny increments to achieve ms precision.
*/
void acting(long time_in_ms)
{
	long start = get_time();
	while ((get_time() - start) < time_in_ms)
		usleep(100);
}
