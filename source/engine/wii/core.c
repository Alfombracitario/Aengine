#include "engine/aengine.h"

extern Texture* gfxCursor;
extern Texture* gfxMouse;

bool keyboardDown[256];

void platformInit(){
    prepareVideo();
    WUPC_Init();//Input vWii
    WPAD_Init();//input Wii
    PAD_Init();//input Gamecube
    ASND_Init();//Sonido
    USB_Initialize();
    MOUSE_Init();
    KEYBOARD_Init(NULL);
}

void finishFrame(){
    //dibujar punteros
    gpuSetTextureMode(true);
    camSet2D();
    for (int i = 0; i < 4; i++) {
        // Solo dibujar si el IR es válido y no está en las esquinas o fuera de pantalla
            if (mouse[i].valid &&
                mouse[i].x > 0 && mouse[i].x < 639 &&
                mouse[i].y > 0 && mouse[i].y < 479) {
                drawSprite(gfxCursor,i,mouse[i].x,mouse[i].y);
            }
        }
    //dibujar mouse
    if (mouse[4].valid &&
        mouse[4].x > 0 && mouse[4].x < 639 &&
        mouse[4].y > 0 && mouse[4].y < 479) {
        drawTexture(gfxMouse,mouse[4].x,mouse[4].y);
    }
    platformRender();
    invalidateTexCache();
}

void platformExit(){
    WUPC_Shutdown();
    for(int i = 0; i < 4; i++){
        WPAD_SetDataFormat(i, WPAD_FMT_BTNS); // resetear a modo simple
        WPAD_Disconnect(i); // desconectar cada wiimote
    }
    KEYBOARD_Deinit();
    MOUSE_Deinit();
    USB_Deinitialize();
    ASND_End();
    WPAD_Shutdown();
    exitVideo();
    exit(0);
}