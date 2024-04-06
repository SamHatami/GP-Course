#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS(x) (x * x * x)
vec3_t cube_points[N_POINTS(9)];
vec2_t projected_point[N_POINTS(9)];

vec3_t camera_position = { .x = 0, .y = 0, .z = -3 };
vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0 };
int fov_factor = 512;
bool is_running = false;
int prev_frame_time = 0;

void setup(void) {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);

	int point_count = 0;

	for (float x = -1; x <= 1; x += 0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t new_point = { .x = x, .y = y, .z = z };
				cube_points[point_count++] = new_point;

			}
		}
	}
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			is_running = false;
		}
		break;
	}
}

void update(void) {

	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - prev_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	prev_frame_time = SDL_GetTicks();

	cube_rotation.y += 0.001;

	for (int i = 0; i < N_POINTS(9); i++) {

		vec3_t point = cube_points[i];

		vec3_t transformed_point = vec3_rotate_y(point, cube_rotation.y);
		transformed_point = vec3_rotate_x(transformed_point, cube_rotation.y);
		transformed_point = vec3_rotate_z(transformed_point, cube_rotation.y);

		transformed_point.z -= camera_position.z;
		projected_point[i] = project(transformed_point);
	}
}

void render(void) {

	for (int i = 0; i < N_POINTS(9); i++) {

		draw_point(fov_factor * projected_point[i].x + window_height / 2, fov_factor * projected_point[i].y + window_width / 2, 2, 0xFFFFFF00);

	}

	render_color_buffer();
	clear_colorbuffer(0x00000000);

	SDL_RenderPresent(renderer);
}

int main(void) {
	is_running = initilizeWindow();

	setup();

	vec2_t v2 = { 2.0,2.0 };

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}