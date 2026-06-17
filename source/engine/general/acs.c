#include "engine/general/acs.h"
#include "engine/fastmath.h"

//this is the generic ACS reader, this won't use hardware shortcuts.
//if the hardware needs it, ACS will have a separated implementation in that platform.

#define ACScolModeABGR1555   0
#define ACScolModeARGB8888   1
#define ACScolModeRGB888     2
#define ACScolModeGrayScale8 3
#define ACScolModeGrayScale4 4
#define ACStotalModes 5

#define ACSmirror 01
#define ACSpattern 00
#define ACSrepeat 1

//función auxiliar
inline void readCommand7(u8 byte, int* pInd, u32* rawPixels){
    //determinar cual es el tipo de comando contra el que estamos tratando
    switch((byte>>5) & 0b011){
        case ACSpattern:{
            //000P PPRR
            u8 repeat = (byte & 0b11)+1;
            u8 pixels = ((byte>>2) & 0b111)+2;

            int rInd = *pInd-pixels;
            u8 iterations = repeat*pixels;
            for(int i = 0; i < iterations; i++){
                rawPixels[(*pInd)++] = rawPixels[rInd++];
            }
        break;}
        case ACSmirror:{
            //001x xxxx
            //debemos leer para atrás y escribirlo hacia adelante
            int mInd = *pInd-1;
            u8 repeat = (byte & 0b11111)+2;
            for(int i = 0; i < repeat; i++){
                rawPixels[(*pInd)++] = rawPixels[mInd--];
            }
        break;}

        default:{//repetición simple (01xxxxxx)
            //obtener último color
            u32 col = rawPixels[*pInd-1];
            u8 repeat = (byte & 0b111111)+1;
            for(int i = 0; i<repeat;i++){
                rawPixels[(*pInd)++] = col;
            }
        break;}
    }
}
inline void readCommand8(u8 byte, int* pInd, u32* rawPixels){
    switch(byte>>6){
        case ACSpattern:{//repeat pattern
            //CCP PPRRR
            u8 repeat = (byte & 0b111)+1;
            u8 pixels = ((byte>>3) & 0b111)+2;

            int rInd = *pInd-pixels;
            u8 iterations = repeat*pixels;
            for(int i = 0; i < iterations; i++){
                rawPixels[(*pInd)++] = rawPixels[rInd++];
            }
        break;}

        case ACSmirror:{//Mirror
            //debemos leer para atrás y escribirlo hacia adelante
            int mInd = *pInd-1;
            u8 repeat = (byte & 0b111111)+2;
            for(int i = 0; i < repeat; i++){
                rawPixels[(*pInd)++] = rawPixels[mInd--];
            }
        break;}

        default:{//repetición simple, caso 10 u 11
            //obtener último indice
            u32 pixel = rawPixels[*pInd-1];
            u8 repeat = (byte & 0b1111111)+1;
            for(int i = 0; i<repeat;i++){
                rawPixels[(*pInd)++] = pixel;
            }
        break;}
    }
}
int readACSheader(const u8* data, int* c, int* w, int* h, int* f){
    const int resTable[16] = {
        0,4,8,16,24,32,48,64,96,128,192,256,320,512,1024,-1
    };
    int ind = 1;
    u8 val = data[ind++];
    
    int resX = resTable[val >> 4];
    int resY = resTable[val & 0xF];
    
    if(resX == -1){ resX = (data[ind] << 8) | data[ind+1]; ind += 2; }
    if(resY == -1){ resY = (data[ind] << 8) | data[ind+1]; ind += 2; }
    
    *w = resX;
    *h = resY;
    *f = data[ind++];  // byte de bpp+colorMode
    *c = data[ind++];             // colorCount
    return ind;
}
int importACS(const u8* data, u32* rawPixels, u32* pal, Texture* tex){
    // ---------- byte 0: version y otros datos ----------
    if(data[0] != 1) return -1;//hardcodeado por ahora

    int ind = 1;//ya leimos el indice 0
    int colorCount, format;
    int resX, resY;
    
    ind = readACSheader(data,&colorCount,&resX,&resY,&format);
    u8 bpp       = format >> 6;
    u8 colorMode = (format >> 3) & 0b111;
    if(colorMode > ACStotalModes) return -1;

    tex->width  = resX; 
    tex->height = resY;

    u32 imgRes = (u32)resX *(u32)resY;
    //  MODO INDEXADO
    if(colorCount > 0){
        switch(colorMode){
            case ACScolModeABGR1555:
                for(int i = 0; i <= colorCount; i++){
                    const u16 color = ((u16)data[ind] << 8) | data[ind+1]; ind += 2;
                    u32 a = (color >> 15) & 1;
                    u32 r = (color >> 10) & 0x1F;
                    u32 g = (color >>  5) & 0x1F;
                    u32 b = (color      ) & 0x1F;
                    pal[i] = ((b<<3|b>>2) << 24)
                        | ((g<<3|g>>2) << 16)
                        | ((r<<3|r>>2) <<  8)
                        | (a ? 0xFF : 0x00);
                }
            break;

            case ACScolModeARGB8888:
                for(int i = 0; i <= colorCount; i++){
                    u32 a = data[ind++];
                    u32 r = data[ind++];
                    u32 g = data[ind++];
                    u32 b = data[ind++];
                    pal[i] = (r << 24) | (g << 16) | (b << 8) | a;
                }
            break;

            case ACScolModeGrayScale4:
                for(int i = 0; i <= colorCount; i++){
                    u32 v  = data[ind++];
                    u32 hi = ((v >> 4) & 0xF) * 17;
                    u32 lo =  (v & 0xF)       * 17;
                    pal[i++] = (hi << 24) | (hi << 16) | (hi << 8) | 0xFF;
                    if(i >= colorCount) break;
                    pal[i]   = (lo << 24) | (lo << 16) | (lo << 8) | 0xFF;
                }
            break;

            case ACScolModeGrayScale8:
                for(int i = 0; i <= colorCount; i++){
                    u32 v = data[ind++];
                    pal[i] = (v << 24) | (v << 16) | (v << 8) | 0xFF;
                }
            break;

            case ACScolModeRGB888:
                for(int i = 0; i <= colorCount; i++){
                    u32 r = data[ind++];
                    u32 g = data[ind++];
                    u32 b = data[ind++];
                    pal[i] = (r << 24) | (g << 16) | (b << 8) | 0xFF;
                }
            break;
        }

        // modo oculto solo-paleta
        if(resX == 0 || resY == 0) return -1;//no soportado por ahora, deberías llamarlo con otra función.

        // --- cabecera de control ---
        int ByteCtrlCount  = ((int)data[ind]<<8) | data[ind+1]; ind+=2;
        int CommandsCount  = ((int)data[ind]<<8) | data[ind+1]; ind+=2;

        const u8* ctrlBase = data + ind;           // puntero fijo al bloque de control
        const u8* cmdBase  = data + ind + ByteCtrlCount; // puntero fijo al bloque de comandos
        ind += ByteCtrlCount + CommandsCount;       // ind apunta ahora a los datos de píxel

        // punteros de lectura independientes — evita recalcular offsets cada vez
        const u8* pixPtr  = data + ind;
        const u8* cmdPtr  = cmdBase;
        int ctrlPos = 0;
        int pInd    = 0;
        u8  part    = 0;

        //  HOT LOOP — bit de control desempaquetado de a 8 bits
        //  para reducir las divisiones/módulos por iteración.
        switch(bpp){
            // -------- 8 BPP --------
            case 3:
                while(pInd < (int)imgRes){
                    // desempaquetar el byte de control actual completo
                    u8 ctrl = ctrlBase[ctrlPos >> 3];
                    int startBit = ctrlPos & 7;
                    int bitsLeft = 8 - startBit;
                    int pixLeft  = (int)imgRes - pInd;
                    int n = bitsLeft < pixLeft ? bitsLeft : pixLeft;

                    u8 mask = 0x80 >> startBit;
                    for(int b = 0; b < n; b++, mask >>= 1){
                        if(ctrl & mask){
                            readCommand8(*cmdPtr++, &pInd, rawPixels);
                        } else {
                            rawPixels[pInd++] = *pixPtr++;
                        }
                    }
                    ctrlPos += n;
                }
            break;

            // -------- 4 BPP --------
            case 2:
                while(pInd < (int)imgRes){
                    u8 ctrl = ctrlBase[ctrlPos >> 3];
                    int startBit = ctrlPos & 7;
                    int bitsLeft = 8 - startBit;
                    int pixLeft  = (int)imgRes - pInd;
                    int n = bitsLeft < pixLeft ? bitsLeft : pixLeft;

                    u8 mask = 0x80 >> startBit;
                    for(int b = 0; b < n; b++, mask >>= 1){
                        if(ctrl & mask){
                            readCommand8(*cmdPtr++, &pInd, rawPixels);
                        } else {
                            u8 raw = *pixPtr;
                            u8 index;
                            if(part == 0){ index = raw >> 4;   part = 1; }
                            else         { index = raw & 0x0F; pixPtr++; part = 0; }
                            rawPixels[pInd++] = index;
                        }
                    }
                    ctrlPos += n;
                }
            break;

            // -------- 2 BPP --------
            case 1:
                while(pInd < (int)imgRes){
                    u8 ctrl = ctrlBase[ctrlPos >> 3];
                    int startBit = ctrlPos & 7;
                    int bitsLeft = 8 - startBit;
                    int pixLeft  = (int)imgRes - pInd;
                    int n = bitsLeft < pixLeft ? bitsLeft : pixLeft;

                    u8 mask = 0x80 >> startBit;
                    for(int b = 0; b < n; b++, mask >>= 1){
                        if(ctrl & mask){
                            readCommand8(*cmdPtr++, &pInd, rawPixels);
                        } else {
                            int shift = 6 - (part << 1);
                            u8 index = (*pixPtr >> shift) & 0b11;
                            rawPixels[pInd++] = index;
                            part++;
                            if(shift == 0){ pixPtr++; part = 0; }
                        }
                    }
                    ctrlPos += n;
                }
            break;

            // -------- 1 BPP --------
            case 0:
                while(pInd < (int)imgRes){
                    u8 ctrl = ctrlBase[ctrlPos >> 3];
                    int startBit = ctrlPos & 7;
                    int bitsLeft = 8 - startBit;
                    int pixLeft  = (int)imgRes - pInd;
                    int n = bitsLeft < pixLeft ? bitsLeft : pixLeft;

                    u8 mask = 0x80 >> startBit;
                    for(int b = 0; b < n; b++, mask >>= 1){
                        if(ctrl & mask){
                            readCommand8(*cmdPtr++, &pInd, rawPixels);
                        } else {
                            int shift = 7 - part;
                            u8 index = (*pixPtr >> shift) & 1;
                            rawPixels[pInd++] = index;
                            part++;
                            if(shift == 0){ pixPtr++; part = 0; }
                        }
                    }
                    ctrlPos += n;
                }
            break;
        }
    //  MODO DIRECTO
    } else {
        int pInd = 0;
        switch(colorMode){
            case ACScolModeABGR1555:
                while(pInd < (int)imgRes){
                    u8 hi = data[ind++];
                    u8 lo = data[ind++];

                    if(hi < 0x80){
                        if((hi | lo) != 0){
                            readCommand7(hi, &pInd, rawPixels);
                            ind--;
                            continue;
                        } else {
                            rawPixels[pInd++] = 0;
                            continue;
                        }
                    }
                    const u16 color = ((u16)hi << 8) | lo;
                    u32 a = (color >> 15) & 1;
                    u32 r = (color >> 10) & 0x1F;
                    u32 g = (color >>  5) & 0x1F;
                    u32 b = (color      ) & 0x1F;
                    rawPixels[pInd++] = ((b<<3|b>>2) << 24)
                        | ((g<<3|g>>2) << 16)
                        | ((r<<3|r>>2) <<  8)
                        | (a ? 0xFF : 0x00);
                }
            break;

            case ACScolModeARGB8888:
                for(int i = 0; i < (int)imgRes; i++){
                    u8 a = data[ind++];
                    u8 r = data[ind++];
                    u8 g = data[ind++];
                    u8 b = data[ind++];
                    if(a == 0){
                        if((r | g | b) != 0){
                            readCommand7(r, &i, rawPixels);
                            ind -= 2;
                            continue;
                        } else {
                            rawPixels[i] = 0;
                            continue;
                        }
                    }
                    rawPixels[i] = (r << 24) | (g << 16) | (b << 8) | a;
                }
            break;

            case ACScolModeGrayScale4:
            //no commands, just raw data? yeah this sounds like a bug, I'll check later my own documentation lol
                for(int i = 0; i < (int)imgRes; i++){
                    u32 v  = data[ind++];
                    u32 hi = ((v >> 4) & 0xF) * 17;
                    u32 lo = (v & 0xF)        * 17;
                    rawPixels[i++] = (hi << 24) | (hi << 16) | (hi << 8) | 0xFF;
                    rawPixels[i]   = (lo << 24) | (lo << 16) | (lo << 8) | 0xFF;
                }
            break;

            case ACScolModeGrayScale8:
                return -1;
            break;

            case ACScolModeRGB888:
                return -1;
            break;
        }
    }
    return 1;//no issues found in the file
}