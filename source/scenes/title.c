#include "engine/aengine.h"
#include "scenes/title.h"

//#include "music_ogg.h"
extern Texture* gfxFont;
static void sceneInit() {
    inputMode(true, false);//enable IR
    //PlayOgg(music_ogg, music_ogg_size, 0, OGG_INFINITE_TIME);
}

static void sceneUpdate() {
    //Game logic
}
static void sceneDraw() {
    setCamera2D();
    //Example of a BG gradient.
    const u32 col1 = c_blue;
    const u32 col2 = c_black;
    draw_rectangle_colour(0,0,640,240,  col1,col1,col2,col2);
    draw_rectangle_colour(0,240,640,240,col2,col2,col1,col1);
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
