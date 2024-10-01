#include "display.h"
#include "config.h"

#include <SDL2/SDL_stdinc.h>
#include <assert.h>
#include <stdlib.h>

uint32_t* create_pixel_buffer(int32_t width, int32_t height, Uint32 default_color) {
    uint32_t* pixel_buffer;
    pixel_buffer = calloc((unsigned)(width * height), sizeof(uint32_t));
    assert(pixel_buffer != NULL);

    for(int32_t i = 0; i < width * height; i++) {
        pixel_buffer[i] = default_color;
    }

    return pixel_buffer;
}

void display_rgb_pixel_buffer(uint32_t* pixel_buffer, int32_t width, int32_t height, char* title) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    SDL_UpdateTexture(texture, NULL, pixel_buffer, width * (int32_t)sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Event e;
    while (1) {
        if (SDL_PollEvent(&e) && e.type == SDL_QUIT) {
            break;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void draw_line(uint32_t* pixel_buffer, int32_t width, int32_t height, Uint32 color, point beginning, point end) {
    int32_t dx = abs(end.x - beginning.x);
    int32_t sx = beginning.x < end.x ? 1 : -1;
    int32_t dy = -abs(end.y - beginning.y);
    int32_t sy = beginning.y < end.y ? 1 : -1;
    int32_t err = dx + dy;
    int32_t e2;

    int32_t x = beginning.x, y = beginning.y;

    while (x != end.x || y != end.y) {
        if ( x >= 0 && x < width && y >= 0 && y < height ) {
            merge_colors_ligthen(&pixel_buffer[x + width * y], color);
        }

        e2 = err * 2;
        err += dy * (e2 > dy);
        x   += sx * (e2 > dy);
        err += dx * (e2 < dx);
        y   += sy * (e2 < dx);
    }

    if ( x >= 0 && x < width && y >= 0 && y < height ) {
        merge_colors_ligthen(&pixel_buffer[x + width * y], color);
    }
}

point add_points(point a, point b) {
    point p;
    p.x = a.x + b.x;
    p.y = a.y + b.y;
    return p;
}

void draw_point_cross(uint32_t* pixel_buffer, int32_t width, int32_t height, Uint32 color, point position) {
    int32_t radius = 6;

    point top_left = position, top_right = position, bottom_left = position, bottom_right = position;

    top_left.x -= radius;
    top_left.y -= radius;

    top_right.x += radius;
    top_right.y -= radius;

    bottom_left.x -= radius;
    bottom_left.y += radius;

    bottom_right.x += radius;
    bottom_right.y += radius;

    draw_line(pixel_buffer, width, height, color, bottom_left, top_right);
    draw_line(pixel_buffer, width, height, color, top_left, bottom_right);
}

void merge_colors_ligthen (uint32_t* pixel, Uint32 color) {
    uint8_t r1, g1, b1, r2, g2, b2;
    b1 = 0xff & ((*pixel) >>  0);
    g1 = 0xff & ((*pixel) >>  8);
    r1 = 0xff & ((*pixel) >> 16);

    b2 = 0xff & (color >>  0);
    g2 = 0xff & (color >>  8);
    r2 = 0xff & (color >> 16);

    (*pixel)  = (uint32_t)(b1 > b2 ? b1 : b2) << 0;
    (*pixel) += (uint32_t)(g1 > g2 ? g1 : g2) << 8;
    (*pixel) += (uint32_t)(r1 > r2 ? r1 : r2) << 16;
}

int32_t* create_heat_map_buffer(int32_t width, int32_t height) {
    int32_t* heat_map_buffer = calloc((unsigned)(width * height), sizeof(unsigned int));
    assert(heat_map_buffer != NULL);

    return heat_map_buffer;
}

void plot_heat_map_point (uint32_t* pixel_buffer, int32_t width, int32_t height, point p, int32_t* heat_map_buffer) {
    constexpr int32_t radius = HEAT_MAP_POINT_SHAPE_DISTANCE_FROM_CENTER;

    for(int32_t x_offset = -radius; x_offset <= radius; x_offset++) {
        for(int32_t y_offset = -radius; y_offset <= radius; y_offset++) {
            /* out of bounds */
            if(p.x + x_offset < 0 || p.x + x_offset >= width || p.y + y_offset < 0 || p.y + y_offset >= height) {
                continue;
            }

            int32_t index = p.x + x_offset + width * (p.y + y_offset);

            /* adds value from point shape to heat map buffer */
            heat_map_buffer[index] += heat_map_point_shape[radius + x_offset + (1 + 2 * radius) * (radius + y_offset)];

            int32_t heat = heat_map_buffer[index];

            heat /= HEAT_MAP_COLOR_WEIGHT;
            heat = heat >= HEAT_MAP_COLOR_COUNT ? HEAT_MAP_COLOR_COUNT - 1 : heat;

            pixel_buffer[index] = heat_map_colors[heat];
        }
    }
}
