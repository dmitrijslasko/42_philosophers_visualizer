#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"

typedef struct s_coor
{
	int				x;
	long			y;
}					t_coor;

int get_philo_circle_radius(int no_of_philos);
t_coor get_table_x_y_position(float starting_angle, int radius, int philo_pos, int total_philo_count);
Color	get_circle_color(int status);
char *get_status_label(int status);

#endif // RENDERER_H