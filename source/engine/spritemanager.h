#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#define MAX_SPRITES 128
#include "engine/types.h"
#include "engine/texture.h"

typedef void (*SpriteFunc)(int id);

typedef struct SpriteVTable {
    SpriteFunc init;
    SpriteFunc update;
    SpriteFunc render;
    SpriteFunc destroy;
} SpriteVTable;

typedef struct Sprite {
    u16 x, y;
    u8 flags;
    u16 affineIndex;
    u8 palette;
    Texture* image;
    const SpriteVTable* vt;
    bool active;
} Spr;

extern Spr spritePool[MAX_SPRITES];

// --- Functions ---
void spriteManagerInit();
int  spriteCreate(const SpriteVTable* vt, u16 x, u16 y);
void spriteDestroy(int id);

void spriteManagerUpdate();
void spriteManagerRender();
void spriteManagerDestroyAll();

extern Spr spritePool[MAX_SPRITES];

#endif
