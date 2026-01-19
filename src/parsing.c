#include "parsing.h"
#include <string.h> // for strcmp

t_state parse_state(const char *msg)
{
    if (!msg)
        return UNKNOWN;
    if (!strcmp(msg, "is eating"))
        return EATING;
    if (!strcmp(msg, "is sleeping"))
        return SLEEPING;
    if (!strcmp(msg, "is thinking"))
        return THINKING;
    if (!strcmp(msg, "died"))
        return DIED;
    return UNKNOWN;
}