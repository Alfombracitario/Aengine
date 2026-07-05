#include "engine/aengine.h"
#include "engine/texture.h"
#include "engine/gba/acs.h"
#include "engine/gba/debug.h"

#define GBA_VRAM_BASE      ((u8*)0x06000000)
#define GBA_BG_VRAM_BASE   ((u8*)0x06000000) // BG VRAM: mismo inicio que VRAM_BASE
#define GBA_BG_VRAM_SIZE   (64 * 1024)

#define GBA_OBJ_VRAM_BASE  ((u8*)0x06010000) // equivalente a tile_mem[4]
#define GBA_OBJ_VRAM_SIZE  (32 * 1024)

static u8* obj_cursor = GBA_OBJ_VRAM_BASE;
static const u8* obj_end = GBA_OBJ_VRAM_BASE + GBA_OBJ_VRAM_SIZE;

static u8* bg_cursor = GBA_BG_VRAM_BASE;
static const u8* bg_end = GBA_BG_VRAM_BASE + GBA_BG_VRAM_SIZE;


#define GBA_OBJ_PALETTE_BASE ((u16*)0x05000200)
#define GBA_OBJ_PALETTE_BANKS 16
#define GBA_OBJ_PALETTE_COLORS_PER_BANK 16

static int nextPalBank = 0;      // next free 4bpp bank (0-15)
static int lastPalBank = -1;     // last one handed out
static bool obj8bppPalUsed = false;

u32 tileBytesFor(u32 imgSize, u32 bpp) {
    // bpp is bits per pixel (4 or 8)
    // total bytes = pixelCount * bitsPerPixel / 8
    return (imgSize * bpp) / 8;
}

void swizzleToTiles(const u16* src, void* dstVoid, u32 width, u32 height, u32 bpp) {
    u32 tilesX = width  / 8;
    u32 tilesY = height / 8;

    if (bpp == 4) {
        u8* dst = (u8*)dstVoid;
        u32 outIdx = 0;

        for (u32 ty = 0; ty < tilesY; ty++) {
            for (u32 tx = 0; tx < tilesX; tx++) {
                for (u32 row = 0; row < 8; row++) {
                    for (u32 col = 0; col < 8; col += 2) {
                        u32 px = tx * 8 + col;
                        u32 py = ty * 8 + row;

                        u8 lo = src[py * width + px]     & 0xF;
                        u8 hi = src[py * width + px + 1] & 0xF;

                        dst[outIdx++] = (hi << 4) | lo; // low nibble = left pixel
                    }
                }
            }
        }
    } else { // bpp == 8
        u8* dst = (u8*)dstVoid;
        u32 outIdx = 0;

        for (u32 ty = 0; ty < tilesY; ty++) {
            for (u32 tx = 0; tx < tilesX; tx++) {
                for (u32 row = 0; row < 8; row++) {
                    for (u32 col = 0; col < 8; col++) {
                        u32 px = tx * 8 + col;
                        u32 py = ty * 8 + row;
                        dst[outIdx++] = (u8)(src[py * width + px] & 0xFF);
                    }
                }
            }
        }
    }
}

void* gba_allocObjPaletteBank(u32 bpp) {
    if (bpp == 8) {
        aAssert(!obj8bppPalUsed, "8bpp OBJ palette already in use");
        obj8bppPalUsed = true;
        lastPalBank = 0;
        return (void*)GBA_OBJ_PALETTE_BASE;
    }

    // 4bpp: hand out the next 16-color bank
    aAssert(nextPalBank < GBA_OBJ_PALETTE_BANKS, "Out of OBJ palette banks");
    void* ptr = (void*)(GBA_OBJ_PALETTE_BASE + nextPalBank * GBA_OBJ_PALETTE_COLORS_PER_BANK);
    lastPalBank = nextPalBank;
    nextPalBank++;
    return ptr;
}

int gba_lastPaletteBankIndex(void) {
    return lastPalBank;
}

void* gba_allocObjGfx(u32 sizeBytes) {
    aAssert(obj_cursor + sizeBytes <= obj_end, "Out of OBJ VRAM");
    void* ptr = obj_cursor;
    obj_cursor += sizeBytes;
    return ptr;
}

void* gba_allocBgGfx(u32 sizeBytes) {
    aAssert(bg_cursor + sizeBytes <= bg_end, "Out of BG VRAM");
    void* ptr = bg_cursor;
    bg_cursor += sizeBytes;
    return ptr;
}

Texture* loadTile(const u8* buffer, u32 size, const u32 format){

}

Texture* loadSprite(const u8* buffer, u32 size, const u32 format) {
    readACSheader(buffer);

    u32 imgRes = (u32)ACSh.width * (u32)ACSh.height;
    aAssert(imgRes <= workPoolSize, "Sprite too big for workPool scratch buffer");
    u16* rawPixels = workPool;
    u16 pal[256];

    Texture* tex = {0};
    int result = importACS(buffer, rawPixels, pal, tex);
    // to-do: revisar result / manejar error de importACS

    u32 tileBytes = tileBytesFor(ACSh.imgSize, ACSh.bpp);
    void* gfx = gba_allocObjGfx(tileBytes);

    swizzleToTiles(rawPixels, gfx, tex->width, tex->height, ACSh.bpp);

    void* palVram = gba_allocObjPaletteBank(ACSh.bpp);
    memcpy(palVram, pal, ACSh.colorCount * sizeof(u16));

    tex->tilesX = tex->width  / 8;
    tex->tilesY = tex->height / 8;
    tex->origX = 0;
    tex->origY = 0;
    tex->format = ACSh.bpp;
    tex->paletteBank = gba_lastPaletteBankIndex();
    tex->platformHandle = gfx;
    tex->tiles = gfx;
    tex->palette = palVram;
    tex->flags = 0;

    return tex;
}

Texture* _textureLoad(const u8* buffer, u32 size, const u32 format){
    Texture* tex = {0};
    return tex;
}
Texture* textureLoadFile(const char* path, const u32 format){
    Texture* tex = {0};
    return tex;
}

void textureFree(Texture* tex){
    
}