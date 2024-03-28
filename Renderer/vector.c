#include "vector.h"

vec2_t addvector2d(vec2_t v1, vec2_t v2)
{

	float x = v1.x + v2.x; //pointers some place?
	float y = v2.x + v2.x;

	vec2_t v3 = { x,y };

	return v3;
}

vec3_t addvector3d(vec3_t v1, vec3_t v2)
{
}
