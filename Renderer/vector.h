#pragma once
typedef struct 
{
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

vec2_t addvector2d(vec2_t v1, vec2_t v2);
vec3_t addvector3d(vec3_t v1, vec3_t v2);
