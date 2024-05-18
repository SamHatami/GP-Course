#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "triangle.h"

#define FPS 60
#define FRAME_TARGET_TIME (1000)/FPS

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* color_buffer_texture;
extern float* z_buffer;
extern uint32_t* color_buffer;
extern int window_width;
extern int window_height;

bool initilizeWindow(void);
void render_color_buffer(void);
void clear_colorbuffer(uint32_t color);
void clear_z_buffer(void);
void destroy_window(void);
void draw_gradient(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_2dgrid(uint32_t color);
void draw_grid(int x, int y, uint32_t color);
void draw_rect(int start_x, int start_y, int width, int height, uint32_t color);
void draw_point(int pos_x, int pos_y, int size, uint32_t color);
void draw_triangle(triangle_t triangle, uint32_t color);
void draw_line(vec2_t p0, vec2_t p1, uint32_t color);
void draw_gradient_line(vec2_t p0, vec2_t p1, uint32_t color[]);
void draw_int_line(int x0, int y0, int x1, int y1, uint32_t color);
void draw_normal(triangleNormal_t n, uint32_t color);

#endif