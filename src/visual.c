#include "raylib.h"
#include <math.h>
#include "philo.h"

#define WINDOW_W	800
#define WINDOW_H	500
#define	TABLE_R		200

typedef struct s_coor
{
	int				x;
	long			y;
}					t_coor;

static t_coor get_table_x_y_position(float starting_angle, int radius, int philo_pos, int total_philo_count)
{
    t_coor pos;
    double angle_step = 360.0 / total_philo_count;

    // Начинаем сверху: 90 degrees
    double angle_deg = starting_angle - (philo_pos * angle_step);
    double angle_rad = angle_deg * (M_PI / 180.0);

    pos.x = WINDOW_W/2 + cos(angle_rad) * radius;
    pos.y = WINDOW_H/2 - sin(angle_rad) * radius;

    return pos;
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

int visualise_table(t_data *data)
{
	int x_offset = 100;
	
	SetTraceLogLevel(LOG_NONE);
	InitWindow(WINDOW_W, WINDOW_H, "philosophers 42");
	SetTargetFPS(60);

	int philo_circle_radius;
	philo_circle_radius = fmin(36, 360 / data->no_of_philos);

	Texture2D floor_img = LoadTexture("floor.png");
	float floor_target_width = WINDOW_W;
	float floor_target_height = WINDOW_H;
	Rectangle floor_src = { 0, 0, floor_img.width, floor_img.height };
	Rectangle floor_dest = {
		WINDOW_W / 2,
		WINDOW_H / 2,
		floor_target_width,
		floor_target_height
	};

	Texture2D table_img = LoadTexture("table1.png");
	float table_target_width = 400;
	float table_target_height = 400;
	Rectangle table_src = { 0, 0, table_img.width, table_img.height };
	Rectangle table_dest = {
		WINDOW_W / 2 + x_offset,
		WINDOW_H / 2,
		table_target_width,
		table_target_height
	};

	
	Texture2D fork_img = LoadTexture("fork0.png");
	Texture2D fork_active_img = LoadTexture("fork1.png");
	float fork_target_width = 20;
	float fork_target_height = 80;
	
	Rectangle fork_src = { 0, 0, fork_img.width, -fork_img.height };
	Rectangle fork_active_src = { 0, 0, fork_active_img.width, -fork_active_img.height };


	Texture2D pasta_img = LoadTexture("pasta1.png");
	float target_width = 200.0f;
	float target_height = 200.0f;
	Rectangle src = { 0, 0, pasta_img.width, pasta_img.height };
	Rectangle dest = {
		WINDOW_W / 2 + x_offset,      // CENTER X
		WINDOW_H / 2 - 5,      // CENTER Y
		target_width,
		target_height
	};
	
	float rotation = 0.0f;
	while (!WindowShouldClose()) 
	{
		BeginDrawing();

		ClearBackground(BLACK);
		
		// draw floor
		DrawTexturePro(floor_img, floor_src, floor_dest,
               (Vector2){floor_target_width/2, floor_target_height/2},
               0.0f, WHITE);

		// draw table
		DrawTexturePro(table_img, table_src, table_dest,
               (Vector2){table_target_width/2, table_target_height/2},
               0.0f, WHITE);

		Color circle_color;
		int status;

		for (int i = 0; i < data->no_of_philos; i++)
		{
			mutex_operation(data->data_access_mutex, LOCK);
			status = data->statuses[i];
			mutex_operation(data->data_access_mutex, UNLOCK);

			int circle_size = philo_circle_radius;
			circle_color = get_circle_color(status);
			if (status == SLEEPING)
				circle_size += 4;
			t_coor pos = get_table_x_y_position(90.0f, TABLE_R, i, data->no_of_philos);
			circle_color = Fade(circle_color, 0.5f); // 0.0–1.0
			DrawCircle(pos.x + x_offset, pos.y, circle_size, circle_color);

			float shift = (360.0f / data->no_of_philos) / 2;
			t_coor fork_pos = get_table_x_y_position(90.0f - shift, TABLE_R - 60, i, data->no_of_philos);

			Rectangle fork_dest = {
			fork_pos.x + x_offset, fork_pos.y - 2,
			fork_target_width,
			fork_target_height
			};
		
			{
				float angle_step = 360.0f / data->no_of_philos;
				// this is the angle of this fork on the circle
				float fork_angle = (90.0f - shift) - i * angle_step;
				// convert so fork points toward center (Raylib +90 correction)
				float fork_angle_to_center = 90.0f - fork_angle;
				Vector2 origin = { fork_target_width / 2, fork_target_height / 2 };
				DrawTexturePro(
					fork_img, 
					fork_src, 
					fork_dest,
					origin,
					fork_angle_to_center,
					WHITE);
				
				if ((data->statuses[i] == EATING || data->statuses[(i + 1) % data->no_of_philos] == EATING) || \
					data->statuses[i] == TAKEN_LEFT_FORK)
				{
				DrawTexturePro(
					fork_active_img, 
					fork_src, 
					fork_dest,
					origin,
					fork_angle_to_center,
					WHITE);
				}
			}
			
			char *label;
			if (status == EATING)
				label = "EATING";
			else if (status == SLEEPING)
				label = "SLEEPING";
			else if (status == DIED)
				label = "DIED";
			else if (status == THINKING)
				label = "THINKING";
			else if (status == TAKEN_LEFT_FORK)
				label = "HAS LEFT FORK";
			// Get text width to center it
			
			int textWidth;
			int fontSize = 16;
			textWidth = MeasureText(label, fontSize);
			DrawText(
				label,
				pos.x - textWidth / 2 + x_offset,            // center X
				pos.y - fontSize / 2 - 22,             // center Y
				fontSize,
				WHITE);

			const char *meals_count = TextFormat("%d", data->philos[i].meals_count);
			
			Color textColor = status == SLEEPING ? YELLOW : WHITE;
			fontSize = status == SLEEPING ? 32 : 22;
			textWidth = MeasureText(meals_count, fontSize);
			DrawText(
				meals_count,
				pos.x - textWidth / 2 + x_offset,            // center X
				pos.y - fontSize / 2 + 5,             // center Y
				fontSize,
				textColor
			);

			// DrawTextEx(
			// 	myFont,
			// 	meals_count,
			// 	(Vector2){pos.x - textWidth / 2 - 4, pos.y - fontSize / 2 + 1 },
			// 	fontSize,       // font size
			// 	0,        // spacing
			// 	WHITE
			// );
		}
		
		// rotate pasta
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
			
			const char *label_1 = TextFormat("%d", data->no_of_philos);
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
			
			int total_meals_consumed = 0;
			for (int i = 0; i < data->no_of_philos; i++)
				total_meals_consumed += data->philos[i].meals_count;
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

		EndDrawing();
	}
	CloseWindow();
	return 0;
}
