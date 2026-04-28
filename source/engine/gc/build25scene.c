#include "engine/gc/build25scene.h"
#include "engine/texture.h"
#include <gccore.h>

extern u16 sheetHeight;
extern Texture* gfxTiles;
#define zn -.5f
#define zf .5f
#define col 0xFFFFFFFF
#define uvS 0.0625f//cuanto mide cada tile en los UVs
#define B_UP   (1 << 0)
#define B_DOWN (1 << 1)
#define B_LEFT (1 << 2)
#define B_RIGHT (1 << 3)
#define B_UL   (1 << 4)
#define B_UR   (1 << 5)
#define B_DL   (1 << 6)
#define B_DR   (1 << 7)

void drawCubeFromUV(u16 tx, u16 ty, u16 uvFront, u16 uvLeft, u16 uvRight, u16 uvTop, u16 uvBottom, int ind,u8* neightbors){
    u8 mask = neightbors[ind];
    u16 tx2 = tx+1;
    u16 ty2 = ty+1;
    float uvX;
    float uvX2;
    float uvY = 0;
    float uvY2 = uvY + 1;
    u32 color[2] = {col,col};
    //cara inferior
    if((mask & B_DOWN) == 0){
        if(mask & B_DL){color[0] = 0x252525FF;}else{color[0]= 0x7F7F7FFF;}
        if(mask & B_DR){color[1] = 0x252525FF;}else{color[1]= 0x7F7F7FFF;}
        uvX  = (uvBottom*uvS);
        uvX2 = uvX + uvS;
        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
            GX_Position3f32(tx2, ty ,zn); GX_Color1u32(color[1]); GX_TexCoord2f32(uvX , uvY2);
            GX_Position3f32(tx2, ty ,zf); GX_Color1u32(color[1]); GX_TexCoord2f32(uvX , uvY );
            GX_Position3f32(tx,  ty ,zf); GX_Color1u32(color[0]); GX_TexCoord2f32(uvX2, uvY );
            GX_Position3f32(tx,  ty ,zn); GX_Color1u32(color[0]); GX_TexCoord2f32(uvX2, uvY2);
        GX_End();
    }

    //cara superior
    if((mask & B_UP) == 0){
        if(mask & B_UL){color[1] = 0x7F7F7FFF;}else{color[1]= col;}
        if(mask & B_UR){color[0] = 0x7F7F7FFF;}else{color[0]= col;}
        uvX  = (uvTop*uvS);
        uvX2 = uvX + uvS;
        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
            GX_Position3f32(tx, ty2 ,zn); GX_Color1u32(color[1]); GX_TexCoord2f32(uvX , uvY2);
            GX_Position3f32(tx, ty2 ,zf); GX_Color1u32(color[1]); GX_TexCoord2f32(uvX , uvY );
            GX_Position3f32(tx2,ty2 ,zf); GX_Color1u32(color[0]); GX_TexCoord2f32(uvX2, uvY );
            GX_Position3f32(tx2,ty2 ,zn); GX_Color1u32(color[0]); GX_TexCoord2f32(uvX2, uvY2);
        GX_End();
    }
    // cara izquierda
    if((mask & B_LEFT) == 0){
        if(mask & B_DL){color[0] = 0x7F7F7FFF;}else{color[0]= 0xCCCCCCFF;}
        if(mask & B_UL){color[1] = 0x7F7F7FFF;}else{color[1]= 0xCCCCCCFF;}
        uvX  = (uvLeft * uvS);
        uvX2 = uvX + uvS;
        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
            GX_Position3f32(tx,ty2,zn); GX_Color1u32(color[1]); GX_TexCoord2f32(uvX,   uvY2);
            GX_Position3f32(tx,ty, zn); GX_Color1u32(color[0]); GX_TexCoord2f32(uvX,   uvY );
            GX_Position3f32(tx,ty, zf); GX_Color1u32(color[0]); GX_TexCoord2f32(uvX2,  uvY );
            GX_Position3f32(tx,ty2,zf); GX_Color1u32(color[1]); GX_TexCoord2f32(uvX2,  uvY2);
        GX_End();
    }

    //cara derecha
    if((mask & B_RIGHT) == 0){
        if(mask & B_DR){color[1] = 0x505050FF;}else{color[1]= 0xAAAAAAFF;}
        if(mask & B_UR){color[0] = 0x505050FF;}else{color[0]= 0xAAAAAAFF;}
        uvX  = (uvRight * uvS);
        uvX2 = uvX + uvS;
        GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
            GX_Position3f32(tx2,ty , zn); GX_Color1u32(color[0]); GX_TexCoord2f32(uvX, uvY2);
            GX_Position3f32(tx2,ty2, zn); GX_Color1u32(color[1]); GX_TexCoord2f32(uvX, uvY );
            GX_Position3f32(tx2,ty2, zf); GX_Color1u32(color[1]); GX_TexCoord2f32(uvX2 ,  uvY );
            GX_Position3f32(tx2,ty , zf); GX_Color1u32(color[0]); GX_TexCoord2f32(uvX2 ,  uvY2);
    GX_End();
    }
    
    uvX  = (uvFront*uvS);
    uvX2 = uvX + uvS;
    GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
        GX_Position3f32(tx, ty ,zn); GX_Color1u32(col); GX_TexCoord2f32(uvX , uvY2);
        GX_Position3f32(tx, ty2,zn); GX_Color1u32(col); GX_TexCoord2f32(uvX , uvY );
        GX_Position3f32(tx2,ty2,zn); GX_Color1u32(col); GX_TexCoord2f32(uvX2, uvY );
        GX_Position3f32(tx2,ty ,zn); GX_Color1u32(col); GX_TexCoord2f32(uvX2, uvY2);
    GX_End();

    //back nunca es visible
}
void build25Dscene(const u8* roomData, size_t dataSize, u8* neightbors)
{
    const u8* ptr = roomData;
    const u8* end = roomData + dataSize;
    const u8 roomWidth  = *ptr++;
    const u8 roomHeight = *ptr++;

    // coordenadas
    u16 tx = 0;
    u16 ty = 0;
    while (ptr < end)
    {
        u8 id = *ptr++;

        TileInfo* t = &tileSet[id];

        if (t->draw) {
            t->draw(tx, ty);
        }
        else {
            int ind = ty * roomWidth + tx;
            drawCubeFromUV(
                tx, ty,
                t->uvFront,
                t->uvLeft,
                t->uvRight,
                t->uvTop,
                t->uvBottom,
                ind,
                neightbors
            );
        }
        tx++;
        if (tx >= roomWidth) {
            tx = 0;
            ty++;
        }
    }
}

// tiles       : puntero al mapa
// lut         : LUT[256] que dice si un tile es solido (0 o 1)
// width       : ancho del mapa (en tiles)
// offset      : desde donde empieza a leer
// neighbors[] : salida (un array de u8), del mismo tamaño
static unsigned char getSolid(const uint8_t* tiles, const uint8_t* lut,
                               int tx, int ty, int width, int height) {
    if (tx < 0 || ty < 0 || tx >= width || ty >= height)
        return 0;
    return lut[tiles[ty * width + tx]];
}

void computeNeighbors(const uint8_t *tiles, const uint8_t *lut,
                      int width, int offset, uint8_t *neighbors, int count) 
{
    int height = count / width;

    for (int i = 0; i < count; i++)
    {
        uint8_t mask = 0;
        int idx = offset + i;
        int x = idx % width;
        int y = idx / width;

        if (getSolid(tiles, lut, x, y+1, width, height)) mask |= B_UP;
        if (getSolid(tiles, lut, x, y-1, width, height)) mask |= B_DOWN;
        if (getSolid(tiles, lut, x-1, y, width, height)) mask |= B_LEFT;
        if (getSolid(tiles, lut, x+1, y, width, height)) mask |= B_RIGHT;

        if (getSolid(tiles, lut, x-1, y+1, width, height)) mask |= B_UL;
        if (getSolid(tiles, lut, x+1, y+1, width, height)) mask |= B_UR;
        if (getSolid(tiles, lut, x-1, y-1, width, height)) mask |= B_DL;
        if (getSolid(tiles, lut, x+1, y-1, width, height)) mask |= B_DR;

        neighbors[i] = mask;
    }
}