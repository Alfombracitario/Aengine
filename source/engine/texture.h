#pragma once
#include "engine/types.h"

typedef struct Texture {
    u32 flags;
    u16 width;
    u16 height;
    u16 tilesX;
    u16 tilesY;
    u16 origX;
    u16 origY;    
    u8  format;
    void* platformHandle; // nullptr = fallo o no inicializada
} Texture;

#define textureLoad(name) _textureLoad(name, name##_size)

Texture* _textureLoad   (const u8* buffer, u32 size);
Texture textureLoadFile (const char* path);
void    textureFree     (Texture* tex);
inline void initTiles(Texture* tex, u16 tx, u16 ty) {
    tex->tilesX = tx;
    tex->tilesY = ty;
}
inline void textureSetOrig(Texture* tex, u16 x, u16 y) {
    tex->origX = x;
    tex->origY = y;
}
