/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 23:36:33 by imunaev-          #+#    #+#             */
/*   Updated: 2025/02/13 15:29:36 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
 * @brief Checks if a character is a numeric digit.
 *
 * This function tests whether the given character `c` is a numeric
 * digit (characters '0' through '9').
 *
 * @param c The character to check.
 * @return int Nonzero if the character is a numeric digit, 0 otherwise.
 */
static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
/**
 * @brief Checks if a character is a whitespace character.
 *
 * This function determines whether the given character is a
 * whitespace character, which includes:
 * - Space (' ')
 * - Newline ('\n')
 * - Form feed ('\f')
 * - Carriage return ('\r')
 * - Horizontal tab ('\t')
 * - Vertical tab ('\v')
 *
 * @param c The character to check.
 * @return int 1 if the character is a whitespace character, 0 otherwise.
 */

 static int	ft_isspace(char c)
 {
	 return (c == ' ' || c == '\n' || c == '\f' || c == '\r'
		 || c == '\t' || c == '\v');
 }

/**
 * @brief Converts the initial portion of a string to an long integer.
 *
 * This function skips leading whitespace, handles an optional '+' or '-' sign,
 * and accumulates numeric characters until encountering a non-digit or the end
 * of the string. The result is returned as an integer value.
 *
 * @param str The string to be converted.
 * @return int The converted integer, or 0 if no valid conversion could be
 * performed.
 */
long	ft_atol(const char *str)
{
	int			sign;
	long long	num;

	num = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (ft_isdigit(*str))
	{
		num = num * 10 + (*str - '0');
		str++;
	}
	return ((long)(num * sign));
}
