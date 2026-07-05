#include <nds.h>

// 1. Asignar banco(s) de VRAM al rol de sprites
vramSetBankA(VRAM_A_MAIN_SPRITE);

// 2. Inicializar el OAM (Object Attribute Memory)
oamInit(&oamMain, SpriteMapping_1D_128, false); // false = sin paleta extendida

// 3. Pedir espacio dentro de ese banco para un sprite concreto
u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);

// 4. Copiar los datos (ya en formato tile) al puntero que te dio el allocator
dmaCopy(spriteTiles, gfx, spriteTilesLen);
dmaCopy(spritePal, SPRITE_PALETTE, spritePalLen); // paleta va a una zona fija aparte

// 5. Asociar ese gfx a un slot de OAM (0-127) y setear posición/tamaño
oamSet(&oamMain, 0, x, y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, gfx, -1, false, false, false, false, false);

// cada frame:
oamUpdate(&oamMain);

