#ifndef INPUT_H
#define INPUT_H

#include "engine/aengine.h"
//funciones en compileTime
#define buttonsCheckHeld(p, key)   (buttonsHeld[p] & (key))
#define buttonsCheckDown(p, key)   (buttonsDown[p] & (key))
#define buttonsCheckUp(p, key)     (buttonsUp[p] & (key))

#define controlsPointersCount (platformTotalControls*platformHasPointer)
#define touchIndex    controlsPointersCount+platformHasTouch
#define USBmouseIndex touchIndex+platformHasUSBmouse

extern bool keyboardDown[(255*platformHasUSBkeyboard)+1];

//constantes
#if CURRENT_PLATFORM == PLATFORM_SNES//SNES está optimizado en compile time.
#include <snes.h>

extern u16 buttonsDown[platformTotalControls];
extern u16 buttonsHeld[platformTotalControls];
extern u16 buttonsUp[platformTotalControls];

#define keyA    KEY_A
#define keyB    KEY_B
#define keyX    KEY_X
#define keyY    KEY_Y

#define faceU   keyX
#define faceD   keyB
#define faceL   keyY
#define faceR   keyA

#define padU    KEY_UP
#define padD    KEY_DOWN
#define padL    KEY_LEFT
#define padR    KEY_RIGHT

#define keyStart KEY_START
#define keySelect KEY_SELECT

#define keyHome (KEY_START|KEY_SELECT)
#else

typedef struct { u32 src; u32 dst; } BtnMap;

inline u32 remapBits(u32 raw, const BtnMap* map, int len) {
    u32 out = 0;
    for (int i = 0; i < len; i++)
        if (raw & map[i].src) out |= map[i].dst;
    return out;
}

extern u32 buttonsDown[platformTotalControls];
extern u32 buttonsHeld[platformTotalControls];
extern u32 buttonsUp[platformTotalControls];

typedef struct {
    int x, y;
    int rx, ry;
    int prevX, prevY;
    bool valid;
} mouse_t;

typedef struct {
    float x, y, deadZone;
} stick_t;

extern mouse_t mouse[USBmouseIndex+1];

extern stick_t joystickR[platformTotalControls];
extern stick_t joystickL[platformTotalControls];



#define keyA    (1<<0)   // Acción principal
#define keyB    (1<<1)   // Acción secundaria
#define keyX    (1<<2)   // Acción terciaria
#define keyY    (1<<3)   // Acción alternativa

#define faceU   keyX     //variantes para el resto de plataformas
#define faceD   keyB
#define faceL   keyY
#define faceR   keyA

#define key1    keyX   // Variantes en Wiimote
#define key2    keyY

#define padU    (1<<4)
#define padD    (1<<5)
#define padL    (1<<6)
#define padR    (1<<7)

#define keyL    (1<<8)   // L digital / bumper izq
#define keyR    (1<<9)   // R digital / bumper der

#define keyC    keyL   // Versión Nunchuck
#define keyZ    keyR

#define keyZL   (1<<10)  // Trigger extra
#define keyZR   (1<<11)

#define keyStart (1<<12)
#define keySelect (1<<13)
#define keyHome (1<<14)

#define clickStickL (1<<15)
#define clickStickR (1<<16)

#define clickL (1<<17)//click izquierdo del mouse
#define clickR (1<<18)
#define clickM (1<<19)

#define backL (1<<20)//botones de atrás (switch 2)
#define backR (1<<21)

#define keyCapture (1<<22)

#endif

//funciones
void inputUpdate();
void inputMode(bool enableIR, bool enableMotion);
bool inputShaken(int index, float fuerzaMinima);
void inputTilt(int index, float* x, float* y, float* z);

void inputSetHorizontalMode();
void inputSetVerticalMode();

//game maker input method

//macros
// --- Básicas ---
#define vk_anykey      0
#define vk_nokey      -1

#define vk_backspace   8
#define vk_tab         9
#define vk_enter      13
#define vk_shift      16
#define vk_control    17
#define vk_alt        18
#define vk_pause      19
#define vk_escape     27
#define vk_space      32

// --- Navegación ---
#define vk_pageup     33
#define vk_pagedown   34
#define vk_end        35
#define vk_home       36

#define vk_left       37
#define vk_up         38
#define vk_right      39
#define vk_down       40

// --- Edición ---
#define vk_insert     45
#define vk_delete     46

// --- Teclas especiales ---
#define vk_printscreen 44
#define vk_help       47

// --- Numpad ---
#define vk_numpad0    96
#define vk_numpad1    97
#define vk_numpad2    98
#define vk_numpad3    99
#define vk_numpad4   100
#define vk_numpad5   101
#define vk_numpad6   102
#define vk_numpad7   103
#define vk_numpad8   104
#define vk_numpad9   105

#define vk_multiply  106
#define vk_add       107
#define vk_subtract  109
#define vk_decimal   110
#define vk_divide    111

// --- Function keys ---
#define vk_f1        112
#define vk_f2        113
#define vk_f3        114
#define vk_f4        115
#define vk_f5        116
#define vk_f6        117
#define vk_f7        118
#define vk_f8        119
#define vk_f9        120
#define vk_f10       121
#define vk_f11       122
#define vk_f12       123

// --- Lock keys ---
#define vk_capslock   20
#define vk_numlock   144
#define vk_scrolllock 145

//teclado físico
#if platformHasUSBkeyboard
    bool keyboardCheck(int vk);
    bool keyboardCheckPressed(int vk);
    bool keyboardCheckReleased(int vk);
    bool keyboardCheckAny();
#endif

#endif
