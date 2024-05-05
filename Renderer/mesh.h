#pragma once
#define MESH_H
#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES 12

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
	vec3_t* vertices;	//dynamic array of vertices
	face_t* faces;		//dynamic array of faces
	vec3_t rotation;	//x,y,z rotation values
	vec3_t scale;
	vec3_t translation;
	tex2_t* text_coordinates; // u,v (w excluded)
	vec3_t* vertex_normals;
	vec3_t* normals;
} mesh_t;

extern mesh_t mesh;

void load_cube_mesh_data(void);

void load_mesh_from_file(char* model);
void load_obj_file_data(char* filename);
