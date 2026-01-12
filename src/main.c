/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 01:18:05 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/15 00:55:13 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *simulation_thread(void *arg)
{
    t_data *data = (t_data *)arg;

    create_philo_threads(data);
    create_monitor(data);

    join_philo_threads(data);
    join_monitor_thread(data);

    return NULL;
}

/**
 * @brief main function
 * 1) checks data validity
 * 2) initializes data
 * 3) initializes forks (mutexes) and philos
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv)
{
    t_data data;
    pthread_t sim_thread;

    if (is_valid_input(argv, argc) == FALSE)
        return (INVALID_INPUT);
    if (init_data(&data, argc, argv))
        return (EXIT_FAILURE);
    if (data.no_of_meals_required == 0)
        return (MEALS_REQUIRED_ZERO);
    if (init_forks(&data) || init_philos(&data))
        return (EXIT_FAILURE);

    // run PHILO_SIMULATION in a separate thread
    pthread_create(&sim_thread, NULL, simulation_thread, &data);

    // run VISUALISATION in main thread (required by raylib/macOS)
   if (!visualise_table(&data))
        return 0;

    // simulation finishes => join thread
    pthread_join(sim_thread, NULL);

    free_data(&data);
    return (EXIT_SUCCESS);
}
