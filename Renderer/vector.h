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

float vec2_length(vec2_t v);
float vec2_dot(vec2_t v1, vec2_t v2);
vec2_t vec2_add(vec2_t v1, vec2_t v2);
vec2_t vec2_sub(vec2_t v1, vec2_t v2);
vec2_t vec2_scalar(vec2_t v1, float a);
vec2_t vec2_div(vec2_t v1, float a);
void vec2_normalize(vec2_t* v);

vec2_t project(vec3_t v1, int fov);

float vec3_dot(vec3_t v1, vec3_t v2);
vec3_t vec3_add(vec3_t v1, vec3_t v2);
vec3_t vec3_sub(vec3_t v1, vec3_t v2);
vec3_t vec3_cross(vec3_t v1, vec3_t v2);
vec3_t vec3_scalar(vec3_t v1, float a);
vec3_t vec3_div(vec3_t v1, float a);
vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);
void vec3_normalize(vec3_t* v);
float vec3_length(vec3_t v);

