/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:17:25 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/14 01:26:31 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Initializes philosophers.
 */
int	init_philos(t_data *data)
{
	int	i;

	data->philos = safe_malloc(data->no_of_philos * sizeof(t_philosopher));
	if (data->philos == NULL)
		return (MALLOC_FAIL);
	i = 0;
	while (i < data->no_of_philos)
	{
		memset(&data->philos[i], 0, sizeof(t_philosopher));
		data->philos[i].id = i + 1;
		data->philos[i].fork_left = &data->forks[i];
		data->philos[i].fork_right = &data->forks[(i + 1) % data->no_of_philos];
		if (DEBUG)
			printf(TXT_GREEN "Philo [%d] initialized.\n" TXT_RST, data->philos[i].id);
		data->philos[i].data = data;
		i++;
	}
	return (EXIT_SUCCESS);
}
