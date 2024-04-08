#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define WINDOW_TITLE "Hermite Spline"

/* Catppuccin inspired colors */
#define COL_BG    0x1E1E2F
#define COL_FG    0xCED6F2
#define COL_POINT 0xC196F3
#define COL_LINE  0x44526A

/* Fixed point math one */
#define ONE (1 << 12)

#define SHOW_LINES 1
#define SHOW_POINTS 1

const uint32_t HeatMapColors[] = 
{
    0x1E1E2F,
    0x233B63,
    0x296696,
    0x2E83C9,
    0x33A1FC,
    0x5CB9FF,
    0x85D0FF,
    0xAEE7FF,
    0xD7FFFF,
    0xFFFFF0,
    0xFFF3CC,
    0xFFE7A9,
    0xFFDB85,
    0xFFCF62,
    0xFFC43F,
    0xFFB81B,
    0xFFAD00,
    0xFFB033,
    0xFFB266,
    0xFFB599,
    0xFFB8CC,
    0xFFBBFF,
    0xFFA5F2,
    0xFF8FE5,
    0xFF79D8,
    0xFF63CB,
    0xFF4DBE,
    0xFF37B1,
    0xFF21A4,
    0xFF0B97,
    0xFF0080,
    0xFF8080
};

/* the "weight" of each color
 * if at 1, each occurrence will increase the color index by 1
 * if at any other number n,
 * the index will increase for every n additional occurrences
 * 22 is chosen since the HeatMapPointShape sum from left to right
 * in the middle of the shape equals 22 */
#define HEAT_MAP_COLOR_WEIGHT 22

#define HEAT_MAP_COLOR_COUNT 32

/* a monochrome bitmap of the shape of every plotted point on the heat map */
const uint32_t HeatMapPointShape[] =
{
    0, 1, 2, 1, 0,
    1, 3, 5, 3, 1,
    2, 5, 8, 5, 2, /* 2 + 5 + 8 + 5 + 2 = 22 */
    1, 3, 5, 3, 1,
    0, 1, 2, 1, 0
};

#define HEAT_MAP_POINT_SHAPE_DISTANCE_FROM_CENTER 2

#endif
