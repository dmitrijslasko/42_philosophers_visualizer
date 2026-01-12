/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmlasko <dmlasko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:54:23 by dmlasko           #+#    #+#             */
/*   Updated: 2025/11/14 01:27:03 by dmlasko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SETTINGS_H
# define SETTINGS_H

# ifndef DEBUG
#  define DEBUG 0
# endif

// SETITNGS AND LIMITS
# define MAX_PHILOS					300
# define MAX_TIME_TO_DIE_MS			5000
# define MAX_EAT_TIME_MS			5000
# define MAX_SLEEP_TIME_MS			5000
# define MAX_MEAL_COUNT				5000
# define MAX_ARGUMENT_LEN			8

# define SYNC_THREADS				1
# define SYNC_WAITTIME_US			100
# define START_DELAY_US				900
# define THREAD_START_DELAY_COEF	2		// 1/n of the eat time
# define MONITOR_FREQ_US	    	1
# define CUSTOM_SLEEP_PERIOD_US 	100
# define DEATH_MSG_DELAY_MS			2

// macros functions are forbidden in this case
// # define STRINGIFY(x) #x
// # define TOSTRING(x) STRINGIFY(x)

// COLORS
# define TXT_RST			"\033[0m"
# define TXT_RED			"\033[0;31m"
# define TXT_GREEN			"\033[0;32m"
# define TXT_YELLOW			"\033[0;33m"
# define TXT_B_MAGENTA   	"\033[1;35m"   // Bold Magenta

// MESSAGES
# define MSG_WRONG_ARGUMENT_COUNT		"Please provide 4-5 arguments.\n"
# define MSG_TOO_MANY_PHILOS			\
				"Too many philos, keep the count under 300.\n"
# define MSG_ZERO_PHILOS				\
				"0 philos... how do you think this will work?"
# define MSG_ONE_OR_MORE_ARGS_WRONG 	"One or more arguments are wrong.\n"

// MESAGES - DEBUG
# define MSG_DEBUG_LEFT_FORK			"%ld >> %ld [%d] has taken a L fork\n"
# define MSG_DEBUG_RIGHT_FORK			"%ld >> %ld [%d] has taken a R fork\n"
# define MSG_DEBUG_EAT					\
				"%ld >> %ld [%d] is eating\t\tMeals count: %d\n"
# define MSG_DEBUG_SLEEP				\
				"%ld >> %ld [%d] is sleeping\n"
# define MSG_DEBUG_THINK				\
				"%ld >> %ld [%d] is thinking\n"
# define MSG_DEBUG_DIED					"%ld >> %ld [%d] died\n"

// MESSAGES - CLEAN
# define MSG_LEFT_FORK					"%ld %d has taken a fork\n"
# define MSG_RIGHT_FORK					"%ld %d has taken a fork\n"
# define MSG_EAT						"%ld %d is eating\n"
# define MSG_SLEEP						"%ld %d is sleeping\n"
# define MSG_THINK						"%ld %d is thinking\n"
# define MSG_DIED						"%ld %d died\n"

// ERROR CODES
# define MALLOC_FAIL			1
# define INVALID_INPUT			2
# define MEALS_REQUIRED_ZERO	3

#endif
