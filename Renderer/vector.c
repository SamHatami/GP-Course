#include "vector.h"
#include <math.h>

vec2_t vec2_add(vec2_t v1, vec2_t v2)
{
	float x = v1.x + v2.x; //pointers some place?
	float y = v2.x + v2.x;

	vec2_t v3 = { x,y };

	return v3;
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2) {

	float x = v1.x - v2.x; //pointers some place?
	float y = v2.x - v2.x;

	vec2_t v3 = { x,y };

	return v3;
}

vec2_t vec2_scalar(vec2_t v1, float a)
{
	vec2_t v = { .x = v1.x * a, .y = v1.y * a };
	return v;
}

vec2_t vec4_to_vec2(vec4_t v1)
{
	vec2_t v = { .x = v1.x, .y = v1.y };

	return v;
}

vec2_t vec2_div(vec2_t v1, float a) {

	vec2_t v = { .x = v1.x / a, .y = v1.y / a };
	return v;

}


float vec2_length(vec2_t v) {

	return sqrt(v.x * v.x + v.y * v.y);
}

float vec2_dot(vec2_t v1, vec2_t v2) {

	return (v1.x * v2.x + v1.y * v2.y);
}

vec2_t project(vec3_t v1, int fov) {
	vec2_t v2 = {
		.x = (fov*v1.x) / v1.z,
		.y = (fov*v1.y) / v1.z
	};

	return v2;
}

void vec2_normalize(vec2_t* v) {

	float length = sqrt(v->x * v->x + v->y * v->y);
	v->x / length;
	v->y / length;
}

float vec3_length(vec3_t v) {

	return sqrt(v.x * v.x + v.y * v.y + v.z*v.z);
}

vec4_t vec3_to_vec4(vec3_t v)
{
	vec4_t v4 = { v.x,v.y,v.z,1.0 };
	return v4;
}

vec3_t vec4_to_vec3(vec4_t v)
{
	vec3_t v3 = { v.x,v.y,v.z };
	return v3;
}

void vec3_normalize(vec3_t* v) {
	float length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->x /= length;
	v->y /= length;
	v->z /= length;
}

float vec3_dot(vec3_t v1, vec3_t v2) {

	return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z*v2.z);
}

vec3_t vec3_add(vec3_t* v1, vec3_t* v2)
{
	float x = v1->x + v2->x;
	float y = v1->y + v2->y;
	float z = v1->z + v2->z;

	vec3_t v3 = { x,y,z };

	return v3;
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2)
{
	float x = v1.x - v2.x;
	float y = v1.y - v2.y;
	float z = v1.z - v2.z;

	vec3_t v3 = { x,y,z };

	return v3;
}

vec3_t vec3_scalar(vec3_t v1, float a)
{
	vec3_t v = { .x = v1.x * a, .y = v1.y * a, .z = v1.z*a };
	return v;
}

vec3_t vec3_div(vec3_t v1, float a) {

	vec3_t v = { .x = v1.x / a, .y = v1.y / a, .z= v1.z/a };
	return v;

}

vec3_t vec3_cross(vec3_t v1, vec3_t v2) {
	vec3_t n = {
		.x = v1.y*v2.z - v1.z*v2.y,
		.y = v1.z*v2.x - v1.x*v2.z,
		.z = v1.x*v2.y - v1.y*v2.x
	};

	return n;
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated_v = {
		.x = v.x,
		.y = v.y * cos(angle) - v.z * sin(angle),
		.z = v.y * sin(angle) + v.z * cos(angle)
	};

	return rotated_v;
}
vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_v = {
		.x = v.x * cos(angle) - v.z * sin(angle),
		.y = v.y,
		.z = v.x * sin(angle) + v.z * cos(angle)
	};

	return rotated_v;
}
vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_v = {
		.x = v.x * cos(angle) - v.y * sin(angle),
		.y = v.x * sin(angle) + v.y * cos(angle),
		.z = v.z
	};

	return rotated_v;
}