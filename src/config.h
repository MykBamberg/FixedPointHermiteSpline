#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define WINDOW_TITLE "Hermite Spline"

#define COL_BG    0x1E1E2F
#define COL_FG    0xCED6F2
#define COL_POINT 0xC196F3
#define COL_LINE  0x44526A

/* Fixed point math one */
#define SHOW_LINES 1
#define SHOW_POINTS 1

static const uint32_t HeatMapColors[] = {
    0x1e1e2f,
    0x1f1f31,
    0x202033,
    0x212135,
    0x222237,
    0x232339,
    0x24243b,
    0x25253d,
    0x27273f,
    0x282841,
    0x292943,
    0x2a2a45,
    0x2b2b47,
    0x2c2c49,
    0x2d2d4b,
    0x2e2e4d,
    0x303050,
    0x373957,
    0x3f435f,
    0x464c66,
    0x4e566e,
    0x556076,
    0x5d697d,
    0x647385,
    0x6c7d8d,
    0x738694,
    0x7b909c,
    0x8299a3,
    0x8aa3ab,
    0x91adb3,
    0x99b6ba,
    0xa0c0c2,
    0xa8caca,
    0xa9cac2,
    0xabcaba,
    0xaccab3,
    0xaecaab,
    0xafcaa3,
    0xb1ca9c,
    0xb2ca94,
    0xb4ca8d,
    0xb5ca85,
    0xb7ca7d,
    0xb8ca76,
    0xbaca6e,
    0xbbca66,
    0xbdca5f,
    0xbeca57,
    0xc0ca50,
    0xc3c553,
    0xc7c056,
    0xcbbc59,
    0xcfb75c,
    0xd3b25f,
    0xd7ae62,
    0xdba965,
    0xdfa568,
    0xe3a06b,
    0xe79b6e,
    0xeb9771,
    0xef9274,
    0xf38d77,
    0xf7897a,
    0xfb847d,
    0xff8080
};

/* the "weight" of each color
 * if set to 1, each occurrence will increase the color index by 1
 * if set to any other number n,
 * the index will increase for every n additional occurrences */
#define HEAT_MAP_COLOR_WEIGHT 1

#define HEAT_MAP_COLOR_COUNT 65

/* a monochrome bitmap of the shape of every plotted point on the heat map */
const uint32_t HeatMapPointShape[] = {
    0, 1, 2, 1, 0,
    1, 3, 5, 3, 1,
    2, 5, 8, 5, 2,
    1, 3, 5, 3, 1,
    0, 1, 2, 1, 0
};

#define HEAT_MAP_POINT_SHAPE_DISTANCE_FROM_CENTER 2

#endif
