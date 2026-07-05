#include "engine/aengine.h"
#include "scenes/title.h"
#include "engine/spritemanager.h"
#include "objects/player.h"

//assets
#include "icon_acs.h"
//#include "music_ogg.h"
extern Texture* gfxFont;
Texture* gfxIcon;

static void sceneInit() {
    inputMode(true, false);//enable IR
    gfxIcon = loadSprite(icon_acs,icon_acs_size,FMT_ACS|GFX_8BPP);
    spriteManagerInit();

    playerID = spriteCreate(&playerVT, 0, 0);
    
}

static void sceneUpdate() {
    
}
static void sceneDraw() {    
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
