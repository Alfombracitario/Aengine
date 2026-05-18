#ifndef CAMERA_H
#define CAMERA_H
#include "engine/gc/gc_types.h"

typedef struct {
    // proyección
    u8    type;
    float fov;
    float near;
    float far;
    float aspect;

    // posición
    float x, y, z;

    float pitch, yaw, roll;

    float xto, yto, zto;
    float xup, yup, zup;

    u16   width, height;
    //matrices
    Mtx    view;
    Mtx44  proj;
} camState;
extern camState cam;

void camSet2D(void);

#endif
