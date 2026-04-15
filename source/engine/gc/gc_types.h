#pragma once
#include <ogc/gx.h>

typedef struct GCtexture {
    GXTexObj texObj;
    void*    pixels;
} GCtexture;