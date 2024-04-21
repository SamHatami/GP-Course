#include "triangle.h"
#include <stdint.h>
#include "display.h"

void int_swap(int* a, int* b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

//void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
//
//}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {

	float inc_left	= (float)(x1 - x0) / (y1 - y0); //inverse slope, x dependant of y
	float inc_right	= (float)(x2 - x0) / (y2 - y0);//inverse slope, x dependant of y

	
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

void triangle_midpoint_normal(vec3_t midpoint_normal[],vec3_t n_normalized, vec3_t tris[])
{

	midpoint_normal[0].x = (tris[0].x + tris[1].x + tris[2].x) / 3;
	midpoint_normal[0].y = (tris[0].y + tris[1].y + tris[2].y) / 3;
	midpoint_normal[0].z = (tris[0].z + tris[1].z + tris[2].z) / 3;
	

	midpoint_normal[1] = vec3_add(&midpoint_normal[0], &n_normalized);

	vec3_normalize(&midpoint_normal[1]);

	
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
