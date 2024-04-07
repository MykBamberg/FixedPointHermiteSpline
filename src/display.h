#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdlib.h>

typedef struct
{
    int x;
    int y;
} point;

point addPoints (point a, point b);

void displayRGBPixelBuffer (Uint32* pixelBuffer, int width, int height, char* title);

void drawLine (Uint32* pixelBuffer, int width, int height, Uint32 color, point beginning, point end);
void drawPointCross (Uint32* pixelBuffer, int width, int height, Uint32 color, point position);
void mergeColorsLighten (Uint32* pixel, Uint32 color);

unsigned int* createHeatMapBuffer (int width, int height);
void plotHeatMapPoint 
(
    Uint32* pixelBuffer, int width, int height, 
    point p, unsigned int* heatMapBuffer, 
    const Uint32* heatMapColors, int colorWeight, int colorCount, 
    const uint32_t* pointShape, int pointDistanceFromCenter
);

#endif
