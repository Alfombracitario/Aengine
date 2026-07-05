#include "engine/spritemanager.h"

Spr spritePool[MAX_SPRITES];

void spriteManagerInit(){
    for (int i = 0; i < MAX_SPRITES; i++) {
        spritePool[i].active = false;
        spritePool[i].vt = NULL;
    }
}

int spriteCreate(const SpriteVTable* vt, u16 x, u16 y){
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (!spritePool[i].active){
            Spr* spr = &spritePool[i];
            spr->active = true;
            spr->vt = vt;
            spr->x = x;
            spr->y = y;
            spr->image = NULL;
            spr->palette = 0;

            if (vt && vt->init) vt->init(i);

            return i;
        }
    }
    return -1; // no slots free
}

void spriteDestroy(int id) {
    Spr* spr = &spritePool[id];
    if (spr->active && spr->vt && spr->vt->destroy) {
        spr->vt->destroy(id);
    }
    spr->active = false;
    spr->vt = NULL;
}

void spriteManagerUpdate() {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (spritePool[i].active) {
            if (spritePool[i].vt && spritePool[i].vt->update)
                spritePool[i].vt->update(i);
        }
    }
}

void spriteManagerRender() {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (spritePool[i].active) {
            if (spritePool[i].vt && spritePool[i].vt->render)
                spritePool[i].vt->render(i);
        }
    }
}

void spriteManagerDestroyAll() {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (spritePool[i].active)
            spriteDestroy(i);
    }
}