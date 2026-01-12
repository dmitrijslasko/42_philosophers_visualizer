/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:10:34 by dmlasko           #+#    #+#             */
/*   Updated: 2025/01/24 01:19:55 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Function to calculate string length.
 * @param str string
 * @return length of the string
 */
int	ft_strlen(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

/**
 * Simplified atoi function. Transforms argument to int.
 * Assumes the incoming str to not have - or +.
 * @param str string argument
 * @return integer
 */
int	int_atoi(const char *str)
{
	unsigned int	nb;

	nb = 0;
	while (*str)
	{
		nb = nb * 10 + (*str - '0');
		str++;
	}
	return (nb);
}
