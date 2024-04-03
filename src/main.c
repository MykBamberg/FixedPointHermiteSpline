/*
  A simple fixed point Hermite Spline rendering application!
  See LICENSE for license details.
*/

#include <SDL2/SDL.h>
#include <stdlib.h>

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define WINDOW_TITLE "Hermite Spline"

/* Catppuccin inspired colors */
#define C_BKG   0x1E1E2F
#define C_FG    0xCED6F2
#define C_HIGH  0xC196F3
#define C_XRTA  0x44526A

/* Fixed point math one */
#define ONE (1 << 12)

typedef struct
{
    int x;
    int y;
} point;

point addPoints(point a, point b) 
{
    point p; 
    p.x = a.x + b.x; 
    p.y = a.y + b.y; 
    return p;
}

void displayRGBPixelBuffer(Uint32* pixelBuffer, int width, int height);
void drawLine(Uint32* pixelBuffer, int width, int height, Uint32 color, point beginning, point end);
void drawPointCross(Uint32* pixelBuffer, int width, int height, Uint32 color, point position);

int main() {
    Uint32* pixelBuffer = malloc(sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT);
    
    /* Set BG color */
    
    for(int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        pixelBuffer[i] = C_BKG;
    }
    
    point p0, p1, v0, v1; // p: start/end points, v: start/end velocities
    
    p0.x =  100;  p0.y =  50; 
    p1.x =  400;  p1.y =  250;
    v0.x = -90 ;  v0.y =  450;
    v1.x = -300;  v1.y =  250;
    
    /* linear interpolation between p0 and p1 */
    {
        drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, C_XRTA, p0, p1);
        drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, C_HIGH, p0);
        drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, C_HIGH, p1);
    }
    /* velocity vectors at start and end points */
    {
        drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, C_XRTA, p0, addPoints(p0, v0));
        drawLine(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, C_XRTA, p1, addPoints(p1, v1));
        drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, C_HIGH, addPoints(p0, v0));
        drawPointCross(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT, C_HIGH, addPoints(p1, v1));
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
            pixelBuffer[x + SCREEN_WIDTH * y] = C_FG;
        }
    }
    
    displayRGBPixelBuffer(pixelBuffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    free(pixelBuffer);
    
    return 0;
}

void displayRGBPixelBuffer(Uint32* pixelBuffer, int width, int height)
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    
    SDL_UpdateTexture(texture, NULL, pixelBuffer, width * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    SDL_Event e;
    while (1) 
    {
        if (SDL_PollEvent(&e) && e.type == SDL_QUIT)
        {
            break;
        }
    }
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawLine(Uint32* pixelBuffer, int width, int height, Uint32 color, point beginning, point end)
{
    int dx = abs(end.x - beginning.x);
    int sx = beginning.x < end.x ? 1 : -1;
    int dy = -abs(end.y - beginning.y);
    int sy = beginning.y < end.y ? 1 : -1;
    int err = dx + dy;
    int e2;
    
    int x = beginning.x, y = beginning.y;
    
    while (x != end.x || y != end.y) 
    {
        if ( x >= 0 && x < width &&
             y >= 0 && y < height )
        {
            pixelBuffer[x + width * y] = color;
        }
        
        e2 = err * 2;
        err += dy * (e2 > dy);
        x   += sx * (e2 > dy);
        err += dx * (e2 < dx);
        y   += sy * (e2 < dx);
    }
    
    if ( x >= 0 && x < width &&
        y >= 0 && y < height )
    {
        pixelBuffer[x + width * y] = color;
    }
}

void drawPointCross(Uint32* pixelBuffer, int width, int height, Uint32 color, point position)
{
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
