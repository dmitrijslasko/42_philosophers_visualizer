/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:49:02 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/14 01:23:40 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// LIBRARIES
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>
# include <string.h>

# include "settings.h"

// STRUCTS
typedef struct s_data		t_data;
typedef pthread_mutex_t		t_mtx;

typedef enum e_boolean
{
	FALSE,
	TRUE,
}	t_boolean;

typedef enum e_opcode
{
	INIT,
	DESTROY,
	LOCK,
	UNLOCK,
}	t_opcode;

typedef enum e_status
{
	TAKEN_LEFT_FORK,
	TAKEN_RIGHT_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DIED
}	t_status;

typedef struct s_philosopher
{
	int				id;
	long			last_meal_time_ms;
	int				meals_count;
	t_mtx			*fork_left;
	t_mtx			*fork_right;
	t_data			*data;
}					t_philosopher;

typedef struct s_data
{
	int				no_of_philos;
	int				time_to_die_ms;
	int				time_to_eat_ms;
	int				time_to_sleep_ms;
	int				no_of_meals_required;
	int				thread_start_delay_ms;
	long			simulation_start_time_ms;
	long			simulation_start_time_us;
	int				simulation_status;
	t_mtx			*forks;
	t_philosopher	*philos;
	pthread_t		*philo_threads;
	pthread_t		monitor_thread;
	t_mtx			*data_access_mutex;
	t_mtx			*print_mutex;
	int				*statuses;
}					t_data;

// FUNCTIONS
// aux.c
int		int_atoi(const char *str);
int		ft_strlen(const char *str);

// parsing.c
int		contains_only_digits(const char *str);
int		is_valid_single_argument(const char *str);
int		is_valid_input(char **argv, int argc);

// init_data.c
int		free_data(t_data *data);
int		init_data(t_data *data, int argc, char **argv);

// init_forks.c
int		init_forks(t_data *data);
int		destroy_forks(t_data *data);

// init_philos.c
int		init_philos(t_data *data);

// monitor_func.c
int		philo_is_alive(t_data *data, t_philosopher *philo);
int		all_philos_are_alive(t_data *data);
int		philo_is_full(t_data *data, t_philosopher *philo);
int		all_philos_are_full(t_data *data);

// monitor_threads.c
int		create_monitor(t_data *data);
void	join_monitor_thread(t_data *data);

// output.c
int		print_error(char *str, int error_code);
void	write_status_debug(t_data *data, t_philosopher *philo, t_status status);
void	write_status(t_data *data, t_philosopher *philo, t_status status);

// parsing.c
int		is_valid_input(char **argv, int argc);

// philo_routine.c
void	wait_for_all_threads(t_data *data);
void	*philosopher_routine(void *arg);

// philo_threads.c
int		join_philo_threads(t_data *data);
int		create_philo_threads(t_data *data);

// safe_functions.c
void	*safe_malloc(int size_bytes);
void	mutex_operation(t_mtx *mutex, t_opcode opcode);
void	set_protected_value(t_data *data, void *ptr, long value);
long	get_protected_value(t_data *data, void *ptr);

// sleep.c
void	msleep(t_data *data, unsigned int sleep_time_ms);

// stats.c
void	print_stats(t_data *data);

// time.c
long	get_epoch_time_s_part(void);
long	get_epoch_time_us_part(void);
long	get_epoch_time_us(void);
long	get_epoch_time_ms(void);

// time2.c
long	get_sim_runtime_ms(t_data *data);
long	get_sim_runtime_us(t_data *data);

int visualise_table(t_data *data);


#endif
