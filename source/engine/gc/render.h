#ifndef RENDER_H
#define RENDER_H
#include <gccore.h>
#include "engine/texture.h"

typedef struct {
    bool  enabled;
    u8    type;
    u32   color;
    float start;
    float end;
} fogState;

typedef struct {
    bool textureMode;
    bool normalMode;
    bool colorMode;
    bool texFilter;
    bool copyFilter;
    bool copyColor;
    bool texRepeat;
    bool zTest;
    bool zWrite;
    u32  zFunc;
    u32  zNear;
    u32  zFar;
    bool aTest;
    u8   aRef;
    u32  cullmode;
    fogState  fog;
    u16  viewHeight;
    u16  viewWidth;
    u16  viewX;
    u16  viewY;
} gpuState;

extern gpuState gpuS;
extern fogState fogS;

// | =============================== [GPU STATES] =============================== |
//funciones auxiliares
void invalidateTexCache(void);

//texturas
void gpuSetTexFilter(bool linear);
void gpuSetCopyFilter(bool enable);
void gpuSetTexRepeat(bool repeat);
void gpuSetTexture(const Texture *tex);

inline bool gpuGetTexFilter(void){
    return gpuS.texFilter;
}
inline bool gpuGetCopyFilter(void){
    return gpuS.copyFilter;
}
inline bool gpuGetTexRepeat(void){
    return gpuS.texRepeat;
}

//ztest
void gpuSetZtestEnable(bool enable);
void gpuSetZfunc(u32 cmp_function);
void gpuSetZwriteEnable(bool enable);

inline bool gpuGetZtest(void){
    return gpuS.zTest;
}
inline u32 gpuGetZfunc(void){
    return gpuS.zFunc;
}
inline bool gpuGetZwriteEnable(void){
    return gpuS.zWrite;
}

//alpha
void gpuSetAlphatestEnable(bool enable);
void gpuSetAlphatestRef(u8 ref);
//cullmode
void gpuSetCullmode(u32 cullmode);

inline u32 gpuGetCullmode(){
    return gpuS.cullmode;
}

//fog
void gpuSetFogEnable(bool enable);
void gpuSetFogType(u32 type);
void gpuSetFogColor(u32 color);
void gpuSetFogRange(float start, float end);

inline bool gpuGetFogEnabled(void) {
    return gpuS.fog.enabled;
}

inline u32 gpuGetFogType(void) {
    return gpuS.fog.type;
}

inline u32 gpuGetFogColor(void) {
    return gpuS.fog.color;
}

inline float gpuGetFogStart(void) {
    return gpuS.fog.start;
}

inline float gpuGetFogEnd(void) {
    return gpuS.fog.end;
}

//copy clear
void gpuSetCopyClear(u32 color);
inline u32 gpuGetCopyClear(void){
    return gpuS.copyColor;
}

void gpuSetTextureMode(bool enable);
void gpuSetColorMode(bool enable);
void gpuSetNormalMode(bool enable);

inline bool gpuGetTextureMode(void){
    return gpuS.textureMode;
}
inline bool gpuGetColorMode(void){
    return gpuS.colorMode;
}
inline bool gpuGetNormalMode(void){
    return gpuS.normalMode;
}

// | =============================== [DRAW] =============================== |
//draw sprite
void drawSprite(const Texture *tex, const int frame, f32 x, f32 y);
void drawSpriteExt(const Texture *tex, const int frame, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color);
//draw texture (custom)
void drawTexture(const Texture *tex, f32 xpos, f32 ypos);
void drawTextureExt(const Texture *tex, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color);
void drawTextureStretched(const Texture* tex, const f32 x, const f32 y, const f32 w, const f32 h);
void drawTextureTiled(const Texture* tex, const f32 x, const f32 y);
//draw shape
void drawRectangleColour(f32 x, f32 y, f32 w, f32 h, u32 colorTopLeft, u32 colorTopRight, u32 colorBottomRight, u32 colorBottomLeft);
//viewport
void viewportResize(u16 width, u16 height);
void viewportMove(u16 x, u16 y);

//Macros
#define cBlack   0x000000FF
#define cMarron  0x800000FF
#define cGreen   0x008000FF
#define cOlive   0x808000FF
#define cNavy    0x000080FF
#define cPurple  0x800080FF
#define cTeal    0x008080FF
#define cGray    0x808080FF
#define cSilver  0xC0C0C0FF
#define cRed     0xFF0000FF
#define cLime    0x00FF00FF
#define cYellow  0xFFFF00FF
#define cOrange  0xFF7F00FF
#define cBlue    0x0000FFFF
#define cFuchsia 0xFF00FFFF
#define cAqua    0x00FFFFFF
#define cWhite   0xFFFFFFFF

//
#define cmpFuncNever        GX_NEVER
#define cmpFuncLess         GX_LESS
#define cmpFuncEqual        GX_EQUAL
#define cmpFuncLessEqual    GX_LEQUAL
#define cmpFuncGreater      GX_GREATER
#define cmpFuncNotEqual     GX_NEQUAL
#define cmpFuncGreaterEqual GX_GEQUAL
#define cmpFuncAlways       GX_ALWAYS

#define cullNone        GX_CULL_NONE
#define cullClockwise   GX_CULL_BACK
#define cullCounterCW   GX_CULL_FRONT

#define fogLinear   GX_FOG_PERSP_LIN
#define fogExp      GX_FOG_PERSP_EXP
#define fogExp2     GX_FOG_PERSP_EXP2
#define fogRevExp   GX_FOG_PERSP_REVEXP
#define fogRevExp2  GX_FOG_PERSP_REVEXP2

#endif