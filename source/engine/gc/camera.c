#include "engine/gc/render.h"
#include "engine/gc/camera.h"
//muy simplificado
void setCamera3D(float fov, u32 bgCol, float fogDist, float fogDistFar){
    //GRRLIB_3dMode(0.1, fogDistFar, fov, 0, 0);

    u8 a = bgCol & 0xFF;
    u8 b = (bgCol>>8) & 0xFF;
    u8 g = (bgCol>>16) & 0xFF;
    u8 r = (bgCol>>24) & 0xFF;
    GXColor fogCol = {r,g,b,a};
    GX_SetCopyClear(fogCol, GX_MAX_Z24);
    GX_SetFog(GX_FOG_LIN, fogDist, fogDistFar, fogDist, fogDistFar, fogCol);
}

void updateCamera3D(float xfrom, float yfrom, float zfrom, float xto, float yto, float zto){

}

void updateCamera25D(float x, float y, float z){

}

void setCamera2D(){
    //GRRLIB_2dMode();
    GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS,  GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}