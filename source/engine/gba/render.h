#ifndef RENDER_H
#define RENDER_H
#include "engine/texture.h"

typedef struct {
    bool  enabled;
    u8    type;
    u32   color;
    u32   start;
    u32   end;
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
void invalidateTexCache(void);

//textures
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

//zTest... yeah I don't think this will be good for the GBA
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

//Alpha
void gpuSetAlphatestEnable(bool enable);
void gpuSetAlphatestRef(u8 ref);

//Cullmode
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

#define cullBack        GX_CULL_BACK
#define cullFront       GX_CULL_FRONT

#define fogLinear   GX_FOG_PERSP_LIN
#define fogExp      GX_FOG_PERSP_EXP
#define fogExp2     GX_FOG_PERSP_EXP2
#define fogRevExp   GX_FOG_PERSP_REVEXP
#define fogRevExp2  GX_FOG_PERSP_REVEXP2

//incluso más macros

//macros de vertices genéricas
#define vertexBegin(count)             GX_Begin(GX_QUADS, GX_VTXFMT0, count)
#define vertexEnd()                    GX_End()

#define vertexPos2(x, y)               GX_Position2f32(x, y)
#define vertexPos3(x, y, z)            GX_Position3f32(x, y, z)

#define vertexColor(r, g, b, a)        GX_Color4u8(r, g, b, a)
#define vertexColor1(clr)              GX_Color1u32(clr)

#define vertexNormal(x, y, z)          GX_Normal3f32(x, y, z)

#define vertexUv(s, t)                 GX_TexCoord2f32(s, t)
#define vertexUv1(s)                   GX_TexCoord1f32(s)

// ─── POSITION ─────────────────────────────────────────────────────────────────

#define vertexPos2f32(x, y)            GX_Position2f32(x, y)
#define vertexPos2s16(x, y)            GX_Position2s16(x, y)
#define vertexPos2u16(x, y)            GX_Position2u16(x, y)
#define vertexPos2s8(x, y)             GX_Position2s8(x, y)
#define vertexPos2u8(x, y)             GX_Position2u8(x, y)

#define vertexPos3f32(x, y, z)         GX_Position3f32(x, y, z)
#define vertexPos3s16(x, y, z)         GX_Position3s16(x, y, z)
#define vertexPos3u16(x, y, z)         GX_Position3u16(x, y, z)
#define vertexPos3s8(x, y, z)          GX_Position3s8(x, y, z)
#define vertexPos3u8(x, y, z)          GX_Position3u8(x, y, z)

#define vertexPos1x8(index)            GX_Position1x8(index)
#define vertexPos1x16(index)           GX_Position1x16(index)

// ─── COLOR ────────────────────────────────────────────────────────────────────

#define vertexColor4u8(r, g, b, a)     GX_Color4u8(r, g, b, a)
#define vertexColor3u8(r, g, b)        GX_Color3u8(r, g, b)
#define vertexColor3f32(r, g, b)       GX_Color3f32(r, g, b)
#define vertexColor1u32(clr)           GX_Color1u32(clr)
#define vertexColor1u16(clr)           GX_Color1u16(clr)
#define vertexColor1x8(index)          GX_Color1x8(index)
#define vertexColor1x16(index)         GX_Color1x16(index)

// ─── NORMAL ───────────────────────────────────────────────────────────────────

#define vertexNormalf32(x, y, z)       GX_Normal3f32(x, y, z)
#define vertexNormals16(x, y, z)       GX_Normal3s16(x, y, z)
#define vertexNormals8(x, y, z)        GX_Normal3s8(x, y, z)
#define vertexNormal1x8(index)         GX_Normal1x8(index)
#define vertexNormal1x16(index)        GX_Normal1x16(index)

// ─── UV ───────────────────────────────────────────────────────────────────────

#define vertexUv2f32(s, t)             GX_TexCoord2f32(s, t)
#define vertexUv2s16(s, t)             GX_TexCoord2s16(s, t)
#define vertexUv2u16(s, t)             GX_TexCoord2u16(s, t)
#define vertexUv2s8(s, t)              GX_TexCoord2s8(s, t)
#define vertexUv2u8(s, t)              GX_TexCoord2u8(s, t)

#define vertexUv1f32(s)                GX_TexCoord1f32(s)
#define vertexUv1s16(s)                GX_TexCoord1s16(s)
#define vertexUv1u16(s)                GX_TexCoord1u16(s)
#define vertexUv1s8(s)                 GX_TexCoord1s8(s)
#define vertexUv1u8(s)                 GX_TexCoord1u8(s)

#define vertexUv1x8(index)             GX_TexCoord1x8(index)
#define vertexUv1x16(index)            GX_TexCoord1x16(index)

// ─── MATRIX ───────────────────────────────────────────────────────────────────

#define vertexMatrix(index)            GX_MatrixIndex1x8(index)

#endif