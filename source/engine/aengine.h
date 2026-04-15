/*
    Este es el código central del engine, el que enlaza todo
*/
#ifndef AENGINE_H
#define AENGINE_H

#include "engine/platform.h" // define CURRENT_PLATFORM
// ── Librerías comunes ─────────────────────────────────────────────────────────

#include <stdlib.h>
#include <stdio.h>

// ── Código del engine ─────────────────────────────────────────────────────────

#include "engine/fastmath.h"
#include "engine/scenemanager.h"
#include "engine/misc.h"
#include "engine/texture.h"

// ── Código específico por plataforma ──────────────────────────────────────────

// Común a GC y Wii
#if CURRENT_PLATFORM == PLATFORM_WII || CURRENT_PLATFORM == PLATFORM_GC
  //librerías
  #include <gccore.h>
  #include <asndlib.h>
  #include "engine/gc/video.h"
  #include "engine/gc/camera.h"
  #include "engine/gc/render.h"
  #include "engine/gc/hdma.h"
  #include "engine/gc/build25scene.h"
  #include "engine/gc/vertexbuffer.h"
  #include "engine/gc/oggplayer.h"
#endif

// Exclusivo GC
#if CURRENT_PLATFORM == PLATFORM_GC
    #include "engine/gc/core.h"
#endif

// Exclusivo Wii
#if CURRENT_PLATFORM == PLATFORM_WII
  //librerías
  #include <wiiuse/wpad.h>
  #include <ogc/usbmouse.h>
  #include <wiikeyboard/keyboard.h>
  #include "engine/wii/core.h"
#endif

// Exclusivo de SNES
#if CURRENT_PLATFORM == PLATFORM_SNES
  #include <snes.h>
  #include "engine/snes/core.h"
#endif

//Includes que se hacen después y son globales.
#include "engine/input.h"

#endif // AENGINE_H