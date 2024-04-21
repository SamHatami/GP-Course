#pragma once
#include "vector.h"
#include <stdint.h>
typedef struct {
	int a;
	int b;
	int c;
} face_t;

typedef struct {
	vec2_t points[3];
	float avg_depth;
} triangle_t;

typedef struct {
	vec2_t points[2];
} triangleNormal_t;

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void fill_flat_bottom_triangle(int x0, int y0, int Mx, int , int x2, int y2, uint32_t color);
void fill_flat_top_triangle(int x0, int y0, int Mx, int My, int x2, int y2, uint32_t color);

void triangle_midpoint_normal(vec3_t midpoint_normal[], vec3_t n_normalized, vec3_t tris[]);

void sort_triangle_depth(triangle_t* triangle);