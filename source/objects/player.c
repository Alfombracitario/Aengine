#include "objects/player.h"
#include "engine/aengine.h"

int playerID = -1;

extern u32 roomWidth;
extern u32 roomHeight;

// 1) Tabla virtual del player
const InstanceVTable playerVT = {
    playerInit,
    playerUpdate,
    playerRender,
    playerDestroy
};

// 2) Implementaciones
void playerInit(int id) {
    Instance* inst = &instancePool[id];
    inst->hp = 3;
    inst->sx = 0;
    inst->sy = 0;
    inst->xScale = 1;
    inst->yScale = 1;
    inst->image = NULL;
    inst->hitboxHeight = 1.0f;
    inst->hitboxWidth = 1.0f;
}

void playerUpdate(int id) {
}

void playerRender(int id) {
}

void playerDestroy(int id) {
}
