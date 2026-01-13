#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 512

#define WINDOW_W	800
#define WINDOW_H	500
#define WINDOW_NAME	"philosophers 42 visualizer"
#define	TABLE_R		200

#define MAX_PHILOS 200

#define TABLE_X_OFFSET 100

int animation_is_running = 0;
int no_of_philos = 0;
int total_meals_consumed = 0;

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

typedef struct s_coor
{
	int				x;
	long			y;
}					t_coor;

typedef struct s_philosopher
{
    t_state state;
	int   meals_count;
    long    last_time;
}   t_philosopher;

t_philosopher philos[200];

static t_state parse_state(const char *msg)
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

void make_stdin_nonblocking(void)
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void parse_line(char *line)
{
    long    time;
    int     id;
    char    msg[64];
    t_state state;

    if (sscanf(line, "%ld %d %63[^\n]", &time, &id, msg) != 3)
        return;

    state = parse_state(msg);
    if (id <= 0 || id > MAX_PHILOS)
        return;

	if (id > no_of_philos)
		no_of_philos = id;
    philos[id - 1].state = state;
	// printf("[%d] State updated to %d at time %ld\n", id, state, time);
	philos[id - 1].meals_count += (state == EATING) ? 1 : 0;
	total_meals_consumed += (state == EATING) ? 1 : 0;
    philos[id - 1].last_time = time;
	
}

void poll_input_nonblocking(void)
{
    static char line[1024];
    static int  line_len = 0;

    char    buf[BUF_SIZE];
    ssize_t bytes;
    int     i;

    while (1)
    {
        bytes = read(STDIN_FILENO, buf, BUF_SIZE);

        if (bytes <= 0)
        {
            // No data available right now
            if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
                return;
            // EOF or real error
            return;
        }

        i = 0;
        while (i < bytes)
        {
            if (buf[i] == '\n')
            {
                line[line_len] = '\0';
                parse_line(line);
                line_len = 0;
            }
            else if (line_len < (int)sizeof(line) - 1)
            {
                line[line_len++] = buf[i];
            }
            i++;
        }
    }
}

void process_input(void)
{
    char    buf[BUF_SIZE];
    char    line[1024];
    int     line_len = 0;
    ssize_t bytes;
    int     i;

    while ((bytes = read(0, buf, BUF_SIZE)) > 0) {
        i = 0;
        while (i < bytes)
        {
            if (buf[i] == '\n') {
                line[line_len] = '\0';
                printf("VISUALIZER RECEIVED: %s\n", line);
                parse_line(line);
                line_len = 0;
            }
            else if (line_len < (int)sizeof(line) - 1) {
                line[line_len++] = buf[i];
            }
            i++;
        }
    }
}

static int get_philo_circle_radius(int no_of_philos)
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

static t_coor get_table_x_y_position(float starting_angle, int radius, int philo_pos, int total_philo_count)
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

static Color	get_circle_color(int status) 
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

int visualise_table(t_philosopher philos[])
{
	SetTraceLogLevel(LOG_NONE);
	InitWindow(WINDOW_W, WINDOW_H, WINDOW_NAME);
	SetTargetFPS(60);
	
	animation_is_running = 1;

	Texture2D floor_img = LoadTexture("visualizer/assets/floor.png");
	float floor_target_width = WINDOW_W;
	float floor_target_height = WINDOW_H;

	Rectangle floor_src = { 0, 0, floor_img.width, floor_img.height };
	Rectangle floor_dest = {
		WINDOW_W / 2,
		WINDOW_H / 2,
		floor_target_width,
		floor_target_height
	};

	Texture2D table_img = LoadTexture("visualizer/assets/table1.png");
	float table_target_width = 400;
	float table_target_height = 400;
	Rectangle table_src = { 0, 0, table_img.width, table_img.height };
	Rectangle table_dest = {
		WINDOW_W / 2 + TABLE_X_OFFSET,
		WINDOW_H / 2,
		table_target_width,
		table_target_height
	};
	
	Texture2D fork_img = LoadTexture("visualizer/assets/fork0.png");
	Texture2D fork_active_img = LoadTexture("visualizer/assets/fork1.png");
	float fork_target_width = 20;
	float fork_target_height = 80;
	
	Rectangle fork_src = { 0, 0, fork_img.width, -fork_img.height };
	Rectangle fork_active_src = { 0, 0, fork_active_img.width, -fork_active_img.height };

	Texture2D pasta_img = LoadTexture("visualizer/assets/pasta1.png");
	float target_width = 200.0f;
	float target_height = 200.0f;
	Rectangle src = { 0, 0, pasta_img.width, pasta_img.height };
	Rectangle dest = {
		WINDOW_W / 2 + TABLE_X_OFFSET,      // CENTER X
		WINDOW_H / 2 - 5,      				// CENTER Y
		target_width,
		target_height
	};

	float rotation = 0.0f;

	make_stdin_nonblocking();

	while (!WindowShouldClose()) 
	{
		poll_input_nonblocking();
		BeginDrawing();
		ClearBackground(RED);

		// draw floor
		DrawTexturePro(floor_img, floor_src, floor_dest,
               (Vector2){floor_target_width / 2, floor_target_height / 2}, 0.0f, WHITE);

		// draw table
		DrawTexturePro(table_img, table_src, table_dest,
               (Vector2){table_target_width / 2, table_target_height / 2}, 0.0f, WHITE);

		Color circle_color;
		int status;

		for (int i = 0; i < no_of_philos; i++)
		{
			status = philos[i].state;
			if (status == DIED)
				animation_is_running = 0;
			
			int philo_circle_size = get_philo_circle_radius(no_of_philos);
			if (status == SLEEPING) philo_circle_size += 4;

			t_coor pos = get_table_x_y_position(90.0f, TABLE_R, i, no_of_philos);

			circle_color = get_circle_color(status);
			circle_color = Fade(circle_color, 0.5f); // 0.0–1.0
			DrawCircle(pos.x + TABLE_X_OFFSET, pos.y, philo_circle_size, circle_color);

			float angle_step = 360.0f / no_of_philos;
			float rotation_shift = angle_step / 2;
			t_coor fork_pos = get_table_x_y_position(90.0f - rotation_shift, TABLE_R - 60, i, no_of_philos);

			Rectangle fork_dest = {
			fork_pos.x + TABLE_X_OFFSET, fork_pos.y - 2,
			fork_target_width,
			fork_target_height
			};
		
			// this is the angle of this fork on the circle
			float fork_angle = (90.0f - rotation_shift) - i * angle_step;
			
			// convert so fork points toward center (Raylib +90 correction)
			float fork_angle_to_center = 90.0f - fork_angle;
			Vector2 origin = { fork_target_width / 2, fork_target_height / 2 };
			

			Texture2D fork_image = fork_img;
			if ((philos[i].state == EATING) ||
				(philos[(i + 1) % no_of_philos].state == EATING))
			{
				fork_image = fork_active_img;
			}
			DrawTexturePro(
				fork_image, 
				fork_src, 
				fork_dest,
				origin,
				fork_angle_to_center,
				WHITE);
			
			char *status_label = get_status_label(status);

			int textWidth;
			int fontSize = 16;
			textWidth = MeasureText(status_label, fontSize);
			
			// status label
			DrawText(
				status_label,
				pos.x - textWidth / 2 + TABLE_X_OFFSET,            // center X
				pos.y - fontSize / 2 - 12,             // center Y
				fontSize,
				WHITE);
			
			// philosopher ID
			DrawText(
				TextFormat("%d", i + 1),
				pos.x + TABLE_X_OFFSET,            // center X
				pos.y - 36,             // center Y
				15,
				YELLOW);


			const char *meals_count = TextFormat("%d", philos[i].meals_count);
			
			Color textColor = status == SLEEPING ? YELLOW : WHITE;
			fontSize = 26;
			textWidth = MeasureText(meals_count, fontSize);
			DrawText(
				meals_count,
				pos.x - textWidth / 2 + TABLE_X_OFFSET,            // center X
				pos.y - 0,             		// center Y
				fontSize,
				textColor
			);
		}
		
		if (animation_is_running)
			rotation += 10 * GetFrameTime();   // degrees/sec
		DrawTexturePro(
			pasta_img,
			src,
			dest,
			(Vector2){target_width / 2, target_height / 2},
			rotation,
			WHITE
		);

		int textFontSize = 20;
		int startingY = 10;

			DrawText(
				"Philosopher count:",
				15,            // center X
				startingY,             // center Y
				textFontSize,
				WHITE
			);
			
			const char *label_1 = TextFormat("%d", no_of_philos);
			DrawText(
				label_1,
				225,            // center X
				startingY,             // center Y
				textFontSize,
				WHITE
			);
				DrawText(
				"Thread count:",
				15,            // center X
				startingY += 20,             // center Y
				textFontSize,
				WHITE
			);
			
			DrawText(
				label_1,
				225,            // center X
				startingY,             // center Y
				textFontSize,
				WHITE
			);
				DrawText(
				"Total meals eaten:",
				15,            // center X
				startingY += 20,             // center Y
				textFontSize,
				WHITE
			);
			
			label_1 = TextFormat("%d", total_meals_consumed);
			DrawText(
				label_1,
				225,            // center X
				startingY,             // center Y
				textFontSize,
				WHITE
			);

			DrawText(
				"Each philosopher is a thread.",
				15,            // center X
				WINDOW_H - 50,             // center Y
				textFontSize -= 2,
				WHITE
			);
			DrawText(
				"Each fork is a mutex.",
				15,            // center X
				WINDOW_H - 30,             // center Y
				textFontSize,
				WHITE
			);

			DrawText(
				"Dmitrijs Lasko | 42 Berlin | 2025",
				WINDOW_W - 195,            // center X
				WINDOW_H - 20,             // center Y
				12,
				GRAY
			);

		EndDrawing();
	}
	CloseWindow();
	return 0;
}



int main(int argc, char **argv)
{
	for (int i = 0; i < no_of_philos; i++) {
		philos[i].meals_count = 0;
	}
    visualise_table(philos);
    return 0;
}