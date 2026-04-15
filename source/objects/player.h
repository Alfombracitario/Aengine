#pragma once
#include "engine/objectmanager.h"

extern int playerID;

void playerInit(int id);
void playerUpdate(int id);
void playerRender(int id);
void playerDestroy(int id);

extern const InstanceVTable playerVT;
