#include "engine/aengine.h"

extern Texture* gfxCursor;
extern Texture* gfxMouse;

bool keyboardDown[256];

void keyPress_cb(char key)//no está funcionando de momento
{
    if(key >= 256){
        return;//tecla no válida
    }
    keyboardDown[(u8)key] = true;
}

void platformInit(){
    prepareVideo();
    WPAD_Init();//input Wii
    PAD_Init();//input Gamecube
    ASND_Init();//Sonido
    USB_Initialize();
    MOUSE_Init();
    KEYBOARD_Init(keyPress_cb);
}

void finishFrame(){
    //dibujar punteros
    for (int i = 0; i < 4; i++) {
        // Solo dibujar si el IR es válido y no está en las esquinas o fuera de pantalla
            if (mouse[i].valid &&
                mouse[i].x > 0 && mouse[i].x < 639 &&
                mouse[i].y > 0 && mouse[i].y < 479) {
                draw_sprite(gfxCursor,i,mouse[i].x,mouse[i].y);
            }
        }
    //dibujar mouse
    if (mouse[4].valid &&
        mouse[4].x > 0 && mouse[4].x < 639 &&
        mouse[4].y > 0 && mouse[4].y < 479) {
        draw_texture(gfxMouse,mouse[4].x,mouse[4].y);
    }
    platformRender();
    invalidateTexCache();
}

void platformExit(){
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