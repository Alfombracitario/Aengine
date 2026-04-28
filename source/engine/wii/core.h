#ifndef CORE_H
#define CORE_H

// Wii platform capabilities header
// All values are 0 if the platform lacks the feature.

// Input
#define platformTotalControls   4       // Number of controllers that can be connected simultaneously
#define platformHasPointer      1       // Controllers have a pointing method (Wiimote IR)
#define platformHasTouch        0       // Platform has a touchscreen
#define platformHasUSBmouse     1       // A mouse can be connected via USB
#define platformHasUSB          1       // Platform has USB ports
#define platformHasUSBkeyboard  1       // A keyboard can be connected via USB
#define platformHasGamepad      0       // Platform has a controller with a built-in screen (e.g. Wii U GamePad)
#define platformScreens         1       // Number of screens
#define platformAnalogTriggers  0       // Number of analog triggers (not just digital buttons)

// CPU
#define platformCPU             1       // Number of CPUs available to the game (Broadway, PowerPC 750)
#define platformCPUclock        729     // CPU clock speed in MHz
#define platformCPUbits         32      // CPU register width in bits
#define platformHasFPU          1       // CPU has a hardware floating point unit
#define platformRAM             (64+24)*1024 // Total RAM in KB (MEM1 24MB + MEM2 64MB)
#define platformCacheLineSize   32      // CPU cache line size in bytes — use for DMA buffer alignment
#define platformWordSize        4       // Natural word size in bytes (32-bit)
#define platformStackAlign      8       // Required stack alignment in bytes (PPC ABI)
#define platformHeapAlign       32      // Recommended heap allocation alignment in bytes
#define platformHas64BitTypes   1       // Platform supports uint64_t / int64_t
#define platformHasDouble       1       // Platform supports double (f64)

// DMA
#define platformDMAalign        32      // Required DMA buffer alignment in bytes (equals cache line size)
#define platformDMAminSize      32      // Minimum transfer size where DMA is worth using over memcpy

// GPU
#define platformGPU             1       // Number of GPUs (Hollywood)
#define platformHas3D           1       // GPU supports 3D rendering
#define platformGPUclock        243     // GPU clock speed in MHz
#define platformTexRAM          (3*1024)    // Dedicated texture memory in KB (TMEM)
#define platformMaxTexWidth     1024    // Maximum texture width in pixels
#define platformMaxTexHeight    1024    // Maximum texture height in pixels
#define platformVRAM            0       // Dedicated VRAM in KB (0 = shares main RAM)
#define platformGPUcmdBufAlign  32      // Required GPU command buffer alignment in bytes
#define platformGPUcmdBufSize   (256*1024) // Recommended GPU command buffer size in bytes

// PPU
#define platformPPU             0       // Number of PPUs (0 = no tile/sprite hardware, use GPU instead)
#define platformPPUmodes        0       // Number of PPU background modes
#define platformMaxSprites      0       // Maximum hardware sprites on screen
#define platformHasScanlines    0       // PPU exposes per-scanline effects (HDMA etc.)

// Display
#define platformMaxResW         640     // Maximum horizontal resolution in pixels (NTSC/PAL)
#define platformMaxResH         480     // Maximum vertical resolution in pixels (NTSC)
#define platformPALmaxResH      576     // Maximum vertical resolution in pixels (PAL)
#define platformHasProgressive  1       // Output supports progressive scan (480p via component)
#define platformHasInterlaced   1       // Output supports interlaced scan (480i default)
#define platformRefreshRate     60      // Default refresh rate in Hz (NTSC)
#define platformPALrefreshRate  50      // Refresh rate in Hz when running in PAL mode

// Audio
#define platformAPU             1       // Platform has a dedicated audio processing unit (DSP)
#define platformAPUclock        81      // APU clock speed in MHz
#define platformARAM            0       // Dedicated audio RAM in KB (0 = uses main RAM; GC has 16MB)
#define platformAudioChannels   64      // Maximum simultaneous mono voices in hardware
#define platformMaxSampleRate   48000   // Maximum supported sample rate in Hz
#define platformMinSampleRate   8000    // Minimum useful sample rate in Hz
#define platformHasStereo       1       // APU supports stereo output

// Network
#define platformHasWifi         1       // Platform has built-in WiFi

void platformInit();
void platformExit();
void finishFrame();

#endif