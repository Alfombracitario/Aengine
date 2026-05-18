#include "engine/aengine.h"
#include "scenes/title.h"

//#include "music_ogg.h"
extern Texture* gfxFont;
static void sceneInit() {
    inputMode(true, false);//enable IR
}

static void sceneUpdate() {
    //Game logic
}
static void sceneDraw() {
    camSet2D();
    //Example of a BG gradient.
    const u32 col1 = cBlue;
    const u32 col2 = cBlack;
    drawRectangleColour(0,0,640,240,  col1,col1,col2,col2);
    drawRectangleColour(0,240,640,240,col2,col2,col1,col1);
    
}
static void sceneExit() {
    //Free resources
}

const Scene title = {
    sceneInit,
    sceneUpdate,
    sceneDraw,
    sceneExit
};
