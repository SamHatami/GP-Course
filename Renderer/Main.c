#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS(x) (x * x * x)
vec3_t cube_points[N_POINTS(9)];
vec2_t projected_point[N_POINTS(9)];

vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };
int fov_factor = 512;
bool is_running = false;

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

vec2_t project(vec3_t v) {
	vec2_t projected = {
		.x = v.x/v.z,
		.y = v.y/v.z
	};
	return projected;

}
void update(void) {

	for (int i = 0; i < N_POINTS(9); i++) {

		vec3_t point = cube_points[i];
		point.z -= camera_position.z;
		projected_point[i] = project(point);
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