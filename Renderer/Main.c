#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "upng.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include "triangle.h"
#include "texture.h"
#include "matrix.h"
#include "lights.h"
#include "Camera.h"
#define N_POINTS(x) (x * x * x)

#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;
triangleNormal_t* triangleNormals_to_render = NULL;


bool is_running = false;
float delta_time = 0;
int prev_frame_time = 0;

bool show_normals = false;
bool show_vertices = false;
bool fill_triangles = false;
bool show_triangle_edges = false;
bool flat_shading = false;
bool backface_culling = true;
bool texture = true;

light_t directionalLight;
uint32_t baseColor = 0xFFFFFFFF;

mat4_t world_matrix;
mat4_t proj_matrix;
mat4_t view_matrix;

void setup(void) {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
	z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window_width, window_height);



	//load_cube_mesh_data();
	load_png_texture_data("./Assets/efa.png");
	load_obj_file_data("./Assets/efa.obj");

	//load_obj_file_data("./Assets/Models/Geosphere.obj");
	//load_mesh_file_data("./Assets/Models/pot.obj");
	//load_obj_file_data("./Assets/cube.obj");
	//texture_width = 64;
	//texture_height = 64;
	//mesh_texture = (uint32_t*)REDBRICK_TEXTURE;
	//Initilalize perspective projection matrix
	float fov = (M_PI / 180) * 60; //Field of view in radians
	float aspect = (float)window_height / (float)window_width;
	float znear = 0.1;
	float zfar = 100;
	proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

	init_frustum_places(fov, znear, zfar)
}

void process_input(void) {
	SDL_Event event;
	SDL_MouseMotionEvent mouseEvent;
	
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

		if (event.key.keysym.sym == SDLK_1) {
			show_vertices = !show_vertices;
		}

		if (event.key.keysym.sym == SDLK_2) {
			show_triangle_edges = !show_triangle_edges;
		}

		if (event.key.keysym.sym == SDLK_4) {
			backface_culling = !backface_culling;
		}

		if (event.key.keysym.sym == SDLK_3) {
			fill_triangles = !fill_triangles;
			flat_shading = false;
		}

		if (event.key.keysym.sym == SDLK_5) {
			flat_shading = !flat_shading;
			fill_triangles = false;
		}

		if (event.key.keysym.sym == SDLK_6) {
			texture = !texture;
			fill_triangles = false;
		}

		if (event.key.keysym.sym == SDLK_7) {
			show_normals = !show_normals;
		}

		if (event.key.keysym.sym == SDLK_w) {
			camera.forward_velocity = vec3_scalar(camera.direction, 5 * delta_time);
			camera.position = vec3_add(&camera.position, &camera.forward_velocity);
		}


		if (event.key.keysym.sym == SDLK_s)
		{
			camera.forward_velocity = vec3_scalar(camera.direction, 5 * delta_time);
			camera.position = vec3_sub(camera.position, camera.forward_velocity);
		}

		if (event.key.keysym.sym == SDLK_a)
	
			camera.position.x -= 1.0 * delta_time;


		if (event.key.keysym.sym == SDLK_d)
			camera.position.x += 1.0 * delta_time;

		if (event.key.keysym.sym == SDLK_UP)
			camera.position.y += 1.0 * delta_time;
		
		if (event.key.keysym.sym == SDLK_DOWN)
			camera.position.y -= 1.0 * delta_time;

		if (event.key.keysym.sym == SDLK_LEFT)
			camera.yaw -= 1.0 * delta_time;

		if (event.key.keysym.sym == SDLK_RIGHT)
			camera.yaw += 1.0 * delta_time;
		break;

	}
}

void update(void) {
	//int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - prev_frame_time);

	//if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
	//	SDL_Delay(time_to_wait);
	//}

	delta_time = (SDL_GetTicks() - prev_frame_time)/1000.0;
	prev_frame_time = SDL_GetTicks();

	directionalLight.direction.x = 0;
	directionalLight.direction.y = 0;
	directionalLight.direction.z = 1;


	//Initialize the counter of triangles to render for the current frame
	num_triangles_to_render = 0;
	triangleNormals_to_render = NULL;

	//mesh.rotation.x += 5*delta_time;

	//camera.position.x += 0.008;
	//camera.position.y += 0.008;

	mesh.translation.y = 0;
	mesh.translation.z = 5.0;

	//Create the view-matrix look at a hardcoded target point

	

	vec3_t target = { 0, 0, 1 };
	mat4_t camera_yaw_rotation = mat4_rotate(camera.yaw, 1);
	vec4_t newtarget = vec3_to_vec4(target);
	
	camera.direction = vec4_to_vec3(mat4_mul_vec4(camera_yaw_rotation, vec3_to_vec4(target)));

	//Offset the camera position in the direction where the camera i spoiinting at
	target = vec3_add(&camera.position, &camera.direction);
	vec3_t up_direction = { 0, 1, 0 };

	view_matrix = mat4_look_at(camera.position, target, up_direction);

	//Create scale, rotation and translation matrices that will be used to multiply
	mat4_t scale_matrix = mat4_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);

	mat4_t translation_matrix = mat4_translate(mesh.translation.x, mesh.translation.y, mesh.translation.z);

	mat4_t rotation_x = mat4_rotate(mesh.rotation.x, 0);

	mat4_t rotation_y = mat4_rotate(mesh.rotation.y, 1);

	mat4_t rotation_z = mat4_rotate(mesh.rotation.z, 2);

	int numfaces = array_length(mesh.faces);
	for (int i = 0; i < numfaces; i++)
	{
		face_t mesh_face = mesh.faces[i];

		vec3_t face_vertices[3];
		vec3_t face_verticesNormals[3];
		vec3_t transformed_normal;

		face_vertices[0] = mesh.vertices[mesh_face.a];
		face_vertices[1] = mesh.vertices[mesh_face.b];
		face_vertices[2] = mesh.vertices[mesh_face.c];

		if(mesh.vertex_normals != NULL)
		{
			face_verticesNormals[0] = mesh.vertex_normals[mesh_face.a];
			face_verticesNormals[1] = mesh.vertex_normals[mesh_face.b];
			face_verticesNormals[2] = mesh.vertex_normals[mesh_face.c];
		}

		vec4_t transformed_vertices[3];
		vec3_t transformed_verticesNormals[3];

		for (int j = 0; j < 3; j++) {
			vec4_t transformed_vertex = vec3_to_vec4(face_vertices[j]);
			vec4_t transformed_vertexNormal = vec3_to_vec4(face_verticesNormals[j]);

			world_matrix = mat4_identity();
			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_z, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_x, world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

			//World matrix * 
			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);
			transformed_vertexNormal = mat4_mul_vec4(world_matrix, transformed_vertexNormal);

			transformed_vertex = mat4_mul_vec4(view_matrix, transformed_vertex);
			transformed_vertexNormal = mat4_mul_vec4(view_matrix, transformed_vertexNormal);

			transformed_vertices[j] = transformed_vertex;
			transformed_verticesNormals[j] = vec4_to_vec3(transformed_vertexNormal);
		}

		vec3_t vector_a = vec4_to_vec3(transformed_vertices[0]);
		vec3_t vector_b = vec4_to_vec3(transformed_vertices[1]);
		vec3_t vector_c = vec4_to_vec3(transformed_vertices[2]);

		vec3_t vector_ab = vec3_sub(vector_b, vector_a);
		vec3_t vector_ac = vec3_sub(vector_c, vector_a);

		vec3_normalize(&vector_ab);
		vec3_normalize(&vector_ac);

		vec3_t normal = vec3_cross(vector_ab, vector_ac); //order -> left handed coordinate system

		vec3_normalize(&normal);

		vec3_t origin = { 0,0,0 };

		//Find the vector between vertex A in the triangle and the camera origin
		vec3_t camera = vec3_sub(origin, vector_a);

		float dotCamera = vec3_dot(normal, camera);

		// vector to camera

			if (dotCamera < 0 && backface_culling) { continue; }
		

		triangleNormal_t  projected_normal;

		triangle_t projected_triangle;

		vec4_t projected_point[3];

		//*** PROJECTION ***

		for (int j = 0; j < 3; j++) {
			projected_point[j] = mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

			projected_point[j].x *= (window_width / 2.0);
			projected_point[j].y *= (window_height / 2.0);

			//Invert y
			projected_point[j].y *= -1;

			//Translate to the middle of the screen
			projected_point[j].x += (window_width / 2.0);
			projected_point[j].y += (window_height / 2.0);

			projected_triangle.points[j].x = projected_point[j].x;
			projected_triangle.points[j].y = projected_point[j].y;
			projected_triangle.points[j].z = projected_point[j].z;
			projected_triangle.points[j].w = projected_point[j].w;
		
		}

		projected_triangle.texcoords[0].u = mesh_face.a_uv.u;
		projected_triangle.texcoords[1].u = mesh_face.b_uv.u;
		projected_triangle.texcoords[2].u = mesh_face.c_uv.u;

		projected_triangle.texcoords[0].v = mesh_face.a_uv.v;
		projected_triangle.texcoords[1].v = mesh_face.b_uv.v;
		projected_triangle.texcoords[2].v = mesh_face.c_uv.v;

		if (flat_shading)
		{
			float dotLight = -1 * vec3_dot(normal, directionalLight.direction);

			projected_triangle.color = light_apply_intensity(baseColor, dotLight);
		}

		//if (!flat_shading) {
		//	for (int j = 0; j < 3; j++) {
		//		float vertex_dotLight = -1 * vec3_dot(transformed_verticesNormals[j], directionalLight.direction);

		//		projected_triangle.vertex_colors[j] = light_apply_intensity(baseColor, vertex_dotLight);
		//
		//	}
		//}

		
		
		if (show_normals) {
			vec3_t midpoint_normal[2];
			vec4_t v4[2];

			//TOOD: Remake this into vec4 -> vec3
			triangle_midpoint_normal(midpoint_normal, normal, transformed_vertices);
			

			for (int k = 0; k < 2; k++) {
				v4[k] = vec3_to_vec4(midpoint_normal[k]);
				vec4_t projected_normal_point = mat4_mul_vec4_project(proj_matrix, v4[k]);

				projected_normal_point.x *= (window_width / 2);
				projected_normal_point.y *= (window_height / 2);
				
				projected_normal_point.y *= -1;

				projected_normal_point.x += (window_width / 2);
				projected_normal_point.y += (window_height / 2);

				projected_normal.points[k].x = projected_normal_point.x;
				projected_normal.points[k].y = projected_normal_point.y;
			}
			array_push(triangleNormals_to_render, projected_normal);
		}

		// Light caluclations  -> vector to single light source

		if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH) {
			triangles_to_render[num_triangles_to_render++] = projected_triangle;
		}

	}

	//sort_triangle_depth(triangles_to_render);
}

void render(void) {
	
	for (int i = 0; i < num_triangles_to_render; i++) {
		triangle_t triangle = triangles_to_render[i];
		if (show_normals) {
			triangleNormal_t normal = triangleNormals_to_render[i];
			draw_normal(normal, 0xFFFFFF00);
		}


		if (flat_shading)
		{
			draw_filled_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
				triangle.color
			);
		}

		if (texture) {

			draw_textured_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texcoords[0].u, triangle.texcoords[0].v,
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texcoords[1].u, triangle.texcoords[1].v,
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texcoords[2].u, triangle.texcoords[2].v,
				mesh_texture
			);

		}

		if (fill_triangles)
		{
			draw_filled_triangle(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
				0xFF808080
			);
		}

		if (show_triangle_edges)
			draw_triangle(triangle, 0xFFFFFFFF);

		if (show_vertices) {
			draw_point(triangle.points[0].x, triangle.points[0].y, 5, 0xFF08FF00);
			draw_point(triangle.points[1].x, triangle.points[1].y, 5, 0xFF08FF00);
			draw_point(triangle.points[2].x, triangle.points[2].y, 5, 0xFF08FF00);
		}
	}

	//draw_grid(0xFF808080);

	render_color_buffer();

	clear_colorbuffer(0x00000000);
	clear_z_buffer();

	SDL_RenderPresent(renderer);
}

void free_resources(void) {
	free(color_buffer);
	free(z_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
	upng_free(png_texture);
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