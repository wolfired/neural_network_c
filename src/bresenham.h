#ifndef BRESENHAM_H
#define BRESENHAM_H

#include <stdint.h>

void bresenham_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, void (*f)(int32_t x, int32_t y));
void bresenham_circ(int32_t x, int32_t y, int32_t r, void (*f)(int32_t x, int32_t y));

#endif
