#pragma once
#ifndef CORE_H
#define CORE_H
#include "engine/aengine.h"

// GBA platform capabilities header
// All values are 0 if the platform lacks the feature.

// Input
#define platformTotalControls   1       // Number of controllers that can be connected simultaneously
#define platformHasPointer      0       // Controllers have a pointing method
#define platformHasTouch        0       // Platform has a touchscreen
#define platformHasUSBmouse     0       // A mouse can be connected via USB
#define platformHasUSB          0       // Platform has USB ports
#define platformHasUSBkeyboard  0       // A keyboard can be connected via USB
#define platformHasGamepad      0       // Platform has a controller with a built-in screen
#define platformScreens         1       // Number of screens
#define platformAnalogTriggers  0       // Number of analog triggers (L and R are digital)

// CPU
#define platformCPU             1       // Number of CPUs available to the game (ARM7TDMI)
#define platformCPUclock        16      // CPU clock speed in MHz (16.78 MHz)
#define platformCPUbits         32      // CPU register width in bits
#define platformHasFPU          0       // CPU has a hardware floating point unit
#define platformRAM             288     // Total RAM in KB (256 KB EWRAM + 32 KB IWRAM)
#define platformCacheLineSize   0       // No data cache on ARM7TDMI
#define platformWordSize        4       // Natural word size in bytes (32-bit)
#define platformStackAlign      8       // Required stack alignment in bytes (AAPCS)
#define platformHeapAlign       4       // Recommended heap allocation alignment in bytes
#define platformHas64BitTypes   0       // Avoid — no native 64-bit arithmetic
#define platformHasDouble       0       // Avoid — software emulated, very slow

// DMA
#define platformDMAalign        4       // Required DMA buffer alignment (32-bit transfers)
#define platformDMAminSize      4       // Minimum transfer size where DMA is worth using

// GPU
#define platformGPU             0       // No 3D GPU
#define platformHas3D           0       // No hardware 3D
#define platformGPUclock        0
#define platformTexRAM          0
#define platformMaxTexWidth     0
#define platformMaxTexHeight    0
#define platformVRAM            96      // VRAM in KB (96 KB)
#define platformGPUcmdBufAlign  0
#define platformGPUcmdBufSize   0

// PPU
#define platformPPU             1       // 1 PPU
#define platformPPUmodes        6       // BG modes 0–5
#define platformMaxSprites      128     // Maximum OAM entries (128 objects)
#define platformHasScanlines    1       // HBlank DMA / HBlank IRQ available

// Display
#define platformMaxResW         240     // Horizontal resolution in pixels
#define platformMaxResH         160     // Vertical resolution in pixels
#define platformPALmaxResH      160     // No PAL mode — same as NTSC
#define platformHasProgressive  1       // Output is always progressive (LCD)
#define platformHasInterlaced   0       // No interlaced output
#define platformRefreshRate     60      // ~59.73 Hz
#define platformPALrefreshRate  60      // No PAL variant

// Audio
#define platformAPU             0       // No dedicated APU — mixing done on ARM7 via DMA
#define platformAPUclock        0
#define platformARAM            0       // No dedicated audio RAM — uses EWRAM
#define platformAudioChannels   2       // 2 hardware DirectSound channels (A and B); maxmod adds SW mixing on top
#define platformMaxSampleRate   32768   // Maximum practical sample rate in Hz
#define platformMinSampleRate   8000    // Minimum useful sample rate in Hz
#define platformHasStereo       1       // Left/right panning per DirectSound channel

// Network
#define platformHasWifi         0       // No built-in WiFi (Link Cable only)

// Colors
#define platformColorFormat     1555
#define platformColorOrder      colorOrderARGB

//other
#define platformACSmode         16

void platformInit();
void platformExit();
void finishFrame();

#endif