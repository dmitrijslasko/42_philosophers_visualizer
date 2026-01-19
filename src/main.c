#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "settings.h"
#include "parsing.h"
#include "renderer.h"

typedef struct s_data {
	int animation_is_running;
	int no_of_philos;
	int total_meals_consumed;
	t_philosopher philos[MAX_PHILOS];
}	t_data;

typedef struct s_assets
{
    Texture2D floor;
    Texture2D table;
    Texture2D fork;
    Texture2D fork_active;
    Texture2D meal;
} t_assets;

void make_stdin_nonblocking(void)
{
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void parse_line(t_data *data, char *line)
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

	if (id > data->no_of_philos)
		data->no_of_philos = id;

    data->philos[id - 1].state = state;
	data->philos[id - 1].meals_count += (state == EATING) ? 1 : 0;

	data->total_meals_consumed += (state == EATING) ? 1 : 0;
    data->philos[id - 1].last_time = time;
	
}

void poll_input_nonblocking(t_data *data)
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
                parse_line(data, line);
				if (PRINT_INPUT)
					printf("VISUALIZER RECEIVED: %s\n", line);
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

static void draw_top(t_data *data) {

	int startingY = 10;
	DrawText(
		"Philosopher count:",
		15,            // center X
		startingY,             // center Y
		20,
		WHITE
	);
	
	DrawText(
		TextFormat("%d", data->no_of_philos),
		225,            // center X
		startingY,             // center Y
		20,
		WHITE
	);

	DrawText(
		"Thread count:",
		15,            // center X
		startingY += 20,             // center Y
		20,
		WHITE
	);
	
	DrawText(
		TextFormat("%d", data->no_of_philos),
		225,            // center X
		startingY,             // center Y
		20,
		WHITE
	);
	
	DrawText(
		"Total meals eaten:",
		15,            // center X
		startingY += 20,             // center Y
		20,
		WHITE
	);
	
	DrawText(
		TextFormat("%d", data->total_meals_consumed),
		225,            // center X
		startingY,             // center Y
		20,
		WHITE
	);
}

static void draw_bottom() {
	DrawText(
		"Each philosopher is a thread.",
		15,            // center X
		WINDOW_H - 50,             // center Y
		18,
		WHITE
	);
	DrawText(
		"Each fork is a mutex.",
		15,            // center X
		WINDOW_H - 30,             // center Y
		18,
		WHITE
	);
	
	DrawText(
		"Dmitrijs Lasko | 42 Berlin | 2025",
		WINDOW_W - 195,            // center X
		WINDOW_H - 20,             // center Y
		12,
		GRAY
	);
}

int visualise_table(t_data *data)
{
	SetTraceLogLevel(LOG_NONE);
	InitWindow(WINDOW_W, WINDOW_H, WINDOW_NAME);
	SetTargetFPS(60);
	
	data->animation_is_running = 1;

	float meal_rotation = 0.0f;
	
	Texture2D floor_img = LoadTexture("visualizer/assets/floor.png");
	Rectangle floor_src = { 0, 0, floor_img.width, floor_img.height };
	Rectangle floor_dest = {
		WINDOW_W / 2,
		WINDOW_H / 2,
		WINDOW_W,
		WINDOW_H
	};

	Texture2D table_img = LoadTexture("visualizer/assets/table1.png");
	Rectangle table_src = { 0, 0, table_img.width, table_img.height };
	Rectangle table_dest = {
		WINDOW_W / 2 + TABLE_X_OFFSET,
		WINDOW_H / 2,
		TABLE_TARGET_SIZE,
		TABLE_TARGET_SIZE
	};
	
	Texture2D fork_img = LoadTexture("visualizer/assets/fork0.png");
	Texture2D fork_active_img = LoadTexture("visualizer/assets/fork1.png");
	float fork_target_width = 20;
	float fork_target_height = 80;
	
	Rectangle fork_src = { 0, 0, fork_img.width, -fork_img.height };
	Rectangle fork_active_src = { 0, 0, fork_active_img.width, -fork_active_img.height };

	Texture2D meal_img = LoadTexture("visualizer/assets/meal.png");
	float target_width = 200.0f;
	float target_height = 200.0f;
	Rectangle src = { 0, 0, meal_img.width, meal_img.height };
	Rectangle dest = {
		WINDOW_W / 2 + TABLE_X_OFFSET,      // CENTER X
		WINDOW_H / 2 - 5,      				// CENTER Y
		target_width,
		target_height
	};

	float fork_rotation = 0.0f;

	make_stdin_nonblocking();

	while (!WindowShouldClose()) 
	{
		poll_input_nonblocking(data);
		BeginDrawing();
		ClearBackground(BLACK);

		// draw floor
		DrawTexturePro(floor_img, floor_src, floor_dest,
               (Vector2){WINDOW_W / 2, WINDOW_H / 2}, 0.0f, WHITE);

		// draw table
		DrawTexturePro(table_img, table_src, table_dest,
               (Vector2){TABLE_TARGET_SIZE / 2, TABLE_TARGET_SIZE / 2}, 0.0f, WHITE);

		for (int i = 0; i < data->no_of_philos; i++)
		{
			int status = data->philos[i].state;
			if (status == DIED)
				data->animation_is_running = 0;
			
			int philo_circle_size = get_philo_circle_radius(data->no_of_philos);
			if (status == SLEEPING) philo_circle_size += 4;

			t_coor pos = get_table_x_y_position(90.0f, TABLE_TARGET_SIZE/2, i, data->no_of_philos);

			Color circle_color = get_circle_color(status);
			circle_color = Fade(circle_color, 0.5f); // 0.0–1.0
			DrawCircle(pos.x + TABLE_X_OFFSET, pos.y, philo_circle_size, circle_color);

			float angle_step = 360.0f / data->no_of_philos;	
			float rotation_shift = angle_step / 2;
			t_coor fork_pos = get_table_x_y_position(90.0f - rotation_shift, TABLE_TARGET_SIZE/2 - 60, i, data->no_of_philos);

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
			if ((data->philos[i].state == EATING) ||
				(data->philos[(i + 1) % data->no_of_philos].state == EATING))
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
		
			int fontSize = 16;	
			int textWidth = MeasureText(status_label, fontSize);

			// status label
			DrawText(
				status_label,
				pos.x - textWidth / 2 + TABLE_X_OFFSET,            // center X
				pos.y - fontSize / 2 - 12,             // center Y
				fontSize,
				WHITE);
			
			// philosopher ID
			textWidth = MeasureText(TextFormat("%d", i + 1), 15);
			DrawText(
				TextFormat("%d", i + 1),
				pos.x + TABLE_X_OFFSET - textWidth / 2,            // center X
				pos.y - 38,             // center Y
				15,
				YELLOW);

			// meals count
			Color textColor = status == SLEEPING ? YELLOW : WHITE;
			const char *meals_count = TextFormat("%d", data->philos[i].meals_count);
			textWidth = MeasureText(meals_count, fontSize);
			DrawText(
				meals_count,
				pos.x - textWidth / 2 + TABLE_X_OFFSET,            // center X
				pos.y - 0,             							// center Y
				26,
				textColor
			);
		}
		
		// draw meal
		if (data->animation_is_running)
			meal_rotation += MEAL_ROTATION_SPEED * GetFrameTime();   // degrees/sec

		DrawTexturePro(
			meal_img,
			src,
			dest,
			(Vector2){target_width / 2, target_height / 2},
			meal_rotation,
			WHITE
		);

		draw_top(data);
		draw_bottom();

		EndDrawing();
	}
	CloseWindow();
	return (0);
}

int main(int argc, char **argv)
{
	t_data data;

	// init data
	data.animation_is_running = 0;
	data.no_of_philos = 0;
	data.total_meals_consumed = 0;

	for (int i = 0; i < MAX_PHILOS; i++) {
		data.philos[i].meals_count = 0;
	}
    visualise_table(&data);
    return (0);
}