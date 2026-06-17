#include "engine/aengine.h"
#include "objects/player.h"

int playerID = -1;

// 1) Tabla virtual del player
const SpriteVTable playerVT = {
    playerInit,
    playerUpdate,
    playerRender,
    playerDestroy
};

// 2) Implementaciones
void playerInit(int id) {
    Sprite* inst = &spritePool[id];
}

void playerUpdate(int id) {
}

void playerRender(int id) {
}

void playerDestroy(int id){

}
