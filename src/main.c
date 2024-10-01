/*
  A simple fixed point Hermite Spline rendering application
  See LICENSE for license details.
*/

#include "config.h"
#include "display.h"
#include "fixed_point.h"
#include <math.h>
#include <assert.h>

static point hermite(int32_t t, const point p0, const point p1, const point v0, const point v1) {
    int32_t t2 = t * t >> FRACTION_BITS;
    int32_t t3 = t2 * t >> FRACTION_BITS;
    point p = {0};

    p.x += (2 * t3 - 3 * t2 + FIXED_POINT_ONE) * p0.x;
    p.x += (t3 - 2 * t2 + t) * v0.x;
    p.x += (-2 * t3 + 3 * t2) * p1.x;
    p.x += (t3 - t2) * v1.x;

    p.y += (2 * t3 - 3 * t2 + FIXED_POINT_ONE) * p0.y;
    p.y += (t3 - 2 * t2 + t) * v0.y;
    p.y += (-2 * t3 + 3 * t2) * p1.y;
    p.y += (t3 - t2) * v1.y;

    return p;
}

FILE* csv_init(void) {
    const char* filename = "out.csv";
    FILE* csv_fp = fopen(filename, "w");
    assert(csv_fp != NULL);

    return csv_fp;
}

int main(void) {
    uint32_t* pixel_buffer = create_pixel_buffer(SCREEN_WIDTH, SCREEN_HEIGHT, COL_BG);
    int32_t* heat_map_buffer = create_heat_map_buffer(SCREEN_WIDTH, SCREEN_HEIGHT);

    FILE* csv_fp = csv_init();
    fprintf(csv_fp, "distance per step; step size\n");

    /* p: start/end points, v: start/end velocities */

    const point p0 = {  50,  50};
    const point p1 = { 800, 350};
    const point v0 = { 110, 200};
    const point v1 = {-500,  50};

    /* Drawing the spline */

    /* Estimate optimal step size */
    int32_t step_size = FIXED_POINT_ONE / (abs(p0.x - p1.x) + abs(p0.y - p1.y));
    point last_p = hermite(0, p0, p1, v0, v1);

    for(int32_t t = step_size; t <= FIXED_POINT_ONE; t += step_size) {
        point p = hermite(t, p0, p1, v0, v1);

        int32_t error = (
            (p.x - last_p.x) * (p.x - last_p.x) +
            (p.y - last_p.y) * (p.y - last_p.y)) -
            (FIXED_POINT_ONE * FIXED_POINT_ONE);

        constexpr int volatility = 3;
        step_size -= (error) >> (FRACTION_BITS * 2 - volatility);

        fprintf(csv_fp, "%f; %d\n",
                hypot((double)p.x - (double)last_p.x, (double)p.y - (double)last_p.y) / (double)FIXED_POINT_ONE,
                step_size);

        last_p = p;

        p.x >>= FRACTION_BITS;
        p.y >>= FRACTION_BITS;

        if (p.x >= 0 && p.x < SCREEN_WIDTH && p.y >= 0 && p.y < SCREEN_HEIGHT) {
            plot_heat_map_point(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, p, heat_map_buffer);
        }
    }

    { /* linear interpolation between p0 and p1 */
        if (SHOW_LINES) draw_line(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p0, p1);
        if (SHOW_POINTS) draw_point_cross(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, p0);
        if (SHOW_POINTS) draw_point_cross(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, p1);
    }
    { /* velocity vectors at start and end points */
        if (SHOW_LINES) draw_line(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p0, add_points(p0, v0));
        if (SHOW_LINES) draw_line(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p1, add_points(p1, v1));
        if (SHOW_POINTS) draw_point_cross(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, add_points(p0, v0));
        if (SHOW_POINTS) draw_point_cross(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, add_points(p1, v1));
    }

    display_rgb_pixel_buffer(pixel_buffer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    free(pixel_buffer);
    free(heat_map_buffer);
    fclose(csv_fp);

    return 0;
}
