#include "triangle.h"
#include <stdint.h>
#include "display.h"
#include "swap.h"



void draw_texel(int x, int y,
	float x0, float y0, float z0, float w0, float u0, float v0,
	float x1, float y1, float z1, float w1, float u1, float v1,
	float x2, float y2, float z2, float w2, float u2, float v2, uint32_t* texture) {
	vec3_t weights = BaryCentricWeights(x, y, x0, y0, x1, y1, x2, y2);

	if (y <0 || y>window_height || x<0 || x>window_width)
		return;
	float interpolated_u = (u0 / w0) * weights.x + (u1 / w1) * weights.y + (u2 / w2) * weights.z;
	float interpolated_v = (v0 / w0) * weights.x + (v1 / w1) * weights.y + (v2 / w2) * weights.z;
	float interpolated_reciprocal_w = (1 / w0) * weights.x + (1 / w1) * weights.y + (1 / w2) * weights.z;

	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;

	int tex_x = abs((int)(interpolated_u * texture_width)) % texture_width;
	int tex_y = abs((int)(interpolated_v * texture_height)) % texture_height; //Why hacky?

	//Adjust 1/w so the pixels that are closer to the camera have smaller values
	interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

	//Only draw pixel if the depth value is less than the stored value in z-buffer
	if (interpolated_reciprocal_w < z_buffer[(window_width * y) + x])
	{
		//Draw pixel at x,y with the color from the texture
		draw_pixel(x, y, texture[(texture_width * tex_y) + tex_x]);

		z_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
	}
}

void draw_filled_pixel(int x, int y,
	float x0, float y0, float z0, float w0,
	float x1, float y1, float z1, float w1,
	float x2, float y2, float z2, float w2, uint32_t* color) {


	if (y <0 || y>window_height || x<0 || x>window_width)
		return;

	vec3_t weights = BaryCentricWeights(x, y, x0, y0, x1, y1, x2, y2);

	float interpolated_reciprocal_w = (1 / w0) * weights.x + (1 / w1) * weights.y + (1 / w2) * weights.z;

	//Adjust 1/w so the pixels that are closer to the camera have smaller values
	interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

	//Only draw pixel if the depth value is less than the stored value in z-buffer
	if (interpolated_reciprocal_w < z_buffer[(window_width * y) + x])
	{
		//Draw pixel at x,y with the color from the texture
		draw_pixel(x, y, color);

		z_buffer[(window_width * y) + x] = interpolated_reciprocal_w;
	}
}

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

//Pixel-koordinater old draw fill triangle
//void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
//	if (y0 > y1) {
//		int_swap(&y0, &y1);
//		int_swap(&x0, &x1);
//	}
//
//	if (y1 > y2) {
//		int_swap(&y1, &y2);
//		int_swap(&x1, &x2);
//	}
//	if (y0 > y1) {
//		int_swap(&y0, &y1);
//		int_swap(&x0, &x1);
//	}
//
//	if (y1 == y2) {
//		fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
//	}
//	else if (y0 == y1) {
//		fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
//	}
//	else {
//		int My = y1;
//		int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;
//
//		fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);
//		fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
//	}
//}


void draw_filled_triangle(int x0, int y0, int z0, int w0, int x1, int y1, int z1, int w1, int x2, int y2, int z2, int w2, uint32_t color) {

	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);

	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);

	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);

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
				draw_filled_pixel(
					x, y,
					x0, y0, z0, w0, 
					x1, y1, z1, w1, 
					x2, y2, z2, w2,color);
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
				draw_filled_pixel(
					x, y,
					x0, y0, z0, w0,
					x1, y1, z1, w1,
					x2, y2, z2, w2, color);
			}
		}
	}

}


void draw_textured_triangle(
	int x0, int y0, int z0, float w0, float u0, float v0,
	int x1, int y1, int z1, float w1, float u1, float v1,
	int x2, int y2, int z2, float w2, float u2, float v2, uint32_t* texture) {
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
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
				draw_texel(
					x, y,
					x0, y0, z0, w0, u0, v0,
					x1, y1, z1, w1, u1, v1,
					x2, y2, z2, w2, u2, v2,
					texture);
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
				draw_texel(
					x, y,
					x0, y0, z0, w0, u0, v0,
					x1, y1, z1, w1, u1, v1,
					x2, y2, z2, w2, u2, v2,
					texture);
			}
		}
	}
}


vec3_t BaryCentricWeights(float px, float py, float ax, float ay, float bx, float by, float cx, float cy) {
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

	float major_area = vec3_length(vec3_cross(triangle_side_1, triangle_side_2));

	weights.x = (float)vec3_length(vec3_cross(pc, pb)) / major_area; //-> p1
	weights.y = (float)vec3_length(vec3_cross(pa, pc)) / major_area; //-> p2
	weights.z = (float)(1 - weights.x - weights.y); //-> p3

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
	//selection sort, some inte funkade :)

	int n = array_length(triangle);

	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++) {
			if (triangle[i].avg_depth < triangle[j].avg_depth) {
				
				triangle_t temp = triangle[i];
				triangle[i] = triangle[j];
				triangle[j] = temp;
			}
		}


	}
}