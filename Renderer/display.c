#include "display.h"
#include "triangle.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;

uint32_t* color_buffer = NULL;

int window_width = 1280;
int window_height = 1280;

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
			color_buffer[(window_width * y) + x] = 0xFF000000;
		}
	}
}

void draw_2dgrid(uint32_t color) {
	int grid_step = 10;

	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			if (x % grid_step == 0 || y % grid_step == 0) {
				color_buffer[(window_width * y) + x] = color;
			}
		}
	}
}

void draw_grid(uint32_t color) {
	int grid_step = 10;

	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			if (x % grid_step == 0 || y % grid_step == 0) {
				color_buffer[(window_width * y) + x] = color;
			}
		}
	}
}

void draw_rect(int start_x, int start_y, int width, int height, uint32_t color) {
	//Check if start values and size values are within the window,
	// else the for loop below will not be able to compute,
	// index out of bound on color buffer

	for (int y = start_y; y < height + start_y; y++)
	{
		for (int x = start_x; x < width + start_x; x++)
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void draw_point(int pos_x, int pos_y, int size, uint32_t color) {
	//Check if start values and size values are within the window,
	// else the for loop below will not be able to compute,
	// index out of bound on color buffer

	if (pos_x >= 0 && pos_y >= 0 && pos_x <= window_height && pos_y <= window_width) {

		for (int y = pos_y; y < pos_y + size; y++)
		{
			for (int x = pos_x; x < pos_x + size; x++)
			{
				color_buffer[(window_width * y) + x] = color;
			}
		}
	}

}


void draw_pixel(int x, int y, uint32_t color) {
	color_buffer[(window_width * y) + x] = color;
}
void render_color_buffer(void) {
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(window_width * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void destroy_window(void) {

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit;
}

void draw_triangle(triangle_t triangle, uint32_t color) {

	draw_line(triangle.points[0], triangle.points[1], 0xFFFFFFFF);
	draw_line(triangle.points[1], triangle.points[2], 0xFFFFFFFF);
	draw_line(triangle.points[2], triangle.points[0], 0xFFFFFFFF);
}

void draw_line(vec2_t p1, vec2_t p2, uint32_t color) {

	int delta_x = p2.x - p1.x;
	int delta_y = p2.y - p1.y;

	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;

	float current_x = p1.x;
	float current_y = p1.y;

	for (int i = 0; i <= side_length; i++) {
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}

}