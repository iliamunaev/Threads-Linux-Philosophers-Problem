/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:13:45 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 19:37:01 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Frees allocated memory for program data.
 *
 * This function deallocates memory used for storing philosopher structures
 * and mutexes if they were allocated. It ensures that no memory leaks occur
 * when the program terminates.
 *
 * @param data A pointer to the program's data structure.
 * @return void This function does not return a value.
 */
void	free_all(t_data *data)
{
	if (data->forks)
		free(data->forks);
	if (data->philo)
		free(data->philo);
}

/**
 * @brief Compares two strings lexicographically.
 *
 * This function compares the strings `s1` and `s2` character by character.
 * It returns an integer indicating the difference between the first
 * non-matching characters.
 *
 * @param s1 The first string to compare.
 * @param s2 The second string to compare.
 * @return int A negative value if `s1` is lexicographically smaller than `s2`,
 *         zero if they are equal, and a positive value if `s1` is greater.
 */
int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

/**
 * @brief Retrieves the current time in milliseconds.
 *
 * This function uses `gettimeofday` to obtain the current system time,
 * converts it into milliseconds, and returns the result.
 *
 * @return long The current time in milliseconds.
 */
long	get_time(void)
{
	struct timeval	tv;
	long			ms;

	gettimeofday(&tv, NULL);
	ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (ms);
}

/**
 * @brief Causes a delay for a specified amount of time.
 *
 * This function makes the program wait for the specified duration in milliseconds.
 * It continuously checks the elapsed time using `get_time()` and `usleep(100)`
 * to avoid excessive CPU usage.
 *
 * @param time_in_ms The time to wait in milliseconds.
 * @return void This function does not return a value.
 */
void	acting(long time_in_ms)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < time_in_ms)
		usleep(100);
}
