#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;
bool is_running = false;
uint32_t* color_buffer = NULL;

int window_width = 800;
int window_height = 800;

bool initilizeWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	//window_width=display_mode.w;
	//window_height = display_mode.h;

	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_BORDERLESS);

	if (!window) {
		fprintf(stderr, "Error creating a SDL window. \n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creatin SDL renderer. \n");
	}

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void setup(void) {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
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

void clear_colorbuffer(uint32_t color) {
	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void draw_gradient(void) {
	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			color_buffer[(window_width * y) + x] = 0xFF000000 + 0xFF001100;
		}
	}
}

void draw_grid(uint32_t color) {
	int grid_step = 10;

	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			if (x % grid_step == 0 || y % grid_step == 0 ) {
				color_buffer[(window_width * y) + x] = color;
			}

		}
	}
}

void draw_rect(int start_x, int start_y, int width, int height, uint32_t color) {

	//Check if start values and size values are within the window, 
	// else the for loop below will not be able to compute, 
	// index out of bound on color buffer

	for (int y = start_y; y < height+start_y; y++)
	{
		for (int x = start_x; x < width+start_x; x++)
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void render_color_buffer(void) {
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(window_width * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	draw_grid(0xFF333355);

	draw_rect(200, 200, 400, 400, 0xFF333333);
	render_color_buffer();
	clear_colorbuffer(0x00000000);

	SDL_RenderPresent(renderer);
}

void destroy_window(void) {
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit;
}

int main(void) {
	is_running = initilizeWindow();

	setup();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}