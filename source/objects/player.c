#include "engine/aengine.h"
#include "objects/player.h"

int playerID = -1;
extern Texture gfxIcon;

// 1) Tabla virtual del player
const SpriteVTable playerVT = {
    playerInit,
    playerUpdate,
    playerRender,
    playerDestroy
};

void playerInit(int id) {
    Spr* spr = &spritePool[id];
    spr->image = &gfxIcon;
}

void playerUpdate(int id) {
}

void playerRender(int id) {
}

void playerDestroy(int id){

}