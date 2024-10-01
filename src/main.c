/*
  A simple fixed point Hermite Spline rendering application
  See LICENSE for license details.
*/

#include "config.h"
#include "display.h"
#include "fixed_point.h"
#include <math.h>

point hermite(int t, const point p0, const point p1, const point v0, const point v1) {
    int t2 = t * t >> FRACTION_BITS;
    int t3 = t2 * t >> FRACTION_BITS;
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

int main() {
    Uint32* pixelBuffer = createPixelBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, COL_BG);
    unsigned int* heatMapBuffer = createHeatMapBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    /* p: start/end points, v: start/end velocities */
    
    const point p0 = {  50,  50};
    const point p1 = { 800, 350};
    const point v0 = { 110, 200};
    const point v1 = {-500,  50};
    
    /* Drawing the spline */
    int step_size = FIXED_POINT_ONE / (abs(p0.x - p1.x) + abs(p0.y - p1.y));
    point last_p = hermite(0, p0, p1, v0, v1);

    for(int t = step_size; t <= FIXED_POINT_ONE; t += step_size) {
        point p = hermite(t, p0, p1, v0, v1);

        int error = (
            (p.x - last_p.x) * (p.x - last_p.x) +
            (p.y - last_p.y) * (p.y - last_p.y)) -
            (FIXED_POINT_ONE * FIXED_POINT_ONE);

        const int volatility = 3;
        step_size -= (error) >> (FRACTION_BITS * 2 - volatility);

        /* printf("error: %f; step_size: %d\n", sqrtf(((float)error + FIXED_POINT_ONE * FIXED_POINT_ONE) / FIXED_POINT_ONE / FIXED_POINT_ONE), step_size); */

        last_p = p;

        p.x >>= FRACTION_BITS;
        p.y >>= FRACTION_BITS;
       
        if (p.x >= 0 && p.x < SCREEN_WIDTH && p.y >= 0 && p.y < SCREEN_HEIGHT) {
            plotHeatMapPoint (
                pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, 
                p, heatMapBuffer, HeatMapColors, 
                HEAT_MAP_COLOR_WEIGHT, HEAT_MAP_COLOR_COUNT, 
                HeatMapPointShape, HEAT_MAP_POINT_SHAPE_DISTANCE_FROM_CENTER
            );
        }
    }
    
    { /* linear interpolation between p0 and p1 */
        if (SHOW_LINES) drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p0, p1);
        if (SHOW_POINTS) drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, p0);
        if (SHOW_POINTS) drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, p1);
    }
    { /* velocity vectors at start and end points */
        if (SHOW_LINES) drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p0, addPoints(p0, v0));
        if (SHOW_LINES) drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p1, addPoints(p1, v1));
        if (SHOW_POINTS) drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, addPoints(p0, v0));
        if (SHOW_POINTS) drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, addPoints(p1, v1));
    }
    
    displayRGBPixelBuffer(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    free(pixelBuffer);
    free(heatMapBuffer);
    
    return 0;
}
