/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 20:32:44 by dmlasko           #+#    #+#             */
/*   Updated: 2025/02/01 18:13:41 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_sim_runtime_ms(t_data *data)
{
	long	runtime_ms;

	runtime_ms = get_epoch_time_ms() - data->simulation_start_time_ms;
	return (runtime_ms);
}

long	get_sim_runtime_us(t_data *data)
{
	long	runtime_us;

	runtime_us = 0;
	mutex_operation(data->data_access_mutex, LOCK);
	runtime_us = get_epoch_time_us() - data->simulation_start_time_us;
	mutex_operation(data->data_access_mutex, UNLOCK);
	return (runtime_us);
}
