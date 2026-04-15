#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#define MAX_SPRITES 128

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
    u16 tile;
    const SpriteVTable* vt;
    bool active;
} Sprite;

// --- Functions ---
void spriteManagerInit();
int  spriteCreate(const SpriteVTable* vt, u16 x, u16 y);
void spriteDestroy(int id);

void spriteManagerUpdate();
void spriteManagerRender();
void spriteManagerDestroyAll();

extern Sprite spritePool[MAX_SPRITES];

#endif
