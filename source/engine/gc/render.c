//este código está altamente basado en GRRLIB y Game maker
//se usaron partes del código fuente de GRRLIB para esto
#include "engine/gc/render.h"
#include "engine/fastmath.h"
#include "engine/gc/video.h"
#include "engine/gc/gc_types.h"

//variables globales
u32 surfaceWidth = 640;
u32 surfaceHeight = 480;

//variables cacheadas
//Texutras
static bool texFilter = false;
static bool copyFilter = false;
static bool texRepeat = false;

//zbuffer
static bool zTest = false;
static bool zWrite = false;
static u32 zFunc = GX_LEQUAL;

//alpha
static bool gpu_alpha_test = false;
static u8 gpu_alpha_ref = 128;

//cullmode
static u32 cullMode = 0;

//variables de GRRLIB
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

void invalidateTexCache(){
    lastTex.tex    = NULL;
    lastTex.repeat = 0;
    lastTex.filter = 0;
}

//getters y setters
void gpu_set_tex_filter(bool linear){
    texFilter = linear;
}

bool gpu_get_tex_filter(){
    return texFilter;
}

void gpu_set_copy_filter(bool enable){
    copyFilter = enable;
    if(copyFilter == false){
        GX_SetCopyFilter(GX_FALSE, rmode->sample_pattern, GX_FALSE, rmode->vfilter);
    }
    else{
        GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    }
}

bool gpu_get_copy_filter(){
    return copyFilter;
}

void gpu_set_tex_repeat(bool repeat){
    texRepeat = repeat;
}

bool gpu_get_tex_repeat(){
    return texRepeat;
}

static void gpu_apply_alphatest() {
    if (!gpu_alpha_test) {
        GX_SetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    } else {
        GX_SetAlphaCompare(GX_GREATER, gpu_alpha_ref, GX_AOP_AND, GX_ALWAYS, 0);
    }
}

void gpu_set_alphatestenable(bool enable) {
    gpu_alpha_test = enable;
    gpu_apply_alphatest();
}

void gpu_set_alphatestref(u8 ref) {
    gpu_alpha_ref = ref;
    gpu_apply_alphatest();
}


void gpu_set_ztestenable(bool enable){
    zTest = enable;
    GX_SetZMode(enable, zFunc, zWrite);
}

void gpu_set_zfunc(u32 cmp_function){
    zFunc = cmp_function;
    GX_SetZMode(zTest, cmp_function, zWrite);
}

void gpu_set_zwriteenable(bool enable){
    zWrite = enable;
    GX_SetZMode(zTest, zFunc, enable);
}

//cullmode
void gpu_set_cullmode(u32 cullmode){
    cullMode = cullmode;
    GX_SetCullMode(cullmode);
}
u32 gpu_get_cullmode(){
    return cullMode;
}
//TODO: setter y getter de gpu_uses_normal && gpu_uses_texture

void gpu_set_texture(const Texture *tex){
    //comprobamos si se cambió de textura (peligroso si se combina con funciones de dibujo de otras librerías, si quieres compatibilidad comenta esta parte)
    if (lastTex.tex == tex &&
        lastTex.repeat == texRepeat &&
        lastTex.filter == texFilter) {
        return;//misma textura de antes, misma configuración, nada que cambiar.
    }
    
    lastTex.tex    = tex;
    lastTex.repeat = texRepeat;
    lastTex.filter = texFilter;

    //configurar texture repeat
    GCtexture* gt = (GCtexture*)tex->platformHandle;
    if (texRepeat == true){
        GX_InitTexObj(&texObj, gt->pixels, tex->width, tex->height, tex->format, GX_REPEAT, GX_REPEAT, GX_FALSE);
    }
    else {
        GX_InitTexObj(&texObj, gt->pixels, tex->width, tex->height, tex->format, GX_CLAMP, GX_CLAMP, GX_FALSE);
    }
    //configurar texture filter
    if (!texFilter){
        GX_InitTexObjLOD(&texObj, GX_NEAR, GX_NEAR,
            0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
    }

    GX_LoadTexObj(&texObj,      GX_TEXMAP0);
    GX_SetTevOp  (GX_TEVSTAGE0, GX_MODULATE);
    GX_SetVtxDesc(GX_VA_TEX0,   GX_DIRECT);
}

void draw_texture(const Texture *tex, f32 x, f32 y)
{
    if (!tex || !tex->platformHandle) return;

    gpu_set_texture(tex);

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
void draw_sprite(const Texture *tex, const int frame, f32 x, f32 y){
    if (!tex || !tex->platformHandle) return;

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


    gpu_set_texture(tex);

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

void draw_sprite_ext(const Texture *tex, const int frame, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color){
    if (!tex || !tex->platformHandle) return;

    gpu_set_texture(tex);//si la textura es igual a la anterior no se hace ningún cambio

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
void draw_texture_ext(const Texture *tex, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color){//centro en 0.5,0.5
    if (!tex || !tex->platformHandle) return;

    gpu_set_texture(tex);

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
void draw_texture_stretched(const Texture* tex, const f32 x, const f32 y, const f32 w, const f32 h){
    if (!tex || !tex->platformHandle) return;

    gpu_set_texture(tex);

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

void draw_texture_tiled(const Texture* tex, const f32 x, const f32 y){//no disponible en modo sprite aún
    if (!tex || !tex->platformHandle) return;

    //forzamos el modo repeat y restauramos
    bool rep = texRepeat;
    texRepeat = true;
    gpu_set_texture(tex);
    texRepeat = rep;

    const f32 u_repeat = (float)surfaceWidth  / tex->width;
    const f32 v_repeat = (float)surfaceHeight / tex->height;
    const f32 u_off = x / tex->width;
    const f32 v_off = y / tex->height;

    //aplicamos offset y repeat todo por UVs
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);

        GX_Position3f32(0, 0, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(u_off, v_off);

        GX_Position3f32(surfaceWidth, 0, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(u_off + u_repeat, v_off);

        GX_Position3f32(surfaceWidth, surfaceHeight, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(u_off + u_repeat, v_off + v_repeat);

        GX_Position3f32(0, surfaceHeight, 0.0f);
        GX_Color1u32(0xFFFFFFFF);
        GX_TexCoord2f32(u_off, v_off + v_repeat);

    GX_End();
}
void draw_rectangle_colour(f32 x, f32 y, f32 w, f32 h, u32 colorTopLeft, u32 colorTopRight, u32 colorBottomRight, u32 colorBottomLeft){
    // modo sin textura
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(x,   y,   0.0f); GX_Color1u32(colorTopLeft);
        GX_Position3f32(x+w, y,   0.0f); GX_Color1u32(colorTopRight);
        GX_Position3f32(x+w, y+h, 0.0f); GX_Color1u32(colorBottomRight);
        GX_Position3f32(x,   y+h, 0.0f); GX_Color1u32(colorBottomLeft);
    GX_End();

    // restaurar modo textura
    GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    invalidateTexCache(); // forzar recarga de textura después
}

//surfaces (matenme)
Texture* surface_create(u16 width, u16 height){//no hay formatos aún
}
void surface_copy(Texture *tex,u16 posx, u16 posy, bool clear){
}

void vertex_submit(){
    
}