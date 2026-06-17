#ifndef FASTMATH_H
#define FASTMATH_H
#include "engine/types.h"

extern const float SIN90[90];

float fastSin(float angle);
float fastCos(float angle);
int fastIntLog2(unsigned int x);

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

#endif
