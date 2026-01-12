/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:46:02 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/15 12:50:41 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * Part of the philosopher's routine – taking the LEFT fork.
 */
int	philo_take_forks(t_data *data, t_philosopher *philo)
{
	write_status(data, philo, THINKING);
	
	mutex_operation(data->data_access_mutex, LOCK);
	data->statuses[philo->id - 1] = THINKING;
	mutex_operation(data->data_access_mutex, UNLOCK);

	if (philo->id % 2 == 0)
		mutex_operation(philo->fork_left, LOCK);
	else
		mutex_operation(philo->fork_right, LOCK);
	if (data->no_of_philos == 1)
	{
		mutex_operation(philo->fork_left, UNLOCK);
		return (1);
	}
	write_status(data, philo, TAKEN_LEFT_FORK);
	mutex_operation(data->data_access_mutex, LOCK);
	data->statuses[philo->id - 1] = TAKEN_LEFT_FORK;
	mutex_operation(data->data_access_mutex, UNLOCK);
	
	if (philo->id % 2 == 0)
		mutex_operation(philo->fork_right, LOCK);
	else
		mutex_operation(philo->fork_left, LOCK);
	return (0);
}

/**
 * Part of the philosopher's routine – eating.
 */
void	philo_eat(t_data *data, t_philosopher *philo)
{
	mutex_operation(data->data_access_mutex, LOCK);
	philo->last_meal_time_ms = get_sim_runtime_ms(data);
	mutex_operation(data->data_access_mutex, UNLOCK);
	
	write_status(data, philo, EATING);
	
	mutex_operation(data->data_access_mutex, LOCK);
	data->statuses[philo->id - 1] = EATING;
	mutex_operation(data->data_access_mutex, UNLOCK);
	

	msleep(data, data->time_to_eat_ms);
	
	mutex_operation(data->data_access_mutex, LOCK);
	philo->meals_count++;
	mutex_operation(data->data_access_mutex, UNLOCK);
	
	mutex_operation(philo->fork_left, UNLOCK);
	mutex_operation(philo->fork_right, UNLOCK);
}

/**
 * Part of the philosopher's routine – sleeping.
 */
void	philo_sleep(t_data *data, t_philosopher *philo)
{
	write_status(data, philo, SLEEPING);
	mutex_operation(data->data_access_mutex, LOCK);
	data->statuses[philo->id - 1] = SLEEPING;
	mutex_operation(data->data_access_mutex, UNLOCK);
	msleep(data, data->time_to_sleep_ms);
}

void	wait_for_all_threads(t_data *data)
{
	if (SYNC_THREADS)
	{
		while (1)
		{
			mutex_operation(data->data_access_mutex, LOCK);
			if (data->simulation_status == 1)
			{
				mutex_operation(data->data_access_mutex, UNLOCK);
				break ;
			}
			mutex_operation(data->data_access_mutex, UNLOCK);
			usleep(SYNC_WAITTIME_US);
		}
	}
	mutex_operation(data->data_access_mutex, LOCK);
	if (data->simulation_start_time_us == 0)
		data->simulation_start_time_us = get_epoch_time_us();
	if (data->simulation_start_time_ms == 0)
		data->simulation_start_time_ms = get_epoch_time_ms();
	mutex_operation(data->data_access_mutex, UNLOCK);
}

/**
 * A philosopher's routine.
 */
void	*philosopher_routine(void *arg)
{
	t_philosopher	*philo;
	t_data			*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	wait_for_all_threads(data);
	if (philo->id % 2 == 0)
		usleep(START_DELAY_US);
	mutex_operation(data->data_access_mutex, LOCK);
	philo->last_meal_time_ms = get_sim_runtime_ms(data);
	mutex_operation(data->data_access_mutex, UNLOCK);
	while (1)
	{
		if (get_protected_value(data, &data->simulation_status) == 0)
			break ;
		if (philo_take_forks(data, philo) == 1)
			return (NULL);
		philo_eat(data, philo);
		philo_sleep(data, philo);
	}
	return (NULL);
}
