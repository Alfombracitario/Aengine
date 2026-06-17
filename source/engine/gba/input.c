#include "engine/aengine.h"

u32 buttonsDown[1] = {0};
u32 buttonsHeld[1] = {0};
u32 buttonsUp[1] = {0};

//STUB
mouse_t mouse[USBmouseIndex+1] = {};

void inputMode(bool enableIR, bool enableMotion) {}
bool inputShaken(int index, float fuerzaMinima) { return false; }
void inputTilt(int index, float* x, float* y, float* z) {}
void inputSetHorizontalMode() {}
void inputSetVerticalMode() {}

static const BtnMap gba_map[] = {
    { 1,   keyA      },  // bit 0
    { 2,   keyB      },  // bit 1
    { 4,   keySelect },  // bit 2
    { 8,   keyStart  },  // bit 3
    { 16,  padR      },  // bit 4
    { 32,  padL      },  // bit 5
    { 64,  padD      },  // bit 6
    { 128, padU      },  // bit 7
    { 256, keyR      },  // bit 8
    { 512, keyL      },  // bit 9
};

#define GBA_MAP_LEN          (sizeof(gba_map) / sizeof(gba_map[0]))

void inputUpdate() {
    static u16 prev = 0;
    u16 keys = ~REG_KEYINPUT & 0x03FF;

    prev          = buttonsHeld[0];
    buttonsHeld[0] = remapBits(keys, gba_map, GBA_MAP_LEN);
    buttonsDown[0] = buttonsHeld[0] & ~prev;
    buttonsUp[0]   = ~buttonsHeld[0] & prev;
}