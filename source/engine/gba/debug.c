#include "engine/aengine.h"
#include "engine/gba/debug.h"
#include <stdio.h>
#include <stdlib.h>

void openTextConsole(){
    consoleDemoInit();
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

        while (1) {
            VBlankIntrWait();
        }
    }
    #endif
}

