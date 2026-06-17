// debug.c
#include "engine/gc/debug.h"
#include <gccore.h>
#include "engine/aengine.h"

extern void* xfb[2];
extern GXRModeObj* rmode;

void openTextConsole(){
    GX_AbortFrame();
    GX_Flush();

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
}

void _aAssert(bool cond, const char* msg, const char* file, int line) {
    #ifdef DEBUG_MODE
    if (!cond) {
        openTextConsole();
        printf("\x1b[2;0H");
        printf("|AENGINE ASSERT|\n\n");
        printf("  %s\n\n", msg);
        printf("  file: %s\n", file);
        printf("  line: %d\n", line);
        printf("\nPress any key to exit.");

        while(1) {
            inputUpdate();
            if((buttonsDown[0] | buttonsDown[1] | buttonsDown[2] | buttonsDown[3]) != 0){
                platformExit();
            } else {
                VIDEO_WaitVSync();
            }
        }
    }
    #endif
}

#include <time.h>
#include <ogc/lwp_watchdog.h>

u8 calculateFrameRate(void) {
    static u8 frameCount = 0;
    static u32 lastTime;
    static u8 FPS = 0;
    const u32 currentTime = ticks_to_millisecs(gettime());

    frameCount++;
    if(currentTime - lastTime > 1000) {
        lastTime = currentTime;
        FPS = frameCount;
        frameCount = 0;
    }
    return FPS;
}
