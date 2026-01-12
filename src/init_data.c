/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 23:46:06 by dmlasko           #+#    #+#             */
/*   Updated: 2025/01/08 13:47:59 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	free_data(t_data *data)
{
	mutex_operation(data->data_access_mutex, DESTROY);
	mutex_operation(data->print_mutex, DESTROY);
	free(data->data_access_mutex);
	free(data->print_mutex);
	free(data->forks);
	free(data->philos);
	free(data->philo_threads);
	return (EXIT_SUCCESS);
}

/**
 * initialise program data (data struct)
 * @param data struct to hold the program's data
 * @param argc argument count
 * @param argv argument values
 * @return returns EXIT_SUCCESS on succesful exit
 */
int	init_data(t_data *data, int argc, char **argv)
{
	data->no_of_philos = int_atoi(argv[1]);
	data->time_to_die_ms = int_atoi(argv[2]);
	data->time_to_eat_ms = int_atoi(argv[3]);
	data->time_to_sleep_ms = int_atoi(argv[4]);
	data->no_of_meals_required = -1;
	if (argc == 6)
		data->no_of_meals_required = int_atoi(argv[5]);
	data->thread_start_delay_ms = data->time_to_eat_ms / THREAD_START_DELAY_COEF;
	data->simulation_start_time_ms = 0;
	data->simulation_start_time_us = 0;
	data->simulation_status = 0;
	data->forks = NULL;
	data->philos = NULL;
	data->philo_threads = NULL;
	data->monitor_thread = (pthread_t) NULL;
	data->data_access_mutex = safe_malloc(sizeof(t_mtx));
	mutex_operation(data->data_access_mutex, INIT);
	data->print_mutex = safe_malloc(sizeof(t_mtx));
	mutex_operation(data->print_mutex, INIT);

	data->statuses = safe_malloc(sizeof(int) * data->no_of_philos);
	bzero(data->statuses, data->no_of_philos);
	
	return (EXIT_SUCCESS);
}
