/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 00:45:35 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/14 00:55:33 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(int size_bytes)
{
	void	*ptr;

	ptr = malloc(size_bytes);
	if (!ptr)
		print_error("Memory allocation failed!\n", 0);
	return (ptr);
}

void	mutex_operation(t_mtx	*mutex, t_opcode opcode)
{
	if (opcode == INIT)
		pthread_mutex_init(mutex, NULL);
	else if (opcode == DESTROY)
		pthread_mutex_destroy(mutex);
	else if (opcode == LOCK)
		pthread_mutex_lock(mutex);
	else if (opcode == UNLOCK)
		pthread_mutex_unlock(mutex);
	else
		print_error("Mutex operational code not recognized...\n", 0);
}

long	get_protected_value(t_data *data, void *ptr)
{
	long	result;

	result = 0;
	mutex_operation(data->data_access_mutex, LOCK);
	result = *(long *)ptr;
	mutex_operation(data->data_access_mutex, UNLOCK);
	return (result);
}

void	set_protected_value(t_data *data, void *ptr, long value)
{
	mutex_operation(data->data_access_mutex, LOCK);
	*(long *)ptr = value;
	mutex_operation(data->data_access_mutex, UNLOCK);
}
