#pragma once
#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H
#include "engine/aengine.h"
#if platformHasFPU && platformHas3D
#include <stdint.h>
#include "engine/texture.h"

#define MAX_INSTANCES 256

typedef void (*InstanceFunc)(int id);

typedef struct InstanceVTable {
    InstanceFunc init;
    InstanceFunc update;
    InstanceFunc render;
    InstanceFunc destroy;
} InstanceVTable;

typedef struct Instance {
    float x, y, z;
    float sx, sy, sz;

    float hitboxWidth;
    float hitboxHeight;
    float hitboxDepth;
    
    float xScale;
    float yScale;
    float zScale;
    float xRotation;
    float yRotation;
    float zRotation;

    Texture* image;      // sprite
    float animFrame;
    float animSpeed;
    uint32_t color;

    int16_t hp;

    uint32_t vars[8];    // variables personalizadas

    const InstanceVTable* vt;
    bool active;
} Instance;

// --- Functions ---
void instanceManager_Init();
int  instanceCreate(const InstanceVTable* vt, float x, float y, float z);
void instanceDestroy(int id);

void instanceManagerUpdate();
void instanceManagerRender();
void instanceManagerDestroyAll();

extern Instance instancePool[MAX_INSTANCES];

#endif
#endif