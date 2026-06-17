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
    guVector pos;

    float pitch, yaw, roll;

    guVector to;
    guVector up;

    u16   width, height;
    //matrices
    Mtx    view;
    Mtx44  proj;

} camState;
extern camState cam;

void camSet2D(void);
void camSet3D(void);
void camSetPos(f32 _x, f32 _y, f32 _z);
void camSetLook(f32 _x, f32 _y, f32 _z);
void camSetUp(f32 _x, f32 _y, f32 _z);
void camSetRotation(f32 _pitch, f32 _yaw, f32 _roll);
void camUpdate(void);

#endif
