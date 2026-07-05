//este código está altamente basado en GRRLIB y Game maker
//se usaron partes del código fuente de GRRLIB para esto
#include "engine/gc/render.h"
#include "engine/fastmath.h"
#include "engine/gc/video.h"
#include "engine/gc/gc_types.h"
#include "engine/gc/debug.h"
#include "engine/gc/camera.h"

#define validateDraw(tex) \
    do { \
        aAssert(tex,                 "validateDraw: texture is NULL"); \
        aAssert(tex->platformHandle, "validateDraw: texture not uploaded to GPU"); \
        aAssert(tex->width  > 0,     "validateDraw: texture width is 0"); \
        aAssert(tex->height > 0,     "validateDraw: texture height is 0"); \
        aAssert(gpuS.textureMode,    "validateDraw: textureMode is false"); \
    } while(0)

#define gpuDisableTexture() \
    do { \
        GX_SetVtxDesc(GX_VA_TEX0, GX_NONE); \
        GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR); \
    } while(0)

#define gpuEnableTexture() \
    do { \
        GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT); \
        GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE); \
    } while(0)

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
        .start   = 10.0f,
        .end     = 100.0f,
    },
    .viewHeight = 480,
    .viewWidth  = 640,
};

//variables copiadas de GRRLIB :>
extern Mtx      GXmodelView2D;
static guVector axis = (guVector){0.0f, 0.0f, 1.0f};

//variables de este archivo
typedef struct TextureState {
    const Texture* tex;
    bool repeat;
    bool filter;
} TextureState;

static TextureState lastTex = { NULL, 0, 0 };

static GXTexObj texObj;

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
    if(gpuS.copyFilter == false){
        GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
    }
    else{
        GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    }
}

void gpuSetTexRepeat(bool repeat){
    gpuS.texRepeat = repeat;
}

static void gpuApplyAlphatest(void) {
    if (!gpuS.aTest) {
        GX_SetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    } else {
        GX_SetAlphaCompare(GX_GREATER, gpuS.aRef, GX_AOP_AND, GX_ALWAYS, 0);
    }
}

void gpuSetAlphatestEnable(bool enable) {
    gpuS.aTest = enable;
    gpuApplyAlphatest();
}

void gpuSetAlphatestRef(u8 ref) {
    gpuS.aRef = ref;
    gpuApplyAlphatest();
}


void gpuSetZtestEnable(bool enable){
    gpuS.zTest = enable;
    GX_SetZMode(enable, gpuS.zFunc, gpuS.zWrite);
}

void gpuSetZfunc(u32 cmp_function){
    gpuS.zFunc = cmp_function;
    GX_SetZMode(gpuS.zTest, cmp_function, gpuS.zWrite);
}

void gpuSetZwriteEnable(bool enable){
    gpuS.zWrite = enable;
    GX_SetZMode(gpuS.zTest, gpuS.zFunc, enable);
}
//cullmode
void gpuSetCullmode(u32 cullmode){
    gpuS.cullmode = cullmode;
    GX_SetCullMode(cullmode);
}


void gpuSetCopyClear(u32 color){
    GXColor col = {
        (color >> 24) & 0xFF,  // r
        (color >> 16) & 0xFF,  // g
        (color >> 8)  & 0xFF,  // b
        (color)       & 0xFF   // a
    };
    GX_SetCopyClear(col, GX_MAX_Z24);
    gpuS.copyColor = color;
}
//Fog
static void gpuApplyFog() {
    if (!gpuS.fog.enabled) {
        GX_SetFog(GX_FOG_NONE, 0, 1, 0.1f, 1, (GXColor){0, 0, 0, 255});
        return;
    }

    GXColor col = {
        (gpuS.fog.color >> 24) & 0xFF,
        (gpuS.fog.color >> 16) & 0xFF,
        (gpuS.fog.color >> 8)  & 0xFF,
        0xFF  // alpha ignorado por GX
    };

    GX_SetFog(gpuS.fog.type, gpuS.fog.start, gpuS.fog.end, cam.near, cam.far, col);
}

void gpuSetFogEnable(bool enable) {
    gpuS.fog.enabled = enable;
    gpuApplyFog();
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
    if(enable == true){
        gpuEnableTexture();
    }
    else{
        gpuDisableTexture();
    }
    gpuS.textureMode = enable;
}

void gpuSetColorMode(bool enable) {
    if(enable) {
        GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);
    } else {
        GX_SetVtxDesc(GX_VA_CLR0, GX_NONE);
        GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);
        GXColor white = {255, 255, 255, 255};
        GX_SetChanMatColor(GX_COLOR0A0, white);
    }
    gpuS.colorMode = enable;
}

void gpuSetNormalMode(bool enable) {
    if(enable) {
        GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT);
    } else {
        GX_SetVtxDesc(GX_VA_NRM, GX_NONE);
    }
    gpuS.normalMode = enable;
}

void gpuSetTexture(const Texture *tex){
    //comprobamos si se cambió de textura (peligroso si se combina con funciones de dibujo de otras librerías, si quieres compatibilidad comenta esta parte)
    if (lastTex.tex == tex &&
        lastTex.repeat == gpuS.texRepeat &&
        lastTex.filter == gpuS.texFilter) {
        return;//misma textura de antes, misma configuración, nada que cambiar.
    }
    
    lastTex.tex    = tex;
    lastTex.repeat = gpuS.texRepeat;
    lastTex.filter = gpuS.texFilter;

    //configurar texture repeat
    GCtexture* gt = (GCtexture*)tex->platformHandle;
    if (gpuS.texRepeat == true){
        GX_InitTexObj(&texObj, gt->pixels, tex->width, tex->height, tex->format, GX_REPEAT, GX_REPEAT, GX_FALSE);
    }
    else {
        GX_InitTexObj(&texObj, gt->pixels, tex->width, tex->height, tex->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
    }
    //configurar texture filter
    if (!gpuS.texFilter){
        GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR,
            0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    }

    GX_LoadTexObj(&texObj,      GX_TEXMAP0);
    GX_SetTevOp  (GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0,   GX_DIRECT);
}

void drawTexture(const Texture *tex, f32 x, f32 y)
{
    validateDraw(tex);

    gpuSetTexture(tex);

    //Calcular posición con origen sin rotación
    const f32 x1 = x - tex->origX;
    const f32 y1 = y - tex->origY;
    const f32 x2 = x1 + tex->width;
    const f32 y2 = y1 + tex->height;

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);

        GX_Position3f32(x1, y1, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 0.0f);

        GX_Position3f32(x2, y1, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 0.0f);

        GX_Position3f32(x2, y2, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 1.0f);

        GX_Position3f32(x1, y2, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 1.0f);

    GX_End();
}
void drawSprite(const Texture *tex, const int frame, f32 x, f32 y){
    validateDraw(tex);

    const f32 tileW = 1.0f / tex->tilesX;
    const f32 tileH = 1.0f / tex->tilesY;

    const int col = frame % tex->tilesX;
    const int row = frame / tex->tilesX;

    const f32 u1 = col * tileW;
    const f32 u2 = u1 + tileW;
    const f32 v1 = row * tileH;
    const f32 v2 = v1 + tileH;

    const f32 x1 = x - tex->origX;
    const f32 y1 = y - tex->origY;
    const f32 x2 = x1 + tex->width*tileW;
    const f32 y2 = y1 + tex->height*tileH;


    gpuSetTexture(tex);

    // Dibujar quad
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(x1, y1, 0.0f);
        GX_Color1u32   (0xFFFFFFFF);
        GX_TexCoord2f32(u1, v1);

        GX_Position3f32(x2, y1, 0.0f);
        GX_Color1u32   (0xFFFFFFFF);
        GX_TexCoord2f32(u2, v1);

        GX_Position3f32(x2, y2, 0.0f);
        GX_Color1u32   (0xFFFFFFFF);
        GX_TexCoord2f32(u2, v2);

        GX_Position3f32(x1, y2, 0.0f);
        GX_Color1u32   (0xFFFFFFFF);
        GX_TexCoord2f32(u1, v2);
    GX_End();
}

void drawSpriteExt(const Texture *tex, const int frame, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color){
    validateDraw(tex);

    gpuSetTexture(tex);//si la textura es igual a la anterior no se hace ningún cambio

    const f32 tileW = 1.0f / tex->tilesX;
    const f32 tileH = 1.0f / tex->tilesY;

    const int col = frame % tex->tilesX;
    const int row = frame / tex->tilesX;

    const f32 u1 = col * tileW;
    const f32 u2 = u1 + tileW;
    const f32 v1 = row * tileH;
    const f32 v2 = v1 + tileH;

    const f32 fsin = fastSin(degrees);
    const f32 fcos = fastCos(degrees);

    const f32 lx0 = -tex->origX * scaleX;
    const f32 ly0 = -tex->origY * scaleY;
    const f32 lx1 = lx0 + tex->width  * tileW * scaleX;
    const f32 ly1 = ly0 + tex->height * tileH * scaleY;

    const f32 lx0cos = lx0 * fcos, lx0sin = lx0 * fsin;
    const f32 lx1cos = lx1 * fcos, lx1sin = lx1 * fsin;
    const f32 ly0cos = ly0 * fcos, ly0sin = ly0 * fsin;
    const f32 ly1cos = ly1 * fcos, ly1sin = ly1 * fsin;

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(x + lx0cos - ly0sin, y + lx0sin + ly0cos, 0.0f);
        GX_Color1u32   (color);
        GX_TexCoord2f32(u1, v1);

        GX_Position3f32(x + lx1cos - ly0sin, y + lx1sin + ly0cos, 0.0f);
        GX_Color1u32   (color);
        GX_TexCoord2f32(u2, v1);

        GX_Position3f32(x + lx1cos - ly1sin, y + lx1sin + ly1cos, 0.0f);
        GX_Color1u32   (color);
        GX_TexCoord2f32(u2, v2);

        GX_Position3f32(x + lx0cos - ly1sin, y + lx0sin + ly1cos, 0.0f);
        GX_Color1u32   (color);
        GX_TexCoord2f32(u1, v2);
    GX_End();
}
void drawTextureExt(const Texture *tex, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color){//centro en 0.5,0.5
    validateDraw(tex);

    gpuSetTexture(tex);

    const f32 fsin = fastSin(degrees);
    const f32 fcos = fastCos(degrees);

    const f32 lx0 = -tex->origX * scaleX;
    const f32 ly0 = -tex->origY * scaleY;
    const f32 lx1 = lx0 + tex->width  * scaleX;
    const f32 ly1 = ly0 + tex->height * scaleY;

    const f32 lx0cos = lx0 * fcos, lx0sin = lx0 * fsin;
    const f32 lx1cos = lx1 * fcos, lx1sin = lx1 * fsin;
    const f32 ly0cos = ly0 * fcos, ly0sin = ly0 * fsin;
    const f32 ly1cos = ly1 * fcos, ly1sin = ly1 * fsin;

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(x + lx0cos - ly0sin, y + lx0sin + ly0cos, 0.0f);
        GX_Color1u32   (color);
        GX_TexCoord2f32(1, 0);

        GX_Position3f32(x + lx1cos - ly0sin, y + lx1sin + ly0cos, 0.0f);
        GX_Color1u32   (color);
        GX_TexCoord2f32(1, 0);

        GX_Position3f32(x + lx1cos - ly1sin, y + lx1sin + ly1cos, 0.0f);
        GX_Color1u32   (color);
        GX_TexCoord2f32(1, 1);

        GX_Position3f32(x + lx0cos - ly1sin, y + lx0sin + ly1cos, 0.0f);
        GX_Color1u32   (color);
        GX_TexCoord2f32(0, 1);
    GX_End();
}
void drawTextureStretched(const Texture* tex, const f32 x, const f32 y, const f32 w, const f32 h){
    validateDraw(tex);

    gpuSetTexture(tex);

    const f32 x2 = x+w;
    const f32 y2 = y+h;

    // Dibujar quad
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);

        GX_Position3f32(x, y, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 0.0f);

        GX_Position3f32(x2, y, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 0.0f);

        GX_Position3f32(x2, y2, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(1.0f, 1.0f);

        GX_Position3f32(x, y2, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(0.0f, 1.0f);

    GX_End();
}

void drawTextureTiled(const Texture* tex, const f32 x, const f32 y){//no disponible en modo sprite aún
    validateDraw(tex);

    //forzamos el modo repeat y restauramos
    bool rep = gpuS.texRepeat;
    gpuS.texRepeat = true;
    gpuSetTexture(tex);
    gpuS.texRepeat = rep;

    const f32 u_repeat = (float)cam.width  / tex->width;
    const f32 v_repeat = (float)cam.height / tex->height;
    const f32 u_off = x / tex->width;
    const f32 v_off = y / tex->height;

    //aplicamos offset y repeat todo por UVs
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);

        GX_Position3f32(0, 0, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(u_off, v_off);

        GX_Position3f32(cam.width, 0, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(u_off + u_repeat, v_off);

        GX_Position3f32(cam.width, cam.height, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(u_off + u_repeat, v_off + v_repeat);

        GX_Position3f32(0, cam.height, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(u_off, v_off + v_repeat);

    GX_End();
}
void drawRectangleColour(f32 x, f32 y, f32 w, f32 h, u32 colorTopLeft, u32 colorTopRight, u32 colorBottomRight, u32 colorBottomLeft){
    if(gpuS.textureMode == true){
        gpuDisableTexture();
    }

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(x,   y,   0.0f); GX_Color1u32(colorTopLeft);
        GX_Position3f32(x+w, y,   0.0f); GX_Color1u32(colorTopRight);
        GX_Position3f32(x+w, y+h, 0.0f); GX_Color1u32(colorBottomRight);
        GX_Position3f32(x,   y+h, 0.0f); GX_Color1u32(colorBottomLeft);
    GX_End();

    if(gpuS.textureMode == true){
        gpuEnableTexture();
        invalidateTexCache(); // forzar recarga de textura después
    }
}

void viewportResize(u16 width, u16 height){
    gpuS.viewWidth  = width;
    gpuS.viewHeight = height;
    GX_SetViewport(0, 0, width, height, 0.0f, 1.0f);
}

void viewportMove(u16 x, u16 y){
    gpuS.viewX = x;
    gpuS.viewY = y;
    GX_SetViewport(x, y, gpuS.viewWidth, gpuS.viewHeight, 0.0f, 1.0f);
}

//matrices

void objectView(f32 posx, f32 posy, f32 posz,
                        f32 angx, f32 angy, f32 angz,
                        f32 scalx, f32 scaly, f32 scalz) {
    Mtx objMtx, m, mv, mvi;

    guMtxIdentity(objMtx);

    if ((scalx != 1.0f) || (scaly != 1.0f) || (scalz != 1.0f)) {
        guMtxIdentity(m);
        guMtxScaleApply(m, m, scalx, scaly, scalz);
        guMtxConcat(m, objMtx, objMtx);
    }

    if ((angx != 0.0f) || (angy != 0.0f) || (angz != 0.0f)) {
        Mtx rx, ry, rz;
        guVector axisx = {1, 0, 0};
        guVector axisy = {0, 1, 0};
        guVector axisz = {0, 0, 1};

        guMtxIdentity(m);
        guMtxRotAxisDeg(rx, &axisx, angx);
        guMtxRotAxisDeg(ry, &axisy, angy);
        guMtxRotAxisDeg(rz, &axisz, angz);
        guMtxConcat(ry, rx, m);
        guMtxConcat(m, rz, m);

        guMtxConcat(m, objMtx, objMtx);
    }

    if ((posx != 0.0f) || (posy != 0.0f) || (posz != 0.0f)) {
        guMtxIdentity(m);
        guMtxTransApply(m, m, posx, posy, posz);
        guMtxConcat(m, objMtx, objMtx);
    }

    guMtxConcat(cam.view, objMtx, mv);
    GX_LoadPosMtxImm(mv, GX_PNMTX0);

    guMtxInverse(mv, mvi);
    guMtxTranspose(mvi, mv);
    GX_LoadNrmMtxImm(mv, GX_PNMTX0);
}