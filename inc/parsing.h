#ifndef PARSING_H
#define PARSING_H

typedef enum e_state
{
    THINKING,
    TAKEN_LEFT_FORK,
	TAKEN_RIGHT_FORK,
    EATING,
    SLEEPING,
    DIED,
    UNKNOWN
}   t_state;

typedef struct s_philosopher
{
    t_state state;
	int   meals_count;
    long    last_time;
}   t_philosopher;

t_state parse_state(const char *msg);

#endif // PARSING_H