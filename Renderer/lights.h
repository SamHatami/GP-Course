#pragma once
#include "vector.h"
#include <stdint.h>
typedef struct
{
	vec3_t direction;
	//float intensity;
} light_t;

uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor);

void interpolate_colors(uint32_t color[],int steps);

