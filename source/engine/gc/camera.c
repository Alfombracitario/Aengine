#include "engine/gc/render.h"
#include "engine/gc/camera.h"
//muy simplificado

#define camType3D 0
#define camType2D 1

camState cam = {
    .type   = camType2D,
    .fov    = 90.0f,
    .near   = 0.1f,
    .far    = 1000.0f,
    .aspect = 4.0f/3.0f,
    .x      = 0.0f,
    .y      = 0.0f,
    .z      = 0.0f,
    .pitch  = 0.0f,
    .yaw    = 0.0f,
    .roll   = 0.0f,
    .xto    = 0.0f,
    .yto    = 0.0f,
    .zto    = 0.0f,
    .xup    = 0.0f,
    .yup    = 1.0f,
    .zup    = 0.0f,
    .width  = 640,
    .height = 480,
};

void camSet3D(void){
    //recuperar algunos estados del GPU
    gpuSetZtestEnable(gpuS.zTest);
    gpuSetCullmode(gpuS.cullmode);
    
}

void camSet2D(void) {
    GX_SetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);  // solo si alternas con 3D
    GX_SetCullMode(GX_CULL_NONE);

    guOrtho(cam.proj, 0, cam.height, 0, cam.width, 0, 1000.0f);
    GX_LoadProjectionMtx(cam.proj, GX_ORTHOGRAPHIC);

    guMtxIdentity(cam.view);
    guMtxTransApply(cam.view, cam.view, 0, 0, -100.0f);
    GX_LoadPosMtxImm(cam.view, GX_PNMTX0);
}

void camUpdate(void){
}