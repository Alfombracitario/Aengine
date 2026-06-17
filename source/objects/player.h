#pragma once
#include "engine/spritemanager.h"

extern int playerID;

void playerInit(int id);
void playerUpdate(int id);
void playerRender(int id);
void playerDestroy(int id);

extern const SpriteVTable playerVT;
