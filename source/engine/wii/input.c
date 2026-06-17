#include <ogc/pad.h>
#include <ogc/usbmouse.h>
#include <wiikeyboard/keyboard.h>
#include <wiiuse/wpad.h>
#include <wupc/wupc.h>

#include <string.h>
#include <math.h>

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

static const BtnMap wii_map[] = {
    { WPAD_BUTTON_A,     keyA     },
    { WPAD_BUTTON_B,     keyB     },
    { WPAD_BUTTON_1,     key1     },
    { WPAD_BUTTON_2,     key2     },
    { WPAD_BUTTON_MINUS, keySelect},
    { WPAD_BUTTON_PLUS,  keyStart },
    { WPAD_BUTTON_HOME,  keyHome  },
};
static const BtnMap dpadV_map[] = {
    { WPAD_BUTTON_UP,   padU     },
    { WPAD_BUTTON_DOWN, padD     },
    { WPAD_BUTTON_LEFT, padL     },
    { WPAD_BUTTON_RIGHT,padR     },
};

static const BtnMap dpadH_map[] = {
    { WPAD_BUTTON_UP,   padL },
    { WPAD_BUTTON_DOWN, padR },
    { WPAD_BUTTON_LEFT, padD },
    { WPAD_BUTTON_RIGHT,padU },
};

static const BtnMap nunchuck_map[] = {
    { NUNCHUK_BUTTON_C, keyC },
    { NUNCHUK_BUTTON_Z, keyZ },
};

static const BtnMap pro_map[] = {
    { CLASSIC_CTRL_BUTTON_A,     keyA    },
    { CLASSIC_CTRL_BUTTON_B,     keyB    },
    { CLASSIC_CTRL_BUTTON_X,     keyX    },
    { CLASSIC_CTRL_BUTTON_Y,     keyY    },
    { CLASSIC_CTRL_BUTTON_FULL_L,keyL    },
    { CLASSIC_CTRL_BUTTON_FULL_R,keyR    },
    { CLASSIC_CTRL_BUTTON_ZL,    keyZL   },
    { CLASSIC_CTRL_BUTTON_ZR,    keyZR   },
    { CLASSIC_CTRL_BUTTON_PLUS,  keyStart },
    { CLASSIC_CTRL_BUTTON_MINUS, keySelect},
    { CLASSIC_CTRL_BUTTON_HOME,  keyHome  },
    { CLASSIC_CTRL_BUTTON_UP,    padU     },
    { CLASSIC_CTRL_BUTTON_DOWN,  padD     },
    { CLASSIC_CTRL_BUTTON_LEFT,  padL     },
    { CLASSIC_CTRL_BUTTON_RIGHT, padR     },
};

typedef struct { u16 ks; u8 vk; } KsEntry;

static const KsEntry KS_MAP[] = {
    { KS_Up,        vk_up        },
    { KS_Down,      vk_down      },
    { KS_Left,      vk_left      },
    { KS_Right,     vk_right     },
    { KS_Return,    vk_enter     },
    { KS_space,     vk_space     },
    { KS_Escape,    vk_escape    },
    { KS_BackSpace, vk_backspace },
    { KS_Tab,       vk_tab       },
    { KS_Delete,    vk_delete    },
    { KS_Insert,    vk_insert    },
    { KS_Home,      vk_home      },
    { KS_End,       vk_end       },
    { KS_Prior,     vk_pageup    },
    { KS_Next,      vk_pagedown  },
    { KS_F1,        vk_f1        },
    { KS_F2,        vk_f2        },
    { KS_F3,        vk_f3        },
    { KS_F4,        vk_f4        },
    { KS_F5,        vk_f5        },
    { KS_F6,        vk_f6        },
    { KS_F7,        vk_f7        },
    { KS_F8,        vk_f8        },
    { KS_F9,        vk_f9        },
    { KS_F10,       vk_f10       },
    { KS_F11,       vk_f11       },
    { KS_F12,       vk_f12       },
    { KS_Shift_L,   vk_shift     },
    { KS_Shift_R,   vk_shift     },
    { KS_Control_L, vk_control   },
    { KS_Control_R, vk_control   },
    { KS_Alt_L,     vk_alt       },
    { KS_Alt_R,     vk_alt       },
    { KS_Caps_Lock, vk_capslock  },
    { KS_Num_Lock,  vk_numlock   },
};

static u8 symToVk(u16 sym) {
    if (sym >= KS_a && sym <= KS_z) return 'A' + (sym - KS_a);
    if (sym >= KS_A && sym <= KS_Z) return sym;
    if (sym >= KS_0 && sym <= KS_9) return sym;

    for (int i = 0; i < (int)(sizeof(KS_MAP) / sizeof(KS_MAP[0])); i++)
        if (KS_MAP[i].ks == sym) return KS_MAP[i].vk;

    return 0;
}

#define KS_MAP_LEN          (sizeof(KS_MAP) / sizeof(KS_MAP[0]))

#define GC_MAP_LEN          (sizeof(gc_map) / sizeof(gc_map[0]))
#define WII_MAP_LEN         (sizeof(wii_map) / sizeof(wii_map[0]))
#define PRO_MAP_LEN         (sizeof(pro_map) / sizeof(pro_map[0]))
#define DPAD_MAP_LEN        (sizeof(dpadH_map) / sizeof(dpadH_map[0]))
#define NUNCHUCK_MAP_LEN    (sizeof(nunchuck_map) / sizeof(nunchuck_map[0]))

//teclado USB
// variables privadas
static bool kbState[256]  = {0};
static bool kbLast[256]   = {0};
static bool kbConnected   = true;

static void keyboardUpdate() {
    // copiar estado anterior
    memcpy(kbLast, kbState, sizeof(kbState));

    // drenar eventos
    keyboard_event ev;
    while (KEYBOARD_GetEvent(&ev)) {
        switch (ev.type) {
            case KEYBOARD_CONNECTED:
                kbConnected = true;
                break;
            case KEYBOARD_DISCONNECTED:
                kbConnected = false;
                memset(kbState, 0, sizeof(kbState));
                break;
            case KEYBOARD_PRESSED:
            case KEYBOARD_RELEASED: {
                u8 vk = symToVk(ev.symbol);
                if (vk) kbState[vk] = (ev.type == KEYBOARD_PRESSED);
                break;
            }
            default: break;
        }
    }
}

// Configura el formato de datos de los Wiimotes según las opciones proporcionadas

// ======================================================================
//  INPUT RAW SEPARADO (SIN EMPAQUETAR) - LISTO PARA VOLVER A EMPAQUAR
// ======================================================================
#define Down 0
#define Held 1
#define Up 2
void inputUpdate() {
    WUPC_UpdateButtonStats();//Wii U
    WPAD_ScanPads();//Wii
    PAD_ScanPads();//GC
    
    for (int i = 0; i < platformTotalControls; i++) {
        // IR → mouse
        ir_t irData;
        WPAD_IR(i, &irData);
        mouse[i].prevX = mouse[i].x;
        mouse[i].prevY = mouse[i].y;
        mouse[i].x     = irData.x;
        mouse[i].y     = irData.y;
        mouse[i].valid = irData.valid;

        // Leer estados crudos
        u32 wii[3], gc[3];
        wii[Down] = WPAD_ButtonsDown(i);
        wii[Held] = WPAD_ButtonsHeld(i);
        wii[Up]   = WPAD_ButtonsUp(i);
        gc[Down]  = PAD_ButtonsDown(i);
        gc[Held]  = PAD_ButtonsHeld(i);
        gc[Up]    = PAD_ButtonsUp(i);
        
        // Detectar expansión
        WPADData* data    = WPAD_Data(i);
        bool hasNunchuck  = (data->exp.type == WPAD_EXP_NUNCHUK);
        bool hasClassic   = (data->exp.type == WPAD_EXP_CLASSIC);
        bool hasWUPC = (WUPC_ButtonsHeld(i) != 0 || WUPC_ButtonsDown(i) != 0);

        // Seleccionar dpad según orientación
        const BtnMap* dpad = (inputFlags & flagHorizontalDpad) ? dpadH_map : dpadV_map;

        for (int s = 0; s < 3; s++) {
            u32 input = 0;
            if (hasWUPC) {
                u32 wupc_raw = (s == Down) ? WUPC_ButtonsDown(i) :
                            (s == Held) ? WUPC_ButtonsHeld(i) : 0;
                input |= remapBits(wii[s], pro_map,     PRO_MAP_LEN);
            }
            else if (hasClassic) {
                input |= remapBits(wii[s], pro_map,      PRO_MAP_LEN);
            } else {
                input |= remapBits(gc[s],  gc_map,       GC_MAP_LEN);
                input |= remapBits(wii[s], wii_map,      WII_MAP_LEN);
                input |= remapBits(wii[s], dpad,         DPAD_MAP_LEN);
                if (hasNunchuck)
                    input |= remapBits(wii[s], nunchuck_map, NUNCHUCK_MAP_LEN);
            }
            switch(s) {
                case Down: buttonsDown[i] = input; break;
                case Held: buttonsHeld[i] = input; break;
                case Up:   buttonsUp[i]   = input; break;
            }
        }

        // Joysticks
        #define INV_JS_RANGE (1.0f / 64.0f)
        #define DEADZONE_SQ 0.01f
        if (hasClassic) {
            float lx = (data->exp.classic.ljs.pos.x - data->exp.classic.ljs.center.x) * INV_JS_RANGE;
            float ly = (data->exp.classic.ljs.pos.y - data->exp.classic.ljs.center.y) * INV_JS_RANGE;

            if (lx * lx + ly * ly < DEADZONE_SQ) {
                joystickL[i].x = 0.0f;
                joystickL[i].y = 0.0f;
            } else {
                joystickL[i].x = fmaxf(-1.0f, fminf(1.0f, lx));
                joystickL[i].y = fmaxf(-1.0f, fminf(1.0f, ly));
            }

            // Right stick
            float rx = (data->exp.classic.rjs.pos.x - data->exp.classic.rjs.center.x) * INV_JS_RANGE;
            float ry = (data->exp.classic.rjs.pos.y - data->exp.classic.rjs.center.y) * INV_JS_RANGE;

            if (rx * rx + ry * ry < DEADZONE_SQ) {
                joystickR[i].x = 0.0f;
                joystickR[i].y = 0.0f;
            } else {
                joystickR[i].x = fmaxf(-1.0f, fminf(1.0f, rx));
                joystickR[i].y = fmaxf(-1.0f, fminf(1.0f, ry));
            }
        } else if (hasNunchuck) {
            float raw_x = (data->exp.nunchuk.js.pos.x - data->exp.nunchuk.js.center.x) * INV_JS_RANGE;
            float raw_y = (data->exp.nunchuk.js.pos.y - data->exp.nunchuk.js.center.y) * INV_JS_RANGE;

            float mag = sqrtf(raw_x * raw_x + raw_y * raw_y);

            if (mag < 0.1f) {
                joystickL[i].x = 0.0f;
                joystickL[i].y = 0.0f;
            } else {
                joystickL[i].x = fmaxf(-1.0f, fminf(1.0f, raw_x));
                joystickL[i].y = fmaxf(-1.0f, fminf(1.0f, raw_y));
            }
        } else {
            // GC como fallback
            joystickL[i].x = PAD_StickX(i)/ 127.0f;
            joystickL[i].y = PAD_StickY(i)/ 127.0f;
            joystickR[i].x = PAD_SubStickX(i)/ 127.0f;
            joystickR[i].y = PAD_SubStickY(i)/ 127.0f;
        }
    }

    // Mouse USB
    mouse[USBmouseIndex].rx = 0;
    mouse[USBmouseIndex].ry = 0;
    mouse[USBmouseIndex].prevX = mouse[USBmouseIndex].x;
    mouse[USBmouseIndex].prevY = mouse[USBmouseIndex].y;
    while (MOUSE_GetEvent(&event)) {
        mouse[USBmouseIndex].x += event.rx;
        mouse[USBmouseIndex].y += event.ry;
        mouse[USBmouseIndex].rx += event.rx;
        mouse[USBmouseIndex].ry += event.ry;
        mouse[USBmouseIndex].valid = true;
    }
    if (mouse[USBmouseIndex].x < 0)              mouse[USBmouseIndex].x = 0;
    if (mouse[USBmouseIndex].y < 0)              mouse[USBmouseIndex].y = 0;
    if (mouse[USBmouseIndex].x > gpuS.viewWidth)  mouse[USBmouseIndex].x = gpuS.viewWidth;
    if (mouse[USBmouseIndex].y > gpuS.viewHeight) mouse[USBmouseIndex].y = gpuS.viewHeight;

    keyboardUpdate();
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

bool keyboardCheck(int vk) {
    if (!kbConnected || vk < 0 || vk >= 256) return false;
    if (vk == vk_anykey) {
        for (int i = 1; i < 256; i++)
            if (kbState[i]) return true;
        return false;
    }
    return kbState[(u8)vk];
}

bool keyboardCheckPressed(int vk) {
    if (!kbConnected || vk < 0 || vk >= 256) return false;
    return kbState[(u8)vk] && !kbLast[(u8)vk];
}

bool keyboardCheckReleased(int vk) {
    if (!kbConnected || vk < 0 || vk >= 256) return false;
    return !kbState[(u8)vk] && kbLast[(u8)vk];
}

bool keyboardCheckAny() {
    return keyboardCheck(vk_anykey);
}