#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

#define N_POINTS(x) (x * x * x)
triangle_t triangles_to_render[N_MESH_FACES];

vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };
vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0 };

int fov_factor = 512;
bool is_running = false;
int prev_frame_time = 0;

void setup(void) {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);
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

	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - prev_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	prev_frame_time = SDL_GetTicks();

	cube_rotation.x += 0.001;
	cube_rotation.y += 0.001;
	cube_rotation.z += 0.001;

	for ( int i = 0; i <N_MESH_FACES; i++)
	{
		face_t mesh_face = mesh_faces[i];
		
		vec3_t face_vertices[3];

		face_vertices[0] = mesh_vertices[mesh_face.a - 1];
		face_vertices[1] = mesh_vertices[mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.x);
			transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

			transformed_vertex.z -= camera_position.z;

			vec2_t projected_point = project(transformed_vertex, fov_factor);

			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[j] = projected_point;
		}

		triangles_to_render[i] = projected_triangle;
		
	}

	//for (int i = 0; i < N_POINTS(9); i++) {

	//	vec3_t point = cube_points[i];

	//	vec3_t transformed_point = vec3_rotate_y(point, cube_rotation.y);
	//	transformed_point = vec3_rotate_x(transformed_point, cube_rotation.y);
	//	transformed_point = vec3_rotate_z(transformed_point, cube_rotation.y);

	//	transformed_point.z -= camera_position.z;
	//	projected_point[i] = project(transformed_point);
	//}
}

void render(void) {

	for (int i = 0; i < N_MESH_FACES; i++) {
		triangle_t triangle = triangles_to_render[i];

		draw_point(triangle.points[0].x, triangle.points[0].y, 5, 0xFF08FF00);
		draw_point(triangle.points[1].x, triangle.points[1].y, 5, 0xFF08FF00);
		draw_point(triangle.points[2].x, triangle.points[2].y, 5, 0xFF08FF00);

		draw_triangle(triangle, 0xFFFFFFFF);
	}


	//draw_grid(0xFF808080);

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