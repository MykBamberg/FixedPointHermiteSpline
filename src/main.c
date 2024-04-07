/*
  A simple fixed point Hermite Spline rendering application!
  See LICENSE for license details.
*/

#include "config.h"
#include "display.h"


int main() {
    Uint32* pixelBuffer = malloc(sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT);
    
    /* Set BG color */
    
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        pixelBuffer[i] = COL_BG;
    }
    
    point p0, p1, v0, v1; // p: start/end points, v: start/end velocities
    
    p0.x =  100;  p0.y =  50; 
    p1.x =  400;  p1.y =  250;
    v0.x = -90 ;  v0.y =  450;
    v1.x = -300;  v1.y =  250;
    
    /* linear interpolation between p0 and p1 */
    {
        drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p0, p1);
        drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, p0);
        drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, p1);
    }
    /* velocity vectors at start and end points */
    {
        drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p0, addPoints(p0, v0));
        drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_LINE, p1, addPoints(p1, v1));
        drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, addPoints(p0, v0));
        drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, COL_POINT, addPoints(p1, v1));
    }
    
    /* Drawing the spline */
    for(int t = 0; t <= ONE; t++)
    {
        int t2 = t * t / ONE;
        int t3 = t2 * t / ONE;
        
        int x = 0, y = 0;
        
        x += (2 * t3 - 3 * t2 + ONE)    * p0.x;
        x += (t3 - 2 * t2 + t)          * v0.x;
        x += (-2 * t3 + 3 * t2)         * p1.x;
        x += (t3 - t2)                  * v1.x;
        
        y += (2 * t3 - 3 * t2 + ONE)    * p0.y;
        y += (t3 - 2 * t2 + t)          * v0.y;
        y += (-2 * t3 + 3 * t2)         * p1.y;
        y += (t3 - t2)                  * v1.y;
        
        x /= ONE;
        y /= ONE;
        
        if ( x >= 0 && x < SCREEN_WIDTH &&
            y >= 0 && y < SCREEN_HEIGHT )
        {
            pixelBuffer[x + SCREEN_WIDTH * y] = COL_FG;
        }
    }
    
    displayRGBPixelBuffer(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    free(pixelBuffer);
    
    return 0;
}
