#include "engine/aengine.h"

const Scene* currentScene = NULL;

//Defaut project resources
#include "gfxfont_png.h"//defualt font
Texture* gfxFont;

#if platformHasPointer
#include "gfxcursor_png.h"
Texture* gfxCursor;
#endif

#if platformHasUSBmouse
#include "gfxmouse_png.h"
Texture* gfxMouse;
#endif

//Scenes
#include "scenes/title.h"

//------------------------------------------------- TIEPMO DE EJECUCIÓN ----------------------------------------||
int main(){
    platformInit(); 

    gfxFont  = textureLoad(gfxfont_png);

    #if platformHasUSBmouse
        gfxMouse = textureLoad(gfxmouse_png);
    #endif

    #if platformHasPointer
        gfxCursor = textureLoad(gfxcursor_png);
        initTiles(gfxCursor, 2, 2);
    #endif
    
    //Asignar room inicial
    sceneChange(&title);

    while (1) {
        inputUpdate();
        if(buttonsDown[0] & keyHome){
            sceneEvent(SCENE_EVENT_EXIT);
            break;
        }
        sceneEvent(SCENE_EVENT_UPDATE);
        sceneEvent(SCENE_EVENT_DRAW);
        
        finishFrame();
    }
    #if platformHasUSBmouse
        textureFree(gfxMouse);
    #endif
    #if platformHasPointer
        textureFree(gfxCursor);
    #endif
    textureFree(gfxFont);
    platformExit();
}