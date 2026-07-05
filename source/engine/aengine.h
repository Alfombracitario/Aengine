/*
    Este es el código central del engine, el que enlaza todo
*/
#ifndef AENGINE_H
#define AENGINE_H

#include "engine/platform.h" // define CURRENT_PLATFORM
// ── Librerías comunes ─────────────────────────────────────────────────────────

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ── Código del engine ─────────────────────────────────────────────────────────

#include "engine/fastmath.h"
#include "engine/scenemanager.h"
#include "engine/texture.h"
#include "engine/mem.h"

// ── Código específico por plataforma ──────────────────────────────────────────
#include "engine/general/color.h"

// Común a GC y Wii
#if CURRENT_PLATFORM == PLATFORM_WII || CURRENT_PLATFORM == PLATFORM_GC
  //librerías
  #include <gccore.h>
  #include <asndlib.h>
  #include "engine/gc/video.h"
  #include "engine/gc/camera.h"
  #include "engine/gc/render.h"
  #include "engine/gc/hdma.h"
  //#include "engine/gc/build25scene.h"
  #include "engine/gc/oggplayer.h"
  #include "engine/gc/debug.h"
#endif

// Exclusivo GC
#if CURRENT_PLATFORM == PLATFORM_GC
    #include "engine/gc/core.h"
#endif

// Exclusivo Wii
#if CURRENT_PLATFORM == PLATFORM_WII
  //librerías
  #include <wupc/wupc.h>
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

//Exclusivo de GBA
#if CURRENT_PLATFORM == PLATFORM_GBA
  #include <gba.h>
  #include <gba_console.h>
  #include <gba_video.h>
  #include <gba_interrupt.h>
  #include <gba_systemcalls.h>
  #include <gba_input.h>
  #include "engine/gba/core.h"
  #include "engine/gba/debug.h"
#endif

//Includes que se hacen después y son globales.
#include "engine/input.h"

#endif // AENGINE_H