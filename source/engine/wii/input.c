#include <ogc/pad.h>
#include <ogc/usbmouse.h>
#include <wiikeyboard/keyboard.h>
#include <wiiuse/wpad.h>

#include "engine/wii/core.h"
#include "engine/input.h"
#include "engine/gc/render.h"//para la resolución

u32 buttonsDown[platformTotalControls] = {0,0,0,0};
u32 buttonsHeld[platformTotalControls] = {0,0,0,0};
u32 buttonsUp[platformTotalControls]   = {0,0,0,0};

//input de USB
mouse_t mouse[USBmouseIndex+1];
mouse_event event;

stick_t joystickL[platformTotalControls];
stick_t joystickR[platformTotalControls];

u32 inputFlags = 0;
#define flagHorizontalDpad 1

// Configura el formato de datos de los Wiimotes según las opciones proporcionadas

// ======================================================================
//  INPUT RAW SEPARADO (SIN EMPAQUETAR) - LISTO PARA VOLVER A EMPAQUAR
// ======================================================================

void inputUpdate() {
    WPAD_ScanPads();   // WiiMote / Nunchuck / Pro / Classic
    PAD_ScanPads();    // Gamecube
    
    for (int i = 0; i < 4; i++) {
        ir_t irData;
        WPAD_IR(i, &irData);

        mouse[i].prevX = mouse[i].x;
        mouse[i].prevY = mouse[i].y;
        mouse[i].x     = irData.x;
        mouse[i].y     = irData.y;
        mouse[i].valid = irData.valid;

    for (int j = 0; j < 3; j++){
        //----------------------------------------------------------------------
        // 1) Leer botones crudos (SIN MAPEO / SIN MEZCLAR)
        //----------------------------------------------------------------------
        u32 wii[3];
        u32 gc[3];
        #define Down 0
        #define Held 1
        #define Up 2

        wii[Down]  = WPAD_ButtonsDown(i);
        wii[Held]  = WPAD_ButtonsHeld(i);
        wii[Up]    = WPAD_ButtonsUp(i);

        gc[Down]   = PAD_ButtonsDown(i);
        gc[Held]   = PAD_ButtonsHeld(i);
        gc[Up]     = PAD_ButtonsUp(i);

        //----------------------------------------------------------------------
        // 2) Detectar expansión (Nunchuck / Classic / Ninguno)
        //----------------------------------------------------------------------
        expansion_t exp;
        WPADData* data = WPAD_Data(i);

        bool hasNunchuck = (data->exp.type == WPAD_EXP_NUNCHUK);
        bool hasClassic  = (data->exp.type == WPAD_EXP_CLASSIC);  // Classic / Pro

        //----------------------------------------------------------------------
        // 3) VARIABLES SEPARADAS - WIIMOTE BOTONES
        //----------------------------------------------------------------------
        bool wiimote_A      = (wii[j] & WPAD_BUTTON_A);
        bool wiimote_B      = (wii[j] & WPAD_BUTTON_B);
        bool wiimote_1      = (wii[j] & WPAD_BUTTON_1);
        bool wiimote_2      = (wii[j] & WPAD_BUTTON_2);
        bool wiimote_Plus   = (wii[j] & WPAD_BUTTON_PLUS);
        bool wiimote_Minus  = (wii[j] & WPAD_BUTTON_MINUS);
        bool wiimote_Home   = (wii[j] & WPAD_BUTTON_HOME);

        bool wiimote_Up     = (wii[j] & WPAD_BUTTON_UP);
        bool wiimote_Down   = (wii[j] & WPAD_BUTTON_DOWN);
        bool wiimote_Left   = (wii[j] & WPAD_BUTTON_LEFT);
        bool wiimote_Right  = (wii[j] & WPAD_BUTTON_RIGHT);

        //----------------------------------------------------------------------
        // 4) VARIABLES SEPARADAS - NUNCHUCK
        //----------------------------------------------------------------------
        bool nunchuck_Z   = false;
        bool nunchuck_C   = false;
        s8   nch_x        = 0;
        s8   nch_y        = 0;

        if (hasNunchuck) {
            WPADData* data = WPAD_Data(i);
            if (data && data->exp.type == WPAD_EXP_NUNCHUK) {
                nunchuck_Z = (data->exp.nunchuk.btns & NUNCHUK_BUTTON_Z);
                nunchuck_C = (data->exp.nunchuk.btns & NUNCHUK_BUTTON_C);
                if(j == Held){
                    nch_x = data->exp.nunchuk.js.pos.x;
                    nch_y = data->exp.nunchuk.js.pos.y;

                    //convertir y empacar directamente
                    
                    //PLACEHOLDER
                }
            }
        }

        //----------------------------------------------------------------------
        // 5) VARIABLES SEPARADAS - CLASSIC CONTROLLER / PRO CONTROLLER
        //----------------------------------------------------------------------
        bool cc_A      = false, cc_B = false, cc_X = false, cc_Y = false;
        bool cc_L      = false, cc_R = false, cc_ZL = false, cc_ZR = false;
        bool cc_Plus   = false, cc_Minus = false, cc_Home = false;
        bool cc_Up     = false, cc_Down = false, cc_Left = false, cc_Right = false;

        s16 cc_lx = 0, cc_ly = 0, cc_rx = 0, cc_ry = 0;   // Left/Right sticks

        if (hasClassic) {//placeholder
            WPADData* data = WPAD_Data(i);
            if (data && data->exp.type == WPAD_EXP_CLASSIC) {
                u32 b = data->exp.classic.btns;

                cc_A = (b & CLASSIC_CTRL_BUTTON_A);
                cc_B = (b & CLASSIC_CTRL_BUTTON_B);
                cc_X = (b & CLASSIC_CTRL_BUTTON_X);
                cc_Y = (b & CLASSIC_CTRL_BUTTON_Y);

                cc_L  = (b & CLASSIC_CTRL_BUTTON_FULL_L);
                cc_R  = (b & CLASSIC_CTRL_BUTTON_FULL_R);
                cc_ZL = (b & CLASSIC_CTRL_BUTTON_ZL);
                cc_ZR = (b & CLASSIC_CTRL_BUTTON_ZR);

                cc_Plus  = (b & CLASSIC_CTRL_BUTTON_PLUS);
                cc_Minus = (b & CLASSIC_CTRL_BUTTON_MINUS);
                cc_Home  = (b & CLASSIC_CTRL_BUTTON_HOME);

                cc_Up    = (b & CLASSIC_CTRL_BUTTON_UP);
                cc_Down  = (b & CLASSIC_CTRL_BUTTON_DOWN);
                cc_Left  = (b & CLASSIC_CTRL_BUTTON_LEFT);
                cc_Right = (b & CLASSIC_CTRL_BUTTON_RIGHT);

                cc_lx = data->exp.classic.ljs.pos.x;
                cc_ly = data->exp.classic.ljs.pos.y;
                cc_rx = data->exp.classic.rjs.pos.x;
                cc_ry = data->exp.classic.rjs.pos.y;
            }
        }

        //----------------------------------------------------------------------
        // 6) VARIABLES SEPARADAS - GAMECUBE
        //----------------------------------------------------------------------
        bool gc_A   = (gc[j] & PAD_BUTTON_A);
        bool gc_B   = (gc[j] & PAD_BUTTON_B);
        bool gc_X   = (gc[j] & PAD_BUTTON_X);
        bool gc_Y   = (gc[j] & PAD_BUTTON_Y);
        bool gc_L   = (gc[j] & PAD_TRIGGER_L);
        bool gc_R   = (gc[j] & PAD_TRIGGER_R);
        bool gc_Z   = (gc[j] & PAD_TRIGGER_Z);
        bool gc_Start = (gc[j] & PAD_BUTTON_START);

        bool gc_Up     = (gc[j] & PAD_BUTTON_UP);
        bool gc_Down   = (gc[j] & PAD_BUTTON_DOWN);
        bool gc_Left   = (gc[j] & PAD_BUTTON_LEFT);
        bool gc_Right  = (gc[j] & PAD_BUTTON_RIGHT);

        // Joystick izquierdo GameCube
        s8 gc_lx = PAD_StickX(i);
        s8 gc_ly = PAD_StickY(i);

        // Joystick derecho GameCube
        s8 gc_rx = PAD_SubStickX(i);
        s8 gc_ry = PAD_SubStickY(i);

        //----------------------------------------------------------------------
        // 7) EMPAQUE FINAL UNIFICADO (bitmask del motor)
        //----------------------------------------------------------------------

        u32 input = 0;
        //----------------------------------------------------------------------
        // Helper macros
        //----------------------------------------------------------------------
        #define MAP_BTN(condition, bit)     if(condition){ input |= bit; }

        //----------------------------------------------------------------------
        // PRIORIDAD 1: Classic / PRO (si existe)
        //----------------------------------------------------------------------
        if (hasClassic) {
            MAP_BTN(cc_A,      keyA);
            MAP_BTN(cc_B,      keyB);
            MAP_BTN(cc_X,      keyX);
            MAP_BTN(cc_Y,      keyY);

            MAP_BTN(cc_L,      keyL);
            MAP_BTN(cc_R,      keyR);
            MAP_BTN(cc_ZL,     keyZL);
            MAP_BTN(cc_ZR,     keyZR);

            MAP_BTN(cc_Plus,   keyStart);
            MAP_BTN(cc_Minus,  keySelect);
            MAP_BTN(cc_Home,   keyHome);

            MAP_BTN(cc_Up,     padU);
            MAP_BTN(cc_Down,   padD);
            MAP_BTN(cc_Left,   padL);
            MAP_BTN(cc_Right,  padR);

            // Stick buttons en el futuro (si alguna plataforma los trae)
            // Aquí podrías detectar clicks si la plataforma lo requiere

            // No hacemos "down/up" porque classic no provee estados separados fácilmente.
        }

        //----------------------------------------------------------------------
        // PRIORIDAD 2: GameCube (si NO hay classic)
        //----------------------------------------------------------------------
        else if (gc[Held]||gc[Up]) {
            MAP_BTN(gc_A,      keyA);
            MAP_BTN(gc_B,      keyB);
            MAP_BTN(gc_X,      keyX);
            MAP_BTN(gc_Y,      keyY);

            MAP_BTN(gc_L,      keyL);
            MAP_BTN(gc_R,      keyR);
            MAP_BTN(gc_Z,      keyZL);

            MAP_BTN(gc_Start,  keyStart);

            MAP_BTN(gc_Up,     padU);
            MAP_BTN(gc_Down,   padD);
            MAP_BTN(gc_Left,   padL);
            MAP_BTN(gc_Right,  padR);
        }

        //----------------------------------------------------------------------
        // PRIORIDAD 3: Nunchuck (mezcla con Wiimote)
        //----------------------------------------------------------------------
        if (hasNunchuck) {
            MAP_BTN(nunchuck_C, keyR);    // C → R
            MAP_BTN(nunchuck_Z, keyZL);   // Z → Trigger extra
        }

        //----------------------------------------------------------------------
        // PRIORIDAD 4: Wiimote solo / complementos
        //----------------------------------------------------------------------
        MAP_BTN(wiimote_A,      keyA);
        MAP_BTN(wiimote_B,      keyB);
        MAP_BTN(wiimote_1,      keyX);
        MAP_BTN(wiimote_2,      keyY);

        MAP_BTN(wiimote_Plus,   keyStart);
        MAP_BTN(wiimote_Minus,  keySelect);
        MAP_BTN(wiimote_Home,   keyHome);

        // Cruceta Wii
        if(inputFlags & flagHorizontalDpad){
            MAP_BTN(wiimote_Up,     padL);
            MAP_BTN(wiimote_Down,   padR);
            MAP_BTN(wiimote_Left,   padD);
            MAP_BTN(wiimote_Right,  padU);
        }
        else{
            MAP_BTN(wiimote_Up,     padU);
            MAP_BTN(wiimote_Down,   padD);
            MAP_BTN(wiimote_Left,   padL);
            MAP_BTN(wiimote_Right,  padR);
        }


    //empacar
    switch(j){
        case 0://buttons down
            buttonsDown[i] = input;
        break;

        case 1://buttons held
            buttonsHeld[i] = input;
        break;

        case 2://buttons up
            buttonsUp[i] = input;
        break;
    }
    }//fin for j


    //mouse
    while(MOUSE_GetEvent(&event))
    {
        mouse[4].x+=event.rx;
        mouse[4].y+=event.ry;
        mouse[4].valid = true;
    }
    //Limitar posición del mouse
    if(mouse[4].x < 0){
            mouse[4].x = 0;
    }
    if(mouse[4].y < 0){
            mouse[4].y  = 0;
    }
    if(mouse[4].x > gpuS.viewWidth){
            mouse[4].x = gpuS.viewWidth;
    }
    if(mouse[4].y > gpuS.viewHeight){
            mouse[4].y = gpuS.viewHeight;
    }
    }

    //limpiar teclas del teclado
    for(int i = 0; i < 256; i++){
        //keyboardDown[i] = false;
    }
    
}

void inputSetHorizontalMode(){
    inputFlags |= flagHorizontalDpad;
}

void inputSetVerticalMode(){
    inputFlags &= ~flagHorizontalDpad;
}

void inputMode(bool enableIR, bool enableMotion) {
    for (int i = 0; i < 4; i++) {
        u32 format = WPAD_FMT_BTNS; // base: solo botones

        if (enableMotion && enableIR)
            format = WPAD_FMT_BTNS_ACC_IR;
        else if (enableMotion)
            format = WPAD_FMT_BTNS_ACC;
        else if (enableIR)
            format = WPAD_FMT_BTNS_ACC_IR;//no se puede solo IR sin motion

        WPAD_SetDataFormat(i, format);

        if (enableIR)
            WPAD_SetVRes(i, 640, 480); // resolución estándar del IR
    }
}

// función para manejar qué tanto se agita el wiimote
bool inputShaken(int index, float fuerzaMinima) {
    WPADData *wd = WPAD_Data(index);
    if (!wd) return false;

    if (wd->exp.type != WPAD_EXP_NONE && wd->exp.type != WPAD_EXP_NUNCHUK)
        return false;

    float ax = wd->gforce.x;
    float ay = wd->gforce.y;
    float az = wd->gforce.z;

    float magnitudeSq = ax*ax + ay*ay + az*az;

    return (magnitudeSq > fuerzaMinima * fuerzaMinima);
}

void inputTilt(int index, float* x, float* y, float* z) {
    WPADData* data = WPAD_Data(index);
    *x = data->accel.x - 128.0f;
    *y = data->accel.y - 128.0f;
    *z = data->accel.z - 128.0f;
}

bool keyboardCheck(int vk)
{
    return keyboardDown[(u8)vk];
}