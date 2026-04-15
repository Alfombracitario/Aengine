#pragma once
#include <gccore.h>
#include "engine/objectmanager.h"

#define fromX 0
#define fromY 1

// Función de colisión 2D
typedef void (*TileCollide)(Instance* p, int x, int y, int from);

// Función de dibujo opcional
typedef void (*TileDraw)(int x, int y);

// Información por tile
typedef struct {
    u16 uvFront;   // índice de sprite en atlas
    u16 uvLeft;
    u16 uvRight;
    u16 uvTop;
    u16 uvBottom;
    TileCollide collide;  // NULL si no pasa nada
    TileDraw draw;        // NULL si genérico
} TileInfo;

// Banco global ( <= 256 tiles activos )
extern TileInfo tileSet[256];
extern u8 tileCount;
extern u16 sheetHeight;
