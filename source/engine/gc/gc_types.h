#pragma once
#include <ogc/gx.h>
#include <gctypes.h>
#include <stdbool.h>

typedef struct GCtexture {
    GXTexObj  texObj;
    void*     pixels;

    GXTlutObj tlutObj;
    u16*     tlutData;
    bool      hasTlut;
} GCtexture;