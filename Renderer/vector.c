#include "vector.h"
#include <math.h>

vec2_t addvector2d(vec2_t v1, vec2_t v2)
{
	float x = v1.x + v2.x; //pointers some place?
	float y = v2.x + v2.x;

	vec2_t v3 = { x,y };

	return v3;
}

vec3_t addvector3d(vec3_t v1, vec3_t v2)
{
	float x = v1.x + v2.x;
	float y = v1.y + v2.y;
	float z = v1.z + v2.z;

	vec3_t v3 = { x,y,z };

	return v3;
}

vec2_t project(vec3_t v1, int fov) {
	vec2_t v2 = {
		.x = (fov*v1.x) / v1.z,
		.y = (fov*v1.y) / v1.z
	};

	return v2;
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