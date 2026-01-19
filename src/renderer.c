#include "settings.h"
#include "renderer.h"
#include "parsing.h"

#include <math.h>

int get_philo_circle_radius(int no_of_philos)
{
	int radius;

	if (no_of_philos <= 10)
		radius = 36;
	else if (no_of_philos <= 20)
		radius = 30;
	else if (no_of_philos <= 50)
		radius = 24;
	else if (no_of_philos <= 100)
		radius = 18;
	else
		radius = 12;
	return (radius);
}

t_coor get_table_x_y_position(float starting_angle, int radius, int philo_pos, int total_philo_count)
{
    t_coor pos;
    double angle_step = 360.0 / total_philo_count;

    // Начинаем сверху: 90 degrees
    double angle_deg = starting_angle - (philo_pos * angle_step);
    double angle_rad = angle_deg * (M_PI / 180.0);

    pos.x = WINDOW_W / 2 + cos(angle_rad) * radius;
    pos.y = WINDOW_H / 2 - sin(angle_rad) * radius;

    return (pos);
}

Color	get_circle_color(int status) 
{
	Color	circle_color;

	if (status == EATING)
		circle_color = BLUE;
	else if (status == TAKEN_LEFT_FORK)
		circle_color = GREEN;
	else if (status == DIED)
		circle_color = YELLOW;
	else if (status == SLEEPING)
		circle_color = GRAY;
	else if (status == THINKING)
		circle_color = RED;
	else
		circle_color = WHITE;
	return (circle_color);
}  

char *get_status_label(int status)
{
	if (status == EATING)
		return "EATING";
	else if (status == SLEEPING)
		return "SLEEPING";
	else if (status == DIED)
		return "DIED";
	else if (status == THINKING)
		return "THINKING";
	else if (status == TAKEN_LEFT_FORK)
		return "HAS LEFT FORK";
	else
		return "UNKNOWN";
}