#include "tiles/tiles.h"
#include "engine/types.h"
u8* tiles = NULL;
u32 roomWidth;
u32 roomHeight;

//colissions
void solid(int tx, int ty, cord* px, cord* py) {
    //collision code here
    return;
}

void empty(int tx, int ty, cord* px, cord* py) {
    return;
}

static void drawNothing(int x, int y ){
    return;
}

TileInfo tileSet[] = {
    // id 0: air
    { 0,0,0,0,0, empty, drawNothing},

    // id 1: solid
    { 1,1,1,2,2, solid, NULL},
};

u8 tileCount = 1;

u16 sheetHeight = 1;//64x1024