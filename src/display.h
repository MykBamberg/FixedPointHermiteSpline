#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <inttypes.h>

typedef struct {
    int32_t x;
    int32_t y;
} point;

point add_points(point a, point b);

uint32_t* create_pixel_buffer(int32_t width, int32_t height, Uint32 defaultColor);
void display_rgb_pixel_buffer(uint32_t* pixelBuffer, int32_t width, int32_t height, char* title);

void draw_line(uint32_t* pixelBuffer, int32_t width, int32_t height, Uint32 color, point beginning, point end);
void draw_point_cross(uint32_t* pixelBuffer, int width, int height, Uint32 color, point position);
void merge_colors_ligthen(uint32_t* pixel, Uint32 color);

int32_t* create_heat_map_buffer(int32_t width, int32_t height);
void plot_heat_map_point(uint32_t* pixelBuffer, int32_t width, int32_t height, point p, int32_t* heatMapBuffer);

#endif
