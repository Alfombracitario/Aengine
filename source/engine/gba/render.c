#include "engine/gba/render.h"
#include "engine/fastmath.h"
#include "engine/gba/debug.h"
//#include "engine/gba/camera.h"

// render.c
gpuState gpuS = {
    .textureMode= true,
    .colorMode  = true,
    .normalMode = false,
    .texFilter  = true,
    .copyFilter = false,
    .copyColor  = 0,
    .texRepeat  = false,
    .zTest      = true,
    .zWrite     = true,
    .zFunc      = GX_LEQUAL,
    .zNear      = 0,
    .zFar       = 0,
    .aTest      = false,
    .aRef       = 128,
    .cullmode   = GX_CULL_BACK,
    .fog = {
        .enabled = false,
        .type    = GX_FOG_PERSP_LIN,
        .color   = 0x000000FF,
        .start   = 0,
        .end     = 0f,
    },
    .viewHeight = 160,
    .viewWidth  = 240,
};

//variables de este archivo
typedef struct TextureState {
    const Texture* tex;
    bool repeat;
    bool filter;
} TextureState;

static TextureState lastTex = { NULL, 0, 0 };

void invalidateTexCache(void){
    lastTex.tex    = NULL;
    lastTex.repeat = 0;
    lastTex.filter = 0;
}

//setters
void gpuSetTexFilter(bool linear){
    gpuS.texFilter = linear;
}

void gpuSetCopyFilter(bool enable){
    gpuS.copyFilter = enable;
}

void gpuSetTexRepeat(bool repeat){
    gpuS.texRepeat = repeat;
}

void gpuSetAlphatestEnable(bool enable) {
    gpuS.aTest = enable;
}

void gpuSetAlphatestRef(u8 ref) {
    gpuS.aRef = ref;
}

void gpuSetZtestEnable(bool enable){
    gpuS.zTest = enable;
}

void gpuSetZfunc(u32 cmp_function){
    gpuS.zFunc = cmp_function;
    GX_SetZMode(gpuS.zTest, cmp_function, gpuS.zWrite);
}

void gpuSetZwriteenable(bool enable){
    gpuS.zWrite = enable;
}

//cullmode
void gpuSetCullmode(u32 cullmode){
    gpuS.cullmode = cullmode;
}


void gpuSetCopyClear(u32 color){
    gpuS.copyColor = color;
}
//Fog

void gpuSetFogEnable(bool enable) {
    gpuS.fog.enabled = enable;
}

void gpuSetFogType(u32 type) {
    gpuS.fog.type = type;
}

void gpuSetFogColor(u32 color) {
    gpuS.fog.color = color;
}

void gpuSetFogRange(float start, float end) {
    gpuS.fog.start = start;
    gpuS.fog.end = end;
}

void gpuSetTextureMode(bool enable){
    gpuS.textureMode = enable;
}

void gpuSetColorMode(bool enable) {
    gpuS.colorMode = enable;
}

void gpuSetNormalMode(bool enable) {
    gpuS.normalMode = enable;
}

void gpuSetTexture(const Texture *tex){
    lastTex.tex    = tex;
    lastTex.repeat = gpuS.texRepeat;
    lastTex.filter = gpuS.texFilter;
}

void drawTexture(const Texture *tex, f32 x, f32 y){
    gpuSetTexture(tex);
}
void drawSprite(const Texture *tex, const int frame, f32 x, f32 y){
    gpuSetTexture(tex);
}

void drawSpriteExt(const Texture *tex, const int frame, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color){
    gpuSetTexture(tex);

}
void drawTextureExt(const Texture *tex, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color){//centro en 0.5,0.5
    gpuSetTexture(tex);
}
void drawTextureStretched(const Texture* tex, const f32 x, const f32 y, const f32 w, const f32 h){
    gpuSetTexture(tex);
}

void drawTextureTiled(const Texture* tex, const f32 x, const f32 y){
    bool rep = gpuS.texRepeat;
    gpuS.texRepeat = true;
    gpuSetTexture(tex);
    gpuS.texRepeat = rep;
}
void drawRectangleColour(f32 x, f32 y, f32 w, f32 h, u32 colorTopLeft, u32 colorTopRight, u32 colorBottomRight, u32 colorBottomLeft){
    if(gpuS.textureMode == true){
        gpuDisableTexture();
    }

    if(gpuS.textureMode == true){
        gpuEnableTexture();
    }
}

//THIS IS NOT A CAMERA

//Size of the simulated surface
void viewportResize(u16 width, u16 height){
    gpuS.viewWidth  = width;
    gpuS.viewHeight = height;
}

//offset of the simulated surface
void viewportMove(u16 x, u16 y){
    gpuS.viewX = x;
    gpuS.viewY = y;
}