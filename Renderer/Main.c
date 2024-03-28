#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS(x) (x * x * x)
vec3_t cube_points[N_POINTS(9)];

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
				vec3_t new_point = {.x = x, .y= y, .z =z };
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
}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_grid(0xFF333355);
	draw_pixel(20, 20, 0xFF00FF00);
	draw_rect(200, 200, 400, 400, 0xFF333333);
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