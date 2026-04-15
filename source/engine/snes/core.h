#ifndef CORE_H
#define CORE_H

// SNES platform capabilities header
// All values are 0 if the platform lacks the feature.

// Input
#define platformTotalControls   2       // Number of controllers that can be connected simultaneously
#define platformHasPointer      1       // Controllers have a pointing method (SNES mouse)
#define platformHasTouch        0       // Platform has a touchscreen
#define platformHasUSBmouse     0       // A mouse can be connected via USB
#define platformHasUSB          0       // Platform has USB ports
#define platformHasUSBkeyboard  0       // A keyboard can be connected via USB
#define platformHasGamepad      0       // Platform has a controller with a built-in screen (e.g. Wii U GamePad)
#define platformScreens         1       // Number of screens
#define platformAnalogTriggers  0       // Number of analog triggers (not just digital buttons)

// CPU
#define platformCPU             1       // Number of CPUs available to the game (Ricoh 5A22)
#define platformCPUclock        3       // CPU clock speed in MHz
#define platformCPUbits         16      // CPU register width in bits
#define platformHasFPU          0       // CPU has a hardware floating point unit
#define platformRAM             128     // Total RAM in KB (MEM1 24MB + MEM2 64MB)
#define platformCacheLineSize   0       // CPU cache line size in bytes — use for DMA buffer alignment
#define platformWordSize        2       // Natural word size in bytes (16-bit)
#define platformStackAlign      8       // Required stack alignment in bytes
#define platformHeapAlign       16      // Recommended heap allocation alignment in bytes

// DMA
#define platformDMAalign        2       // Required DMA buffer alignment in bytes (equals cache line size)
#define platformDMAminSize      2       // Minimum transfer size where DMA is worth using over memcpy

// GPU
#define platformGPU             0       // Number of GPUs (Hollywood)
#define platformHas3D           0       // GPU supports 3D rendering
#define platformGPUclock        0       // GPU clock speed in MHz
#define platformTexRAM          0       // Dedicated texture memory in KB (TMEM)
#define platformMaxTexWidth     0       // Maximum texture width in pixels
#define platformMaxTexHeight    0       // Maximum texture height in pixels
#define platformVRAM            64      // Dedicated VRAM in KB (0 = shares main RAM)
#define platformGPUcmdBufAlign  0       // Required GPU command buffer alignment in bytes
#define platformGPUcmdBufSize   0       // Recommended GPU command buffer size in bytes

// PPU
#define platformPPU             2       // Number of PPUs (0 = no tile/sprite hardware, use GPU instead)
#define platformPPUmodes        8       // Number of PPU background modes
#define platformMaxSprites      128     // Maximum hardware sprites on screen
#define platformHasScanlines    1       // PPU exposes per-scanline effects (HDMA etc.)

// Display
#define platformMaxResW         512     // Maximum horizontal resolution in pixels (NTSC/PAL)
#define platformMaxResH         480     // Maximum vertical resolution in pixels (NTSC)
#define platformPALmaxResH      576     // Maximum vertical resolution in pixels (PAL)
#define platformHasProgressive  1       // Output supports progressive scan (480p via component)
#define platformHasInterlaced   1       // Output supports interlaced scan (480i default)
#define platformRefreshRate     60      // Default refresh rate in Hz (NTSC)
#define platformPALrefreshRate  50      // Refresh rate in Hz when running in PAL mode

// Audio
#define platformAPU             1       // Platform has a dedicated audio processing unit (SPC700)
#define platformAPUclock        1       // APU clock speed in MHz
#define platformARAM            64      // Dedicated audio RAM in KB (0 = uses main RAM)
#define platformAudioChannels   8       // Maximum simultaneous mono voices in hardware
#define platformMaxSampleRate   32000   // Maximum supported sample rate in Hz
#define platformMinSampleRate   8000    // Minimum useful sample rate in Hz
#define platformHasStereo       1       // APU supports stereo output

// Network
#define platformHasWifi         0       // Platform has built-in WiFi

void platformInit();
void platformExit();
void finishFrame();

#endif