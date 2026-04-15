#include <gccore.h>
#include <ogc/pad.h>

#include "engine/gc/core.h"
#include "engine/input.h"

u32 buttonsDown[platformTotalControls] = {0,0,0,0};
u32 buttonsHeld[platformTotalControls] = {0,0,0,0};
u32 buttonsUp[platformTotalControls]   = {0,0,0,0};

stick_t joystickL[platformTotalControls];
stick_t joystickR[platformTotalControls];

u32 inputFlags = 0;

//STUB
mouse_t mouse[USBmouseIndex+1] = {};
bool keyboardDown[1] = {};

void inputMode(bool enableIR, bool enableMotion) {}
bool inputShaken(int index, float fuerzaMinima) { return false; }
void inputTilt(int index, float* x, float* y, float* z) {}
void inputSetHorizontalMode() {}
void inputSetVerticalMode() {}

// ======================================================================
//  INPUT RAW SEPARADO (SIN EMPAQUETAR) - LISTO PARA VOLVER A EMPAQUAR
// ======================================================================

void inputUpdate() {
    PAD_ScanPads();    // Gamecube
    
    for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++){
        //----------------------------------------------------------------------
        // 1) Leer botones crudos (SIN MAPEO / SIN MEZCLAR)
        //----------------------------------------------------------------------
        u32 gc[3];
        #define Down 0
        #define Held 1
        #define Up 2

        gc[Down]   = PAD_ButtonsDown(i);
        gc[Held]   = PAD_ButtonsHeld(i);
        gc[Up]     = PAD_ButtonsUp(i);

        //----------------------------------------------------------------------
        // 5) VARIABLES SEPARADAS - CLASSIC CONTROLLER / PRO CONTROLLER
        //----------------------------------------------------------------------
        bool cc_A      = false, cc_B = false, cc_X = false, cc_Y = false;
        bool cc_L      = false, cc_R = false, cc_ZL = false, cc_ZR = false;
        bool cc_Plus   = false, cc_Minus = false, cc_Home = false;
        bool cc_Up     = false, cc_Down = false, cc_Left = false, cc_Right = false;

        s16 cc_lx = 0, cc_ly = 0, cc_rx = 0, cc_ry = 0;   // Left/Right sticks

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

        u32 input = 0;
        //----------------------------------------------------------------------
        // Helper macros
        //----------------------------------------------------------------------
        #define MAP_BTN(condition, bit)     if(condition){ input |= bit; }

        if (gc[Held]||gc[Up]) {
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
    }    
}
bool keyboardCheck(int vk){return false;}
