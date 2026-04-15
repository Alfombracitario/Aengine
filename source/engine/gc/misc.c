#include <time.h>
#include <ogc/lwp_watchdog.h>
#include "engine/misc.h"

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
