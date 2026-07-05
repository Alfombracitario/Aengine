#ifndef ACS_H
#define ACS_H
#include "engine/types.h"
#include "engine/texture.h"
#include "engine/aengine.h"

typedef struct {
    u32 imgSize;
    u8 width;
    u8 height;
    u8 bpp;
    u8 colorCount;
    u8 colorMode;
    u8 headerSize;
    bool valid;
} ACSHeader;

extern ACSHeader ACSh;

int importACS(const u8* data, u16* rawPixels, u16* pal, Texture* tex);
bool readACSheader(const u8* data);

#endif