/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 11:45:51 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/14 01:27:01 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_meal_count(t_data *data)
{
	int				i;
	t_philosopher	*philo;

	philo = data->philos;
	i = 0;
	mutex_operation(data->print_mutex, LOCK);
	while (i < data->no_of_philos)
	{
		printf(TXT_B_MAGENTA"Philosopher [%d]: %d\n"TXT_RST, \
					philo[i].id, philo[i].meals_count);
		i++;
	}
	mutex_operation(data->print_mutex, UNLOCK);
}

void	print_total_runtime(t_data *data)
{
	mutex_operation(data->print_mutex, LOCK);
	printf(TXT_YELLOW "TOTAL RUN TIME: %ld ms.\n" TXT_RST, get_sim_runtime_ms(data));
	mutex_operation(data->print_mutex, UNLOCK);
}

void	print_stats(t_data *data)
{
	print_meal_count(data);
	print_total_runtime(data);
}
