#include "engine/aengine.h"
#include "scenes/title.h"

//assets
#include "icon_acs.h"
//#include "music_ogg.h"
extern Texture* gfxFont;
Texture* gfxIcon;

static void sceneInit() {
    inputMode(true, false);//enable IR
    gfxIcon = textureLoad(icon_acs,FMT_ACS);
}

static void sceneUpdate() {
    
}
static void sceneDraw() {
    drawTexture(gfxIcon,0,0);
    
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
