#include "matrix.h"
#include "vector.h"
#include <math.h>

mat4_t mat4_identity(void)
{
	mat4_t m = { {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
	} };

	return m;
}

mat4_t mat4_scale(float sx, float sy, float sz)
{
	mat4_t m = mat4_identity();

	m.m[0][0] = sx;
	m.m[1][1] = sy;
	m.m[2][2] = sz;

	return m;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
	vec4_t result;

	result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
	result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
	result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
	result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

	return result;
}



mat4_t mat4_translate(float x, float y, float z)
{	
	mat4_t m = mat4_identity();
	m.m[0][3] = x;
	m.m[1][3] = y;
	m.m[2][3] = z;


	return m;
}

mat4_t mat4_rotate(float a, int axis) {

	mat4_t m = mat4_identity();

	switch (axis)
	{
	case 0:
		//rotation around x-axis

		m.m[1][1] = cos(a);
		m.m[2][1] = sin(a);
		m.m[1][2] = -sin(a);
		m.m[2][2] = cos(a);
		break;

	case 1:
		//rotation around y-axis
		m.m[0][0] = cos(a);
		m.m[2][0] = -sin(a);
		m.m[0][2] = sin(a);
		m.m[2][2] = cos(a);
		break;

	case 2:

		//Rotation around z-axis

		m.m[0][0] = cos(a);
		m.m[1][0] = sin(a);
		m.m[0][1] = -sin(a);
		m.m[1][1] = cos(a);
		break;


	default:
		break;
	}

	return m;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b)
{
	mat4_t m;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
		}
	}

	return m;
}

mat4_t mat4_make_perspective(float fov, float aspect, float znear, float zfar)
{
	// viewport tranformation Normal Device Coordinates to Scren Coordinates
    mat4_t m = {{{ 0 }}};
    m.m[0][0] = aspect * (1 / tan(fov / 2));
    m.m[1][1] = 1 / tan(fov / 2);
    m.m[2][2] = zfar / (zfar - znear);
    m.m[2][3] = (-zfar * znear) / (zfar - znear);
    m.m[3][2] = 1.0;

	return m;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v)
{
	//transform to screen space coordinates
	vec4_t result = mat4_mul_vec4(mat_proj, v);

	//Project using the projection divide

	if (result.w != 0.0) { //the original z-value

		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
		return result;
	}
}

mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up) {
    // Compute the forward (z), right (x), and up (y) vectors
    vec3_t z = vec3_sub(target, eye);
    vec3_normalize(&z);
    vec3_t x = vec3_cross(up, z);
    vec3_normalize(&x);
    vec3_t y = vec3_cross(z, x);

    // | x.x   x.y   x.z  -dot(x,eye) |
    // | y.x   y.y   y.z  -dot(y,eye) |
    // | z.x   z.y   z.z  -dot(z,eye) |
    // |   0     0     0            1 |
    mat4_t view_matrix = {{
        { x.x, x.y, x.z, -vec3_dot(x, eye) },
        { y.x, y.y, y.z, -vec3_dot(y, eye) },
        { z.x, z.y, z.z, -vec3_dot(z, eye) },
        {   0,   0,   0,                 1 }
    }};
    return view_matrix;
}