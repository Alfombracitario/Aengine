#include "engine/aengine.h"

u16 buttonsDown[platformTotalControls] = {0,0};
u16 buttonsHeld[platformTotalControls] = {0,0};
u16 buttonsUp[platformTotalControls]   = {0,0};

u32 inputFlags = 0;

//intentaré quitar esto como pueda.
stick_t joystickL[platformTotalControls];
stick_t joystickR[platformTotalControls];

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
    buttonsDown[0] = padsCurrent(0);
    buttonsDown[1] = padsCurrent(1);
}

bool keyboardCheck(int vk){return false;}
