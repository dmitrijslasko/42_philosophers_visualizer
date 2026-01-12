/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 00:17:25 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/14 01:27:01 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(data->no_of_philos * sizeof(pthread_mutex_t));
	if (!data->forks)
		return (MALLOC_FAIL);
	i = 0;
	while (i < data->no_of_philos)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		if (DEBUG)
			printf(TXT_YELLOW"Fork %d initialized\n"TXT_RST, i + 1);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	destroy_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->no_of_philos)
		pthread_mutex_destroy(&data->forks[i++]);
	if (DEBUG)
		printf(TXT_B_MAGENTA">>>>>>>>>>>>> Forks destroyed\n"TXT_RST);
	return (EXIT_SUCCESS);
}
