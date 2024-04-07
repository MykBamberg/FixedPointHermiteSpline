#include <SDL2/SDL.h>
#include <stdlib.h>

typedef struct
{
    int x;
    int y;
} point;

point addPoints(point a, point b);
void displayRGBPixelBuffer(Uint32* pixelBuffer, int width, int height, char* title);
void drawLine(Uint32* pixelBuffer, int width, int height, Uint32 color, point beginning, point end);
void drawPointCross(Uint32* pixelBuffer, int width, int height, Uint32 color, point position);
