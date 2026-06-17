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


static const BtnMap gc_map[] = {
    { PAD_BUTTON_A,     keyA     },
    { PAD_BUTTON_B,     keyB     },
    { PAD_BUTTON_X,     keyX     },
    { PAD_BUTTON_Y,     keyY     },
    { PAD_TRIGGER_L,    keyL     },
    { PAD_TRIGGER_R,    keyR     },
    { PAD_TRIGGER_Z,    keyZL    },
    { PAD_BUTTON_START, keyStart },
    { PAD_BUTTON_UP,    padU     },
    { PAD_BUTTON_DOWN,  padD     },
    { PAD_BUTTON_LEFT,  padL     },
    { PAD_BUTTON_RIGHT, padR     },
};
#define GC_MAP_LEN (sizeof(gc_map) / sizeof(gc_map[0]))

void inputUpdate() {
    PAD_ScanPads();

    for (int p = 0; p < platformTotalControls; p++) {
        buttonsDown[p] = remapBits(PAD_ButtonsDown(p), gc_map, GC_MAP_LEN);
        buttonsHeld[p] = remapBits(PAD_ButtonsHeld(p), gc_map, GC_MAP_LEN);
        buttonsUp[p]   = remapBits(PAD_ButtonsUp(p),   gc_map, GC_MAP_LEN);

        joystickL[p].x = PAD_StickX(p);
        joystickL[p].y = PAD_StickY(p);
        joystickR[p].x = PAD_SubStickX(p);
        joystickR[p].y = PAD_SubStickY(p);
    }
}
bool keyboardCheck(int vk){return false;}
