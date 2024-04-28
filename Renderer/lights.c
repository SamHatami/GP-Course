#include "lights.h"
#include <stdint.h>


uint32_t light_apply_intensity(uint32_t original_color, float percentage_factor)
{
    if (percentage_factor < 0) percentage_factor = 0;
    if (percentage_factor > 1) percentage_factor = 1;
         
    uint32_t a = (original_color & 0xFF000000);
    uint32_t r = (original_color & 0x00FF0000) * percentage_factor;
    uint32_t g = (original_color & 0x0000FF00) * percentage_factor;
    uint32_t b = (original_color & 0x000000FF) * percentage_factor;

    uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

    return new_color;
}

void interpolate_colors(uint32_t color[])
{
    int steps = 32;
    uint32_t colorband[32];

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < steps; j++)
        {
            uint32_t a = (color[i] & 0xFF000000);
            uint32_t r = (color[i] & 0x00FF0000) * 1 / steps;
            uint32_t g = (color[i] & 0x0000FF00) * 1 / steps;
            uint32_t b = (color[i] & 0x000000FF) * 1 / steps;

            uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
        }

    }


}


