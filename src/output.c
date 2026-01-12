/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 18:33:15 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/14 16:36:54 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_error(char *str, int error_code)
{
	printf(TXT_RED"%s"TXT_RST, str);
	return (error_code);
}

void	write_status_debug(t_data *data, t_philosopher *philo, t_status status)
{
	long	runtime_us;
	long	runtime_ms;

	runtime_ms = get_sim_runtime_ms(data);
	runtime_us = get_sim_runtime_us(data);
	mutex_operation(data->print_mutex, LOCK);
	if (get_protected_value(data, &data->simulation_status) == 0)
		return (mutex_operation(data->print_mutex, UNLOCK));
	if (TAKEN_LEFT_FORK == status)
		printf(MSG_DEBUG_LEFT_FORK, runtime_us, runtime_ms, philo->id);
	else if (TAKEN_RIGHT_FORK == status)
		printf(MSG_DEBUG_RIGHT_FORK, runtime_us, runtime_ms, philo->id);
	else if (EATING == status)
		printf(TXT_GREEN MSG_DEBUG_EAT TXT_RST, runtime_us, runtime_ms, philo->id, \
					philo->meals_count + 1);
	else if (SLEEPING == status)
		printf(TXT_YELLOW MSG_DEBUG_SLEEP TXT_RST, runtime_us, runtime_ms, philo->id);
	else if (THINKING == status)
		printf(MSG_DEBUG_THINK, runtime_us, runtime_ms, philo->id);
	else if (DIED == status)
	{
		set_protected_value(data, &data->simulation_status, 0);
		printf(TXT_RED MSG_DEBUG_DIED TXT_RST, runtime_us, runtime_ms, philo->id);
	}
	mutex_operation(data->print_mutex, UNLOCK);
}

void	write_status(t_data *data, t_philosopher *philo, t_status status)
{
	long	runtime;

	if (DEBUG)
		return (write_status_debug(data, philo, status));
	runtime = get_sim_runtime_ms(data);
	mutex_operation(data->print_mutex, LOCK);
	if (get_protected_value(data, &data->simulation_status) == 0)
		return (mutex_operation(data->print_mutex, UNLOCK));
	if (TAKEN_LEFT_FORK == status)
		printf(MSG_LEFT_FORK, runtime, philo->id);
	else if (TAKEN_RIGHT_FORK == status)
		printf(MSG_RIGHT_FORK, runtime, philo->id);
	else if (EATING == status)
		printf(MSG_EAT, runtime, philo->id);
	else if (SLEEPING == status)
		printf(MSG_SLEEP, runtime, philo->id);
	else if (THINKING == status)
		printf(MSG_THINK, runtime, philo->id);
	else if (DIED == status)
	{
		printf(MSG_DIED, runtime, philo->id);
		set_protected_value(data, &data->simulation_status, 0);
	}
	mutex_operation(data->print_mutex, UNLOCK);
}
