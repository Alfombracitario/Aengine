#include "engine/objectmanager.h"
#if platformHasFPU && platformHas3D
Instance instancePool[MAX_INSTANCES];

void instanceManager_Init() {
    for (int i = 0; i < MAX_INSTANCES; i++) {
        instancePool[i].active = false;
        instancePool[i].vt = NULL;
    }
}

int instanceCreate(const InstanceVTable* vt, float x, float y, float z) {
    for (int i = 0; i < MAX_INSTANCES; i++) {
        if (!instancePool[i].active) {
            Instance* inst = &instancePool[i];
            inst->active = true;
            inst->vt = vt;

            inst->x = x;
            inst->y = y;
            inst->z = z;
            inst->sx = inst->sy = inst->sz = 0;

            inst->xScale = inst->yScale = inst->zScale = 1;
            inst->xRotation = inst->yRotation = inst->zRotation = 0;
            inst->image = NULL;      // sprite

            inst->animFrame = 0;
            inst->animSpeed = 0;
            inst->color = 0xFFFFFFFF;

            inst->hp = 1;
            for(int j = 0; j < 8; j++) inst->vars[j] = 0;

            if (vt && vt->init) vt->init(i);

            return i;
        }
    }
    return -1; // no slots free
}

void instanceDestroy(int id) {
    Instance* inst = &instancePool[id];
    if (inst->active && inst->vt && inst->vt->destroy) {
        inst->vt->destroy(id);
    }
    inst->active = false;
    inst->vt = NULL;
}

void instanceManagerUpdate() {
    for (int i = 0; i < MAX_INSTANCES; i++) {
        if (instancePool[i].active) {
            if (instancePool[i].vt && instancePool[i].vt->update)
                instancePool[i].vt->update(i);
        }
    }
}

void instanceManagerRender() {
    for (int i = 0; i < MAX_INSTANCES; i++) {
        if (instancePool[i].active) {
            if (instancePool[i].vt && instancePool[i].vt->render)
                instancePool[i].vt->render(i);
        }
    }
}

void instanceManagerDestroyAll() {
    for (int i = 0; i < MAX_INSTANCES; i++) {
        if (instancePool[i].active)
            instanceDestroy(i);
    }
}
#endif