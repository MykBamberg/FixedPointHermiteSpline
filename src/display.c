#include "display.h"

#include <SDL2/SDL_stdinc.h>
#include <assert.h>
#include <stdlib.h>

Uint32* createPixelBuffer(int width, int height, Uint32 defaultColor) {
    Uint32* pixelBuffer;
    pixelBuffer = calloc((unsigned)(width * height), sizeof(Uint32));
    assert(pixelBuffer != NULL);
    
    for(int i = 0; i < width * height; i++) {
        pixelBuffer[i] = defaultColor;
    }
    
    return pixelBuffer;
}

void displayRGBPixelBuffer(Uint32* pixelBuffer, int width, int height, char* title) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    
    SDL_UpdateTexture(texture, NULL, pixelBuffer, width * (int)sizeof(Uint32));
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

void drawLine(Uint32* pixelBuffer, int width, int height, Uint32 color, point beginning, point end) {
    int dx = abs(end.x - beginning.x);
    int sx = beginning.x < end.x ? 1 : -1;
    int dy = -abs(end.y - beginning.y);
    int sy = beginning.y < end.y ? 1 : -1;
    int err = dx + dy;
    int e2;
    
    int x = beginning.x, y = beginning.y;
    
    while (x != end.x || y != end.y) {
        if ( x >= 0 && x < width && y >= 0 && y < height ) {
            mergeColorsLighten(&pixelBuffer[x + width * y], color);
        }
        
        e2 = err * 2;
        err += dy * (e2 > dy);
        x   += sx * (e2 > dy);
        err += dx * (e2 < dx);
        y   += sy * (e2 < dx);
    }
    
    if ( x >= 0 && x < width && y >= 0 && y < height ) {
        mergeColorsLighten(&pixelBuffer[x + width * y], color);
    }
}

point addPoints(point a, point b) {
    point p; 
    p.x = a.x + b.x; 
    p.y = a.y + b.y; 
    return p;
}

void drawPointCross(Uint32* pixelBuffer, int width, int height, Uint32 color, point position) {
    int distanceFromCenter = 6;
    
    point topLeftPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;
    
    memcpy(&topLeftPoint, &position, sizeof(point)); memcpy(&topRightPoint, &position, sizeof(point));
    memcpy(&bottomLeftPoint, &position, sizeof(point)); memcpy(&bottomRightPoint, &position, sizeof(point));
    
    topLeftPoint.x -= distanceFromCenter;
    topLeftPoint.y -= distanceFromCenter;
    
    topRightPoint.x += distanceFromCenter;
    topRightPoint.y -= distanceFromCenter;
    
    bottomLeftPoint.x -= distanceFromCenter;
    bottomLeftPoint.y += distanceFromCenter;
    
    bottomRightPoint.x += distanceFromCenter;
    bottomRightPoint.y += distanceFromCenter;
    
    drawLine(pixelBuffer, width, height, color, bottomLeftPoint, topRightPoint);
    drawLine(pixelBuffer, width, height, color, topLeftPoint, bottomRightPoint);
}

void mergeColorsLighten (Uint32* pixel, Uint32 color) {
    uint8_t r1, g1, b1, r2, g2, b2;
    b1 = 0xff & ((*pixel) >>  0);
    g1 = 0xff & ((*pixel) >>  8);
    r1 = 0xff & ((*pixel) >> 16);
    
    b2 = 0xff & (color >>  0);
    g2 = 0xff & (color >>  8);
    r2 = 0xff & (color >> 16);
    
    (*pixel)  = (Uint32)(b1 > b2 ? b1 : b2) << 0;
    (*pixel) += (Uint32)(g1 > g2 ? g1 : g2) << 8;
    (*pixel) += (Uint32)(r1 > r2 ? r1 : r2) << 16;
}

unsigned int* createHeatMapBuffer (int width, int height) {
    unsigned int* heatMapBuffer = calloc((unsigned)(width * height), sizeof(unsigned int));
    assert(heatMapBuffer != NULL);
    
    return heatMapBuffer;
}

void plotHeatMapPoint (
    Uint32* pixelBuffer, int width, int height, 
    point p, unsigned int* heatMapBuffer, 
    const Uint32* heatMapColors, int colorWeight, int colorCount, 
    const uint32_t* pointShape, int pointDistanceFromCenter)
{
    for(int xOffset = -pointDistanceFromCenter; xOffset <= pointDistanceFromCenter; xOffset++) {
        for(int yOffset = -pointDistanceFromCenter; yOffset <= pointDistanceFromCenter; yOffset++) {
            /* out of bounds */
            if(p.x + xOffset < 0 || p.x + xOffset >= width || p.y + yOffset < 0 || p.y + yOffset >= height) {
                continue;
            }
            
            int index = p.x + xOffset + width * (p.y + yOffset);
            
            /* adds value from point shape to heat map buffer */
            heatMapBuffer[index] += pointShape[pointDistanceFromCenter + xOffset + (1 + 2 * pointDistanceFromCenter) * (pointDistanceFromCenter + yOffset)];
            
            unsigned int heat = heatMapBuffer[index];
            
            heat /= (unsigned)colorWeight;
            heat = heat >= (unsigned)colorCount ? (unsigned)colorCount - 1 : heat;
            
            pixelBuffer[index] = heatMapColors[heat];
        }
    }
}
