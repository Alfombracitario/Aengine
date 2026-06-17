#include "engine/gc/render.h"
#include "engine/gc/camera.h"
#include "engine/fastmath.h"
//muy simplificado

#define camType3D 0
#define camType2D 1

camState cam = {
    .type   = camType2D,
    .fov    = 90.0f,
    .near   = 0.1f,
    .far    = 1000.0f,
    .aspect = 4.0f/3.0f,
    .pos    = {0.0f, 0.0f, 0.0f},
    .up     = {0.0f, 1.0f, 0.0f},
    .to     = {0.0f, 0.0f, -100.0f},
    .pitch  = 0.0f,
    .yaw    = 0.0f,
    .roll   = 0.0f,
    .width  = 640,
    .height = 480,
};

void camSet3D(void) {
    gpuSetZtestEnable(gpuS.zTest);
    gpuSetCullmode(gpuS.cullmode);

    guLookAt(cam.view, &cam.pos, &cam.up, &cam.to);
    GX_LoadPosMtxImm(cam.view, GX_PNMTX0);

    Mtx44 m;
    guPerspective(m, cam.fov, cam.aspect, cam.near, cam.far);
    GX_LoadProjectionMtx(m, GX_PERSPECTIVE);
    cam.type = camType3D;
}

void camSet2D(void) {
    GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GX_SetCullMode(GX_CULL_NONE);

    guOrtho(cam.proj, 0, cam.height, 0, cam.width, 0, 1000.0f);
    GX_LoadProjectionMtx(cam.proj, GX_ORTHOGRAPHIC);

    guMtxIdentity(cam.view);
    guMtxTransApply(cam.view, cam.view, 0, 0, -100.0f);
    GX_LoadPosMtxImm(cam.view, GX_PNMTX0);
    cam.type = camType2D;
}

void camSetPos(f32 _x, f32 _y, f32 _z){
    cam.pos.x = _x;
    cam.pos.y = _y;
    cam.pos.z = _z;
    camUpdate();
}

void camSetLook(f32 _x, f32 _y, f32 _z){
    cam.to.x = _x;
    cam.to.y = _y;
    cam.to.z = _z;
    camUpdate();
}

void camSetUp(f32 _x, f32 _y, f32 _z){
    cam.up.x = _x;
    cam.up.y = _y;
    cam.up.z = _z;
    camUpdate();
}

void camSetRotation(f32 _pitch, f32 _yaw, f32 _roll){
    cam.pitch = _pitch;
    cam.yaw   = _yaw;
    cam.roll  = _roll;

    // calcular dirección de mirada desde pitch y yaw
    f32 cp = fastCos(_pitch);
    f32 sp = fastSin(_pitch);
    f32 cy = fastCos(_yaw);
    f32 sy = fastSin(_yaw);

    cam.to.x = cam.pos.x + cp * sy;
    cam.to.y = cam.pos.y + sp;
    cam.to.z = cam.pos.z + cp * cy;

    // roll afecta el vector up
    f32 cr = fastCos(_roll);
    f32 sr = fastSin(_roll);
    cam.up.x = -sr;
    cam.up.y =  cr;
    cam.up.z =  0.0f;
    camUpdate();
}
void camUpdate(void) {
    guLookAt(cam.view, &cam.pos, &cam.up, &cam.to);
}