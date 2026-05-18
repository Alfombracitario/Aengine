// debug.c
#include <gccore.h>
#include <stdio.h>
#include "engine/aengine.h"
// asume que estos están declarados como extern en algún header del engine
extern void* xfb[2];
extern GXRModeObj* rmode;

void aAssert(char* text) {
    // detener GX
    GX_AbortFrame();
    GX_Flush();

    // reinicializar la consola sobre el framebuffer
    console_init(
        xfb[0],
        0, 0,
        rmode->fbWidth,
        rmode->xfbHeight,
        rmode->fbWidth * VI_DISPLAY_PIX_SZ
    );

    VIDEO_SetNextFramebuffer(xfb[0]);
    VIDEO_SetBlack(false);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if(rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    printf("\x1b[2;0H");
    printf("|AENGINE ASSERT|\n\n");
    printf("  %s\n\n", text);
    printf("\nPress any key to exit.");
    while(1) {
        inputUpdate();
        if((buttonsDown[0] | buttonsDown[1] | buttonsDown[2] | buttonsDown[3]) != 0){
            platformExit();
        }
        else{
            VIDEO_WaitVSync();
        }
    }
}