#ifndef ACS_H
#define ACS_H
#include "engine/types.h"
#include "engine/texture.h"

typedef struct {
    const u8* data;
    u32       size;
    u32       pos;
} ACSReader;

int importACS(const u8* data, u32* rawPixels, u32* pal, Texture* tex);
int readACSheader(const u8* data, int* c, int* w, int* h, int* f);
//won't need an exporter, this is a GAME engine.

#endif