#include "engine/lib/png/lodepng.h"
#include "engine/general/acs.h"
#include <gccore.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine/texture.h"
#include "malloc.h"
#include "engine/gc/gc_types.h"
#include "engine/gc/debug.h"
#include "engine/gc/render.h"
#include "engine/wii/core.h"

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
static void convertToGXRGB565(const u8* src, u8* dst_raw, u32 width, u32 height) {
    u16* dst = (u16*)dst_raw;
    for (u32 y = 0; y < height; y += 4) {
        for (u32 x = 0; x < width; x += 4) {
            for (u32 ty = 0; ty < 4; ty++)
                for (u32 tx = 0; tx < 4; tx++) {
                    u32 i = ((y + ty) * width + (x + tx)) * 4;
                    u8 r = src[i + 0];
                    u8 g = src[i + 1];
                    u8 b = src[i + 2];
                    *dst++ = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
                }
        }
    }
}
// src: índices lineales 1 byte por pixel
static void convertTo8bpp(const u8* src, u8* dst, u32 width, u32 height) {
    for (u32 y = 0; y < height; y += 4) {
        for (u32 x = 0; x < width; x += 8) {
            for (u32 ty = 0; ty < 4; ty++)
                for (u32 tx = 0; tx < 8; tx++)
                    *dst++ = src[(y + ty) * width + (x + tx)];
        }
    }
}
// src: índices lineales 1 byte por pixel (4-bit cada uno, empacados en el conversor)
static void convertTo4bpp(const u8* src, u8* dst, u32 width, u32 height) {
    for (u32 y = 0; y < height; y += 8) {
        for (u32 x = 0; x < width; x += 8) {
            for (u32 ty = 0; ty < 8; ty++)
                for (u32 tx = 0; tx < 8; tx += 2) {
                    u8 a = src[(y + ty) * width + (x + tx + 0)] & 0xF;
                    u8 b = src[(y + ty) * width + (x + tx + 1)] & 0xF;
                    *dst++ = (a << 4) | b;
                }
        }
    }
}
static Texture textureFromRGBA(const u8* rawPixels, u32 w, u32 h) {
    Texture tex = {0};

    u32 dataSize  = w * h * 4;
    void* gxPixels = memalign(32, dataSize);
    aAssert(gxPixels,"Texture convert error: gxPixels not valid");
    convertToGXRGBA8(rawPixels, (u8*)gxPixels, w, h);
    DCFlushRange(gxPixels, dataSize);

    GCtexture* gt = malloc(sizeof(GCtexture));
    gt->pixels = gxPixels;

    tex.width           = (u16)w;
    tex.height          = (u16)h;
    tex.format          = GX_TF_RGBA8;
    tex.platformHandle  = gt;

    return tex;
}

static Texture textureTo565(const u8* rawPixels, u32 w, u32 h) {
    Texture tex = {0};

    u32 dataSize  = w * h * 4;
    void* gxPixels = memalign(32, dataSize);
    aAssert(gxPixels,"Texture convert error: gxPixels not valid");
    convertToGXRGB565(rawPixels, (u8*)gxPixels, w, h);
    DCFlushRange(gxPixels, dataSize);

    GCtexture* gt = malloc(sizeof(GCtexture));
    gt->pixels = gxPixels;

    tex.width           = (u16)w;
    tex.height          = (u16)h;
    tex.format          = GX_TF_RGB565;
    tex.platformHandle  = gt;
    return tex;
}

Texture* _textureLoad(const u8* buffer, u32 size, const u32 format) {
    Texture* tex = malloc(sizeof(Texture));
    tex->flags           = 0;
    tex->origX           = 0;
    tex->origY           = 0;
    tex->tilesX          = 1;
    tex->tilesY          = 1;

    if(!tex) return NULL;

    if(format == FMT_PNG){
        u8* rawPixels = NULL;
        unsigned int w, h;
        if(lodepng_decode32(&rawPixels, &w, &h, buffer, size)){
            free(tex); return NULL;
        }
        *tex = textureFromRGBA(rawPixels, w, h);
        free(rawPixels);
    }
    else{
        u32* rawPixels = NULL;
        u32* pal = NULL;
        int cAmount, width, height, f;

        readACSheader(buffer,&cAmount,&width,&height,&f);
        u8 bpp       = format >> 6;
        u8 colorMode = (format >> 3) & 0b111;
        
        rawPixels = (u32*)malloc(width * height * sizeof(u32));
        if(cAmount > 0){
            pal = (u32*)malloc((cAmount+1) * sizeof(u32));
        }
        aAssert(rawPixels, "rawPixels is invalid!");

        int out = importACS(buffer, rawPixels, pal, tex);
        aAssert(out != -1,"Not valid ACS file");
        int x, y;
        //transformamos al formato solicitado.
        if((format & GFX_MASK) == 0){
            //dev no solicitó un formato, tendremos que analizar automáticamente qué hacer
            
        }
        switch(format & GFX_MASK){
            default:
            case GFX_8888:
            case GFX_888:
            case GFX_1555:
                *tex = textureFromRGBA((const u8*)rawPixels, tex->width, tex->height);
            break;

            case GFX_565:
                if(bpp == 0){
                    *tex = textureTo565((const u8*)rawPixels, tex->width, tex->height);
                }
                else{
                    if(bpp > 4){
                        
                    }else{

                    }
                }
            break;

            case GFX_GRAY4:
                
            break;

            case GFX_GRAY8:

            break;

            case GFX_8BPP:

            break;

            case GFX_1BPP:
            case GFX_2BPP:
            case GFX_4BPP:

            break;
        }
                

        free(rawPixels);
        free(pal);
    }

    return tex;
}

Texture* textureLoadFile(const char* path, const u32 format){
    FILE* f = fopen(path, "rb");
    if(!f) return NULL;

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    u8* data = malloc(size);
    if(!data){ fclose(f); return NULL; }

    fread(data, 1, size, f);
    fclose(f);

    Texture* tex = _textureLoad(data, size, format);
    free(data);
    return tex;
}

void textureFree(Texture* tex) {
    if (!tex || !tex->platformHandle) return;
    GCtexture* gt = (GCtexture*)tex->platformHandle;
    free(gt->pixels);
    free(gt);
    tex->platformHandle = NULL;
}
