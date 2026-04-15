#ifndef FASTMATH_H
#define FASTMATH_H

extern const float SIN90[90];

float fastSin(float angle);
float fastCos(float angle);

static inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

#endif
