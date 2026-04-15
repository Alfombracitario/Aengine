#include "engine/lib/png/lodepng.h"
#include <gccore.h>
#include <stdlib.h>
#include "engine/texture.h"
#include "malloc.h"
#include "engine/gc/gc_types.h"

static void convertToGXRGBA8(const u8* src, u8* dst, u32 width, u32 height) {
    for (u32 y = 0; y < height; y += 4) {
        for (u32 x = 0; x < width; x += 4) {
            for (u32 ty = 0; ty < 4; ty++)
                for (u32 tx = 0; tx < 4; tx++) {
                    u32 i = ((y + ty) * width + (x + tx)) * 4;
                    *dst++ = src[i + 3]; // A
                    *dst++ = src[i + 0]; // R
                }
            for (u32 ty = 0; ty < 4; ty++)
                for (u32 tx = 0; tx < 4; tx++) {
                    u32 i = ((y + ty) * width + (x + tx)) * 4;
                    *dst++ = src[i + 1]; // G
                    *dst++ = src[i + 2]; // B
                }
        }
    }
}

static Texture textureFromRGBA(const u8* rawPixels, u32 w, u32 h) {
    Texture tex = {0};

    u32 dataSize  = w * h * 4;
    void* gxPixels = memalign(32, dataSize);
    if (!gxPixels) return tex; // platformHandle queda nullptr

    convertToGXRGBA8(rawPixels, (u8*)gxPixels, w, h);
    DCFlushRange(gxPixels, dataSize);

    GCtexture* gt = malloc(sizeof(GCtexture));
    gt->pixels = gxPixels;

    tex.width           = (u16)w;
    tex.height          = (u16)h;
    tex.format          = GX_TF_RGBA8;
    tex.flags           = 0;
    tex.platformHandle  = gt;
    tex.origX           = 0;
    tex.origY           = 0;
    tex.tilesX          = 1;
    tex.tilesY          = 1;


    return tex;
}

Texture* _textureLoad(const u8* buffer, u32 size) {
    u8* rawPixels = NULL;
    unsigned int w, h;
    if (lodepng_decode32(&rawPixels, &w, &h, buffer, size)) return NULL;
    Texture* tex = malloc(sizeof(Texture));
    *tex = textureFromRGBA(rawPixels, w, h);
    free(rawPixels);
    return tex;
}

Texture textureLoadFile(const char* path) {
    u8* rawPixels = NULL;
    unsigned int w, h;
    if (lodepng_decode32_file(&rawPixels, &w, &h, path)) {
        Texture empty = {0}; return empty;
    }
    Texture tex = textureFromRGBA(rawPixels, w, h);
    free(rawPixels);
    return tex;
}

void textureFree(Texture* tex) {
    if (!tex || !tex->platformHandle) return;
    GCtexture* gt = (GCtexture*)tex->platformHandle;
    free(gt->pixels);
    free(gt);
    tex->platformHandle = NULL;
}
