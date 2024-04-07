#include "display.h"

void displayRGBPixelBuffer(Uint32* pixelBuffer, int width, int height, char* title)
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    
    SDL_Init(SDL_INIT_VIDEO);
    
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
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

point addPoints(point a, point b) 
{
    point p; 
    p.x = a.x + b.x; 
    p.y = a.y + b.y; 
    return p;
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
