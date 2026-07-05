#include "engine/gba/acs.h"
#include "engine/fastmath.h"

//versión optimizada para GBA

#define ACScolModeABGR1555   0
#define ACScolModeARGB8888   1
#define ACScolModeRGB888     2
#define ACScolModeGrayScale8 3
#define ACScolModeGrayScale4 4
#define ACStotalModes 5

#define ACSmirror 01
#define ACSpattern 00
#define ACSrepeat 10

ACSHeader ACSh;

//función auxiliar
inline void readCommand7(u8 byte, int* pInd, u16* rawPixels){
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
            u16 col = rawPixels[*pInd-1];
            u8 repeat = (byte & 0b111111)+1;
            for(int i = 0; i<repeat;i++){
                rawPixels[(*pInd)++] = col;
            }
        break;}
    }
}
inline void readCommand8(u8 byte, int* pInd, u16* rawPixels){
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
            u16 pixel = rawPixels[*pInd-1];
            u8 repeat = (byte & 0b1111111)+1;
            for(int i = 0; i<repeat;i++){
                rawPixels[(*pInd)++] = pixel;
            }
        break;}
    }
}
bool readACSheader(const u8* data){
    ACSh.valid = false;
    if(data[0] != 1) return false;//hardcodeado por ahora

    const int resTable[16] = {
        0,4,8,16,24,32,48,64,96,128,192,256,320,512,1024,-1
    };
    int ind = 1;
    u8 val = data[ind++];
    
    int resX = resTable[val >> 4];
    int resY = resTable[val & 0xF];
    
    if(resX == -1){ resX = (data[ind] << 8) | data[ind+1]; ind += 2; }
    if(resY == -1){ resY = (data[ind] << 8) | data[ind+1]; ind += 2; }
    
    ACSh.width = resX;
    ACSh.height = resY;
    ACSh.imgSize = resX*resY;
    int f = data[ind++];  // byte de bpp+colorMode
    ACSh.bpp = f>>6;
    ACSh.colorMode = (f >> 3) & 0b111;
    if(ACSh.colorMode > ACStotalModes) return false;

    ACSh.colorCount = data[ind++];// colorCount
    ACSh.headerSize = ind;
    ACSh.valid = true;
    return true;
}

int importACS(const u8* data, u16* rawPixels, u16* pal, Texture* tex){
    if(ACSh.valid == false){
        bool ok = readACSheader(data);
        if(ok == false){return false;}
    }
    ACSh.valid = false;
    int ind = ACSh.headerSize;
    int colorCount = ACSh.colorCount;
    int colorMode = ACSh.colorMode;
    int bpp = ACSh.bpp;
    int resX = ACSh.width;
    int resY = ACSh.height;

    tex->width  = resX; 
    tex->height = resY;

    u16 imgRes = (u16)resX *(u16)resY;

    //  MODO INDEXADO
    if(colorCount > 0){
        switch(colorMode){
            case ACScolModeABGR1555:
                for(int i = 0; i <= colorCount; i++){
                    pal[i] = ((u16)data[ind] << 8) | data[ind+1]; ind += 2;
                }
            break;

            case ACScolModeARGB8888:
                for(int i = 0; i <= colorCount; i++){
                    u8 a = data[ind++] >> 7;
                    u8 r = data[ind++] >> 3;
                    u8 g = data[ind++] >> 3;
                    u8 b = data[ind++] >> 3;
                    pal[i] = (u16)((a<<15)|(r<<10)|(g<<5)|b);
                }
            break;

            case ACScolModeGrayScale4:
                for(int i = 0; i <= colorCount; i++){
                    u8 v  = data[ind++];
                    u8 hi = (v & 0xF0) >> 3;
                    u8 lo = (v & 0x0F) << 1;
                    pal[i++] = 0x8000 | (hi<<10) | (hi<<5) | hi;
                    if(i >= colorCount) break;
                    pal[i]   = 0x8000 | (lo<<10) | (lo<<5) | lo;
                }
            break;

            case ACScolModeGrayScale8:
                for(int i = 0; i <= colorCount; i++){
                    u8 v = data[ind++] >> 3;
                    pal[i] = 0x8000 | (u16)((v<<10)|(v<<5)|v);
                }
            break;

            case ACScolModeRGB888:
                for(int i = 0; i <= colorCount; i++){
                    u8 r = data[ind++] >> 3;
                    u8 g = data[ind++] >> 3;
                    u8 b = data[ind++] >> 3;
                    pal[i] = 0x8000 | (u16)((r<<10)|(g<<5)|b);
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
                    rawPixels[pInd++] = ((u16)hi << 8) | lo;
                }
            break;

            case ACScolModeARGB8888:
                for(int i = 0; i < (int)imgRes; i++){
                    u8 a = data[ind++] >> 7;
                    u8 r = data[ind++] >> 3;
                    u8 g = data[ind++] >> 3;
                    u8 b = data[ind++] >> 3;
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
                    rawPixels[i] = (u16)((a<<15)|(r<<10)|(g<<5)|b);
                }
            break;

            case ACScolModeGrayScale4:
                for(int i = 0; i < 16; i++){
                    pal[i] = 0x8000 | (u16)((i<<11)|(1<<6)|(1<<1));
                }
            break;

            case ACScolModeGrayScale8:{
                for(int i = 0; i < 32; i++){
                    pal[i] = 0x8000 | (u16)((i<<10)|(i<<5)|i);
                }

                int ByteCtrlCount = ((int)data[ind]<<8) | data[ind+1]; ind+=2;
                int CommandsCount = ((int)data[ind]<<8) | data[ind+1]; ind+=2;

                const u8* ctrlBase = data + ind;
                const u8* cmdPtr   = data + ind + ByteCtrlCount;
                const u8* pixPtr   = data + ind + ByteCtrlCount + CommandsCount;
                int ctrlPos = 0;

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
                            rawPixels[pInd++] = *pixPtr++ >> 3;
                        }
                    }
                    ctrlPos += n;
                }
            break;}

            case ACScolModeRGB888:
                return -1;
        }
    }
    return 1;//no issues found in the file
}