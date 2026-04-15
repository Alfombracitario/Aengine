#include <asndlib.h>
#include "engine/aengine.h"

void platformInit(){
    prepareVideo();
    PAD_Init();//input Gamecube
    ASND_Init();//Sonido
}

void finishFrame(){
    platformRender();
    invalidateTexCache();
}

void platformExit(){
    ASND_End();
    exitVideo();
    exit(0);
}