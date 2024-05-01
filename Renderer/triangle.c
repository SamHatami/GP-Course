#include "triangle.h"
#include <stdint.h>
#include "display.h"
#include "swap.h"

//void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
//
//}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float inc_left = (float)(x1 - x0) / (y1 - y0); //inverse slope, x dependant of y
	float inc_right = (float)(x2 - x0) / (y2 - y0);//inverse slope, x dependant of y

	vec2_t start = {
		.x = x0,
		.y = y0
	};

	vec2_t end = {
		.x = x0,
		.y = y0
	};

	for (int y = y0; y <= y2; y++) {
		int x_start = round(start.x);
		int x_end = round(end.x);
		draw_line((vec2_t) { .x = x_start, .y = y }, (vec2_t) { .x = x_end, .y = y }, color);
		start.x += inc_left;
		start.y = y;

		end.x += inc_right;
		end.y = y;
	}
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	float inc_left = (float)(x2 - x0) / (y2 - y0); //inverse slope, x dependant of y
	float inc_right = (float)(x2 - x1) / (y2 - y1);//inverse slope, x dependant of y

	vec2_t start = {
		.x = x2,
		.y = y2
	};

	vec2_t end = {
		.x = x2,
		.y = y2
	};

	for (int y = y2; y >= y0; y--) {
		int x_start = round(start.x);
		int x_end = round(end.x);

		draw_line((vec2_t) { .x = x_start, .y = y }, (vec2_t) { .x = x_end, .y = y }, color);
		start.x -= inc_left;
		start.y = y;

		end.x -= inc_right;
		end.y = y;
	}
}

//Pixel-koordinater
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}

	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}

	if (y1 == y2) {
		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y0 == y1) {
		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
	}
	else {
		int My = y1;
		int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

		fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
		fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
	}
}

void draw_textured_triangle(
	int x0, int y0, float u0, float v0,
	int x1, int y1, float u1, float v1,
	int x2, int y2, float u2, float v2, uint32_t* texture) {


	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}


	// Top part of scanline rendering
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;

	if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x < x_end; x++) {
				// Draw our pixel with a custom color
				draw_pixel(x, y, (x % 6 == 0 && y % 6 == 0) ? 0xFFFF00FF : 0x00000000);
			}
		}
	}

	inv_slope_1 = 0;
	inv_slope_2 = 0;

	if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1 != 0) {
		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x < x_end; x++)
			{
				draw_pixel(x, y, (x % 6 == 0 && y % 6 == 0) ? 0xFFFF00FF : 0x00000000);
			}
		}
	}
}

vec3_t BaryCentricWeights(int px, int py, int ax, int ay, int bx, int by, int cx, int cy) {

	vec3_t weights; //not a real vectors, just a keeper of three floats.

	vec3_t p = {
		.x = px,
		.y = py
	};

	vec3_t a = {
		.x = ax,
		.y = ay
	};

	vec3_t b = {
		.x = bx,
		.y = by
	};

	vec3_t c = {
		.x = cx,
		.y = cy
	};



	vec3_t pa = vec3_sub(p, a);
	vec3_t pb = vec3_sub(p, b);
	vec3_t pc = vec3_sub(p, c);

	vec3_t triangle_side_1 = vec3_sub(a, b);
	vec3_t triangle_side_2 = vec3_sub(a, c);

	float major_triangle_area = vec3_length(vec3_cross(triangle_side_1, triangle_side_2)) / 2;

	weights.x = vec3_length(vec3_cross(pc, pb)) / major_triangle_area; //-> p1
	weights.y = vec3_length(vec3_cross(pa, pc)) / major_triangle_area; //-> p2
	weights.z = vec3_length(vec3_cross(pa, pb)) / major_triangle_area; //-> p3

	

	return weights;
}

void triangle_midpoint_normal(vec3_t midpoint_normal[], vec3_t n_normalized, vec4_t tris[])
{
	midpoint_normal[0].x = (tris[0].x + tris[1].x + tris[2].x) / 3;
	midpoint_normal[0].y = (tris[0].y + tris[1].y + tris[2].y) / 3;
	midpoint_normal[0].z = (tris[0].z + tris[1].z + tris[2].z) / 3;

	midpoint_normal[1] = vec3_add(&midpoint_normal[0], &n_normalized);

	vec3_normalize(&midpoint_normal[1]); //returns endpoint
}

void sort_triangle_depth(triangle_t* triangle)
{
	//selection sort,

	int n = array_length(triangle);

	for (int i = 0; i < n; i++)
	{
		int min = i;

		for (int j = i + 1; j < n; j++) {
			if (triangle[j].avg_depth < triangle[min].avg_depth) {
				min = j;
			}
		}

		//Sawp
		triangle_t temp = triangle[i];
		triangle[i] = triangle[min];
		triangle[min] = temp;
	}
}