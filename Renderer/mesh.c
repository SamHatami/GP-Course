#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mesh.h"
#include "array.h"

mesh_t mesh = {
	.vertices = NULL,
	.vertex_normals = NULL,
	.faces = NULL,
	.text_coordinates = NULL,
	.rotation = {0,0,0},
	.scale= {1.0,1.0,1.0},
	.translation = {0,0,0},
	.normals = NULL
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{.x = -1, .y = -1, .z = -1},
	{.x = -1, .y = 1, .z = -1},
	{.x = 1, .y = 1, .z = -1},
	{.x = 1, .y = -1, .z = -1},
	{.x = 1, .y = 1, .z = 1},
	{.x = 1, .y = -1, .z = 1},
	{.x = -1, .y = 1, .z = 1},
	{.x = -1, .y = -1, .z = 1}
};

face_t cube_faces[N_CUBE_FACES] = {
	// front
	{.a = 1, .b = 2, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 1, .b = 3, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// right
	{.a = 4, .b = 3, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 4, .b = 5, .c = 6, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// back
	{.a = 6, .b = 5, .c = 7, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 6, .b = 7, .c = 8, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// left
	{.a = 8, .b = 7, .c = 2, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 8, .b = 2, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// top
	{.a = 2, .b = 7, .c = 5, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 2, .b = 5, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
	// bottom
	{.a = 6, .b = 8, .c = 1, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
	{.a = 6, .b = 1, .c = 4, .a_uv = { 0, 1 }, .b_uv = { 1, 0 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF }
};

void load_cube_mesh_data(void) {
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		vec3_t cube_vertex = cube_vertices[i];
		array_push(mesh.vertices, cube_vertex);
	}

	for (int i = 0; i < N_CUBE_FACES; i++) {
		face_t cube_face = cube_faces[i];
		array_push(mesh.faces, cube_face);
	}
}

void load_mesh_from_file(char* model) {
	FILE* mesh_file;
	errno_t error;
	error = fopen_s(&mesh_file,model, "r");
	if (error != 0) {
		return;
	}
	char line[1024];
	float vertex[3];
	int triangle[5];

	char* token;
	char* rest;

	int i;

	while (fgets(line, 1024, mesh_file)) {
		rest = line;
		token = strtok_s(line, " ", &rest);

		if (strcmp(token,"v") == 0) {
			i = 0;
			while ((token = strtok_s(rest, " ", &rest))) {
				vertex[i] = atof(token);
				i++;
			}
			vec3_t newVertex = {
				.x = vertex[0],
				.y = vertex[1],
				.z = vertex[2]
			};
			array_push(mesh.vertices, newVertex);

		}

		else if (strcmp(token, "vn") == 0) {
			i = 0;
			while ((token = strtok_s(rest, " ", &rest))) {
				vertex[i] = atof(token);
				i++;
			}
			vec3_t newVertex_normals = {
				.x = vertex[0],
				.y = vertex[1],
				.z = vertex[2]
			};
			
			array_push(mesh.vertex_normals, newVertex_normals);

		}

		else if (strcmp(token, "vt") == 0) {
			i = 0;
			while ((token = strtok_s(rest, " ", &rest))) {
				vertex[i] = atof(token);
				i++;
			}
			tex2_t text_coordinates = {
				.u = vertex[0],
				.v = 1-vertex[1], //Flipp the v, perhaps use as a option bool
			};

			array_push(mesh.text_coordinates, text_coordinates);

		}


		else if (strcmp(token, "f", 2) == 0) {
			i = 0;
			int vertex_indices[3];
			int texture_indices[3];
			int normal_indices[3];

			//TODO: Detta funkade inte heller f�r formatet som beh�vde l�sas
			//while ((token = strtok_s(rest, " ", &rest))) {
			//	
			//	triangle[i] = atoi(&token[0]); // vertex nr
			//	texture_indices[i] = atoi(&token[2]);
			//	i++;
			//	if (i > 2)
			//		continue;
			//}

			sscanf_s( 
				&rest, "%d/%d/%d %d/%d/%d %d/%d/%d",
				&vertex_indices[0], &texture_indices[0], &normal_indices[0],
				&vertex_indices[1], &texture_indices[1], &normal_indices[1],
				&vertex_indices[2], &texture_indices[2], &normal_indices[2]
			);

			face_t newFace = {
				.a = triangle[0]-1,
				.b = triangle[1]-1,
				.c = triangle[2]-1,
				.a_uv = mesh.text_coordinates[texture_indices[0]-1],
				.b_uv = mesh.text_coordinates[texture_indices[1]-1],
				.c_uv = mesh.text_coordinates[texture_indices[2]-1],
			};

			array_push(mesh.faces, newFace);

			vec3_t vector_ab = vec3_sub(mesh.vertices[newFace.b], mesh.vertices[newFace.a]);
			vec3_t vector_ac = vec3_sub(mesh.vertices[newFace.c], mesh.vertices[newFace.a]);

			vec3_normalize(&vector_ab);
			vec3_normalize(&vector_ac);

			vec3_t normal = vec3_cross(vector_ab, vector_ac); //order -> left handed coordinate system

			vec3_normalize(&normal);
		}



	}

	//fclose(&mesh_file);
	
}

void load_obj_file_data(char* filename) {
	FILE* file;
	errno_t error;
	error = fopen_s(&file,filename, "r");
	char line[1024];

	while (fgets(line, 1024, file)) {
		// Vertex information
		if (strncmp(line, "v ", 2) == 0) {
			vec3_t vertex;
			sscanf_s(&line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			array_push(mesh.vertices, vertex);
		}

		if (strncmp(line, "vt ", 2) == 0) {
			vec3_t vertex;
			sscanf_s(&line, "vt %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			tex2_t text_coordinates = {
				.u = vertex.x,
				.v = 1 - vertex.y, //Flipp the v, perhaps use as a option bool
			};
			array_push(mesh.text_coordinates, text_coordinates);
		}

		// Face information
		if (strncmp(line, "f ", 2) == 0) {
			int vertex_indices[3];
			int texture_indices[3];
			int normal_indices[3];
			sscanf_s(
				&line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&vertex_indices[0], &texture_indices[0], &normal_indices[0],
				&vertex_indices[1], &texture_indices[1], &normal_indices[1],
				&vertex_indices[2], &texture_indices[2], &normal_indices[2]
			);
			face_t face = {
				.a = vertex_indices[0] - 1,
				.b = vertex_indices[1] - 1,
				.c = vertex_indices[2] - 1,
				.a_uv = mesh.text_coordinates[texture_indices[0] - 1],
				.b_uv = mesh.text_coordinates[texture_indices[1] - 1],
				.c_uv = mesh.text_coordinates[texture_indices[2] - 1],
			};
			array_push(mesh.faces, face);
		}
	}
}
