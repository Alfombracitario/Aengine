#pragma once
#include "engine/types.h"

typedef struct Texture {
    u32 flags;
    u16 width, height;
    u16 tilesX, tilesY;
    u16 origX, origY;
    u8  format;
    u8  paletteBank;

    void* platformHandle; // fuente de verdad: lo que da el allocator, se usa para free()
    void* tiles;          // == platformHandle (o offset conocido dentro de él), solo para comodidad de lectura/escritura de píxeles
    void* palette;        // puntero a la paleta ya en VRAM (zona separada, no offset de platformHandle)
} Texture;

#define textureLoad(name, format) _textureLoad(name, name##_size, format)
#define spriteLoad(name, format)  loadSprite(name, name##_size, format)
#define tileLoad(name, format)    loadTile(name, name##_size, format)

Texture* _textureLoad   (const u8* buffer, u32 size,const u32 format);
Texture* textureLoadFile (const char* path,const u32 format);
void     textureFree     (Texture* tex);

Texture* loadSprite(const u8* buffer, u32 size, const u32 format);
Texture* loadTile(const u8* buffer, u32 size, const u32 format);

inline void initTiles(Texture* tex, u16 tx, u16 ty) {
    tex->tilesX = tx;
    tex->tilesY = ty;
}
inline void textureSetOrig(Texture* tex, u16 x, u16 y) {
    tex->origX = x;
    tex->origY = y;
}

// Format field (exclusive)

#define FMT_MASK     0b11000000

#define FMT_PNG      0b10000000
#define FMT_ACS      0b11000000

// Pixel format (output)
#define GFX_MASK     0b00111100
#define GFX_1BPP     0b00000100
#define GFX_2BPP     0b00001000
#define GFX_4BPP     0b00001100
#define GFX_8BPP     0b00010000
#define GFX_GRAY4    0b00010100
#define GFX_GRAY8    0b00011000
#define GFX_1555     0b00011100
#define GFX_888      0b00100000
#define GFX_565      0b00100100
#define GFX_8888     0b00101000
#define GFX_5551     GFX_1555
#define GFX_555      GFX_565

// Layout flags 
#define GFX_TILED    0b00000001