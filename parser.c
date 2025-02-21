/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 19:12:40 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/21 19:34:16 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Converts a string to an integer.
 *
 * This function interprets the initial portion of the string `str` as an
 * integer and returns its value. It skips any leading whitespace characters
 * (including spaces, tabs, and newlines) before processing numeric digits.
 * The function does not handle negative numbers or overflow correctly beyond
 * INT_MAX (2147483647), returning -1 in such cases.
 *
 * @param str The string representation of the integer.
 * @return int The converted integer value. Returns -1 if the value exceeds
 *         the maximum range of a signed 32-bit integer.
 */
static int	ft_atoi(const char *str)
{
	long	res;
	int		i;

	res = 0;
	i = 0;
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

/**
 * @brief Parses command-line arguments and initializes program data.
 *
 * This function validates and extracts parameters from the command-line
 * arguments. It ensures that the correct number of arguments is provided
 * (between 5 and 6) and converts them to integer values using `ft_atoi`.
 * If an argument is invalid (e.g., less than 1), an error message is printed,
 * and the function returns a failure code.
 *
 * @param argc The number of arguments.
 * @param argv The array of argument strings.
 * @param data A pointer to the structure where parsed values are stored.
 * @return int Returns EXIT_SUCCESS (0) if parsing is successful, otherwise
 *         returns EXIT_FAILURE (1) on error.
 */
int	parse_args(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments.\n");
		return (EXIT_FAILURE);
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
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Parses command-line arguments and initializes program data.
 *
 * This function first parses the command-line arguments using `parse_args()`
 * and validates the provided input. If parsing is successful, it proceeds
 * to initialize necessary data structures and resources via `init_all()`.
 * If initialization fails, allocated resources are freed before returning
 * a failure code. The function also records the start time of execution.
 *
 * @param data A pointer to the program's data structure.
 * @param argc The number of command-line arguments.
 * @param argv The array of argument strings.
 * @return int Returns EXIT_SUCCESS (0) if successful, otherwise EXIT_FAILURE (1).
 */
int	parse_and_init(t_data *data, int argc, char **argv)
{
	if (parse_args(argc, argv, data) == 1)
		return (EXIT_FAILURE);
	if (init_all(data) == 1)
	{
		free_all(data);
		return (EXIT_FAILURE);
	}
	data->start_time = get_time();
	return (EXIT_SUCCESS);
}
