/*
  A simple fixed point Hermite Spline rendering application!
  See LICENSE for license details.
*/

#include "config.h"
#include "display.h"


int main() {
    Uint32* pixelBuffer = malloc(sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT);
    unsigned int* heatMapBuffer = createHeatMapBuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    /* Set BG color */
    
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        pixelBuffer[i] = COL_BG;
    }
    
    point p0, p1, v0, v1; // p: start/end points, v: start/end velocities
    
    p0.x =  100;  p0.y =  50; 
    p1.x =  400;  p1.y =  250;
    v0.x = -90 ;  v0.y =  512;
    v1.x = -300;  v1.y =  250;
    
    /* Drawing the spline */
    for(int t = 0; t <= ONE; t++)
    {
        int t2 = t * t / ONE;
        int t3 = t2 * t / ONE;
        
        point p;
        p.x = 0; p.y = 0;
        
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
        
        if ( p.x >= 0 && p.x < SCREEN_WIDTH &&
            p.y >= 0 && p.y < SCREEN_HEIGHT )
        {
            plotHeatMapPoint
            (
                pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, 
                p, heatMapBuffer, HeatMapColors, 
                HEAT_MAP_COLOR_WEIGHT, HEAT_MAP_COLOR_COUNT, 
                HeatMapPointShape, HEAT_MAP_POINT_SHAPE_DISTANCE_FROM_CENTER
            );
        }
    }
    
    /* linear interpolation between p0 and p1 */
    {
        if (SHOW_LINES) drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p0, p1);
        if (SHOW_POINTS) drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, p0);
        if (SHOW_POINTS) drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, p1);
    }
    /* velocity vectors at start and end points */
    {
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
