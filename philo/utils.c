#include "philo.h"

long ft_atol(const char *str)
{
    long result = 0;
    long sign = 1;
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

void print_err(char *msg)
{
    while (*msg)
        write(STDERR_FILENO, msg++, 1);
    write(STDERR_FILENO, "\n", 1);
}

bool is_input_valid(int ac, char **av)
{
    if (ac < 5 || ac > 6)
    {
        print_err("Usage: ./philo nb_philo t_die t_eat t_sleep [nb_must_eat]");
        return false;
    }
    for (int i = 1; i < ac; i++)
    {
        long val = ft_atol(av[i]);
        if (val <= 0)
        {
            print_err("Error: arguments must be positive integers.");
            return false;
        }
    }
    return true;
}

/*
** Get current time in ms since epoch.
*/
uint64_t get_timestamp(void)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL) != 0)
        return 0;
    return ((uint64_t)tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
}

/*
** Thread-safe logging: lock before printing, unlock afterwards.
*/
void log_action(t_ph *ph, const char *msg)
{
    uint64_t time = get_timestamp();
    pthread_mutex_lock(&ph->sim->log_m);
    printf("%llu %zu %s\n",
           (unsigned long long)time,
           ph->index + 1,
           msg);
    pthread_mutex_unlock(&ph->sim->log_m);
}

/*  Helpers to get left/right fork indices  */
size_t left(t_ph *ph)
{
    return (ph->index);
}

size_t right(t_ph *ph)
{
    return ((ph->index + 1) % ph->sim->ph_count);
}
