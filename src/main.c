/*
  A simple fixed point Hermite Spline rendering application!
  See LICENSE for license details.
*/

#include "config.h"
#include "display.h"


int main() {
    Uint32* pixelBuffer = createPixelBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, COL_BG);
    unsigned int* heatMapBuffer = createHeatMapBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    /* p: start/end points, v: start/end velocities */
    
    const point p0 = {  50,  50};
    const point p1 = { 800, 350};
    const point v0 = { 110, 200};
    const point v1 = {-100,  50};
    
    /* Drawing the spline */
    for(int t = 0; t <= ONE; t++) {
        int t2 = t * t / ONE;
        int t3 = t2 * t / ONE;
        point p = {0, 0};
        
        p.x += (2 * t3 - 3 * t2 + ONE)    * p0.x;
        p.x += (t3 - 2 * t2 + t)          * v0.x;
        p.x += (-2 * t3 + 3 * t2)         * p1.x;
        p.x += (t3 - t2)                  * v1.x;
        
        p.y += (2 * t3 - 3 * t2 + ONE)    * p0.y;
        p.y += (t3 - 2 * t2 + t)          * v0.y;
        p.y += (-2 * t3 + 3 * t2)         * p1.y;
        p.y += (t3 - t2)                  * v1.y;
        
        p.x /= ONE;
        p.y /= ONE;
        
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
