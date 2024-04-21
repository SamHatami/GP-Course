#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "triangle.h"

#define N_POINTS(x) (x * x * x)
triangle_t* triangles_to_render = NULL;
triangleNormal_t* triangleNormals_to_render = NULL;
vec3_t camera_position = { .x = 0, .y = 0, .z = 0 };
//vec3_t mesh.rotation = { .x = 0, .y = 0, .z = 0 };

int fov_factor = 1500;
bool is_running = false;
bool show_normals = false;
bool show_vertices = false;
bool fill_triangles = false;
int prev_frame_time = 0;

void setup(void) {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);

	//load_cube_mesh_data();
	load_mesh_from_file("./Assets/Models/ArmorShard.obj");
	//load_obj_file_data("./Assets/Models/pot.obj");
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

		if (event.key.keysym.sym == SDLK_n) {
			show_normals = !show_normals;
		}

		if (event.key.keysym.sym == SDLK_p) {
			show_vertices = !show_vertices;
		}

		if (event.key.keysym.sym == SDLK_s) {
			fill_triangles = !fill_triangles;
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

	triangles_to_render = NULL;

	triangleNormals_to_render = NULL;

	mesh.rotation.x += 0.005;
	mesh.rotation.y += 0.005;
	mesh.rotation.z += 0.006;
	int numfaces = array_length(mesh.faces);
	for (int i = 0; i < numfaces; i++)
	{
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		vec3_t transformed_vertices[3];
		vec3_t transformed_normal;

		face_vertices[0] = mesh.vertices[mesh_face.a - 1];
		face_vertices[1] = mesh.vertices[mesh_face.b - 1];
		face_vertices[2] = mesh.vertices[mesh_face.c - 1];

	

		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.x);
			transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

			transformed_vertex.z += -5;
			transformed_vertex.y += -1;
			
			transformed_vertices[j] = transformed_vertex;
		}
		
		
		vec3_t vector_a = transformed_vertices[0];
		vec3_t vector_b = transformed_vertices[1];
		vec3_t vector_c = transformed_vertices[2];

		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_t vector_ac = vec3_sub(vector_c, vector_a);

		vec3_normalize(&vector_ab);
		vec3_normalize(&vector_ac);

		vec3_t normal = vec3_cross(vector_ab, vector_ac); //order -> left handed coordinate system

		vec3_normalize(&normal);

		vec3_t camera = vec3_sub(camera_position, vector_a);



		float dot = vec3_dot(normal, camera);

		if (dot < 0) { continue; }

		vec3_t midpoint_normal[2];
		
		triangle_midpoint_normal(midpoint_normal, normal, transformed_vertices);

		triangleNormal_t  projected_normal;
		
		if (show_normals) {

			for (int k = 0; k < 2; k++) {

				vec2_t projected_normal_point = project(midpoint_normal[k], fov_factor);

				projected_normal_point.x += (window_width / 2);
				projected_normal_point.y += (window_height / 2);

				projected_normal.points[k] = projected_normal_point;

			}
			array_push(triangleNormals_to_render, projected_normal);

		}

		triangle_t projected_triangle;


		for (int j = 0; j < 3; j++) {

			vec2_t projected_point = project(transformed_vertices[j], fov_factor);

			projected_point.x += (window_width / 2);
			projected_point.y += (window_height / 2);

			projected_triangle.points[j] = projected_point;
		}

		projected_triangle.avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;

		
		array_push(triangles_to_render, projected_triangle);
	}

	sort_triangle_depth(triangles_to_render);
}

void render(void) {
	int numtris = array_length(triangles_to_render);

	for (int i = 0; i < numtris; i++) {
		triangle_t triangle = triangles_to_render[i];
		if (show_normals) {
			triangleNormal_t normal = triangleNormals_to_render[i];
			draw_normal(normal, 0xFFFFFF00);
		}

		if (show_vertices) {
			draw_point(triangle.points[0].x, triangle.points[0].y, 5, 0xFF08FF00);
			draw_point(triangle.points[1].x, triangle.points[1].y, 5, 0xFF08FF00);
			draw_point(triangle.points[2].x, triangle.points[2].y, 5, 0xFF08FF00);
		}

		if(fill_triangles)
		{
			draw_filled_triangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				0xFF08FF00
			);
		}

		draw_triangle(triangle, 0xFFFFFFFF);

	}



	//draw_grid(0xFF808080);
	array_free(triangles_to_render);

	render_color_buffer();
	clear_colorbuffer(0x00000000);

	SDL_RenderPresent(renderer);
}

void free_resources(void) {
	free(color_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
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
	free_resources();

	return 0;
}