// Valores numéricos para comparación en runtime
#define PLATFORM_UNKNOWN    0

// Nintendo
#define PLATFORM_SNES       17
#define PLATFORM_GBA        18
#define PLATFORM_DS         19
#define PLATFORM_GC         1   // IBM Gekko + Flipper GPU
#define PLATFORM_WII        2   // IBM Broadway + Hollywood GPU
#define PLATFORM_WIIU       3   // IBM Espresso + Latte GPU
#define PLATFORM_SWITCH     4   // ARM Cortex-A57 + Tegra X1
#define PLATFORM_3DS        5   // ARM11 VFPv2 + PICA200

// Sony
#define PLATFORM_PS2        6   // MIPS R5900 + VU0/VU1 + GS
#define PLATFORM_PS3        7   // Cell Broadband + RSX
#define PLATFORM_PS4        8   // x86-64 + AMD GCN
#define PLATFORM_PSP        9   // MIPS Allegrex + GPU propio

// Microsoft
#define PLATFORM_XBOX       10  // Intel Pentium III + NV2A
#define PLATFORM_X360       11  // PowerPC Xenon + Xenos
#define PLATFORM_XONE       12  // x86-64 + AMD GCN

// PC / Mobile
#define PLATFORM_PC         13
#define PLATFORM_ANDROID    14  // ARM VFP/NEON
#define PLATFORM_IOS        15  // ARM Apple Silicon

// Otros
#define PLATFORM_DREAMCAST  16  // SH-4 + PowerVR CLX2

// Validar que CURRENT_PLATFORM fue definido externamente (desde el Makefile)
#if !defined(CURRENT_PLATFORM)
  #error "CURRENT_PLATFORM not defined; try make PLATFORM=platform"
#endif

// Validar que el valor es conocido
#if CURRENT_PLATFORM == PLATFORM_UNKNOWN
  #error "CURRENT_PLATFORM is PLATFORM_UNKNOWN. no target selected."
#endif