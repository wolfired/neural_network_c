#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "bresenham.h"

void bresenham_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, void (*f)(int32_t x, int32_t y)) {
    bool steep = abs(x1 - x0) < abs(y1 - y0);

    if(steep) {
        x0 ^= y0;
        y0 ^= x0;
        x0 ^= y0;

        x1 ^= y1;
        y1 ^= x1;
        x1 ^= y1;
    }

    if(x0 > x1) {
        x0 ^= x1;
        x1 ^= x0;
        x0 ^= x1;

        y0 ^= y1;
        y1 ^= y0;
        y0 ^= y1;
    }

    int32_t dx     = x1 - x0;
    int32_t dx2    = dx << 1;
    int32_t dy2    = abs(y1 - y0) << 1;
    int32_t step_y = y0 > y1 ? -1 : 1;

    int32_t d = dy2 - dx;

    while(x0 <= x1) {
        if(steep) {
            f(y0, x0);
        } else {
            f(x0, y0);
        }

        x0 += 1;
        if(0 > d) {
            d += dy2;
        } else {
            d += dy2 - dx2;
            y0 += step_y;
        }
    }
}

void bresenham_circ(int32_t x, int32_t y, int32_t r, void (*f)(int32_t x, int32_t y)) {
    int32_t d  = 1 - r;
    int32_t x0 = r;
    int32_t y0 = 0;
    while(x0 >= y0) {
        f(x + x0, y + y0);
        f(x + y0, y + x0);
        f(x - x0, y + y0);
        f(x - y0, y + x0);
        f(x + x0, y - y0);
        f(x + y0, y - x0);
        f(x - x0, y - y0);
        f(x - y0, y - x0);

        if(0 > d) {
            d += (y0 << 1) + 3;
        } else {
            d += ((y0 - x0) << 1) + 5;
            x0 -= 1;
        }
        y0 += 1;
    }
}
