#pragma once
#include <ogc/gx.h>
extern void* gpFifo;
extern void* xfb[2];
extern int   currentFb;
extern GXRModeObj* rmode;

void prepareVideo();
void platformRender();
void exitVideo();