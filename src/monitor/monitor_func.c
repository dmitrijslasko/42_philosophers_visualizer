/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 17:57:42 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/15 12:50:45 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Checks if a philospher is still alive.
 */
int	philo_is_alive(t_data *data, t_philosopher *philo)
{
	int	last_meal_ms;
	int	response;

	response = 1;
	mutex_operation(data->data_access_mutex, LOCK);
	last_meal_ms = philo->last_meal_time_ms;
	if (get_sim_runtime_ms(data) - last_meal_ms > data->time_to_die_ms)
		response = 0;
	mutex_operation(data->data_access_mutex, UNLOCK);
	return (response);
}

/**
 * Checks if all philosphers are still alive.
 */
int	all_philos_are_alive(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->no_of_philos)
	{
		if (philo_is_alive(data, &data->philos[i]) == 0)
		{
			mutex_operation(data->data_access_mutex, LOCK);
			data->statuses[i] = DIED;
			mutex_operation(data->data_access_mutex, UNLOCK);
			write_status(data, &data->philos[i], DIED);
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

/**
 * Checks if a philospher is full (has has enough food).
 */
int	philo_is_full(t_data *data, t_philosopher *philo)
{
	int	response;

	response = 0;
	mutex_operation(data->data_access_mutex, LOCK);
	if (philo->meals_count >= data->no_of_meals_required && \
				data->no_of_meals_required > 0)
		response = 1;
	mutex_operation(data->data_access_mutex, UNLOCK);
	return (response);
}

/**
 * Checks if all philosphers are full (have had enough food).
 */
int	all_philos_are_full(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->no_of_philos)
	{
		if (philo_is_full(data, &data->philos[i]) != 1)
			return (FALSE);
		i++;
	}
	set_protected_value(data, &data->simulation_status, 0);
	return (TRUE);
}
