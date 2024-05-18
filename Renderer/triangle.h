#pragma once
#include "vector.h"
#include <stdint.h>
#include "texture.h"

typedef struct {
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	uint32_t color;
} face_t;

typedef struct {
	vec4_t points[3];
	tex2_t texcoords[3];
	float avg_depth;
	uint32_t color;
	uint32_t vertex_colors[3];
} triangle_t;

typedef struct {
	vec2_t points[2];
} triangleNormal_t;

void draw_filled_triangle(int x0, int y0, int z0, int w0, int x1, int y1, int z1, int w1, int x2, int y2, int z2, int w2, uint32_t color);

void draw_textured_triangle(
	int x0, int y0, int z0, float w0, float u0, float v0, 
	int x1, int y1, int z1, float w1, float u1, float v1, 
	int x2, int y2, int z2, float w2, float u2, float v2, uint32_t* texture);

void triangle_midpoint_normal(vec3_t midpoint_normal[], vec3_t n_normalized, vec4_t tris[]);

void sort_triangle_depth(triangle_t* triangle);

vec3_t BaryCentricWeights(float px, float py, float ax, float ay, float bx, float by, float cx, float cy);