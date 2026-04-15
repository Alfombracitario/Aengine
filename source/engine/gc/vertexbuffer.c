//como placeholder, no usamos display list aún, usaremos un método de placeholder fácil de programar
#include "engine/gc/vertexbuffer.h"
static u8 format;//flags

#define formatPosition      0b1
#define formatColour        0b10
#define formatTexCoord      0b100
#define formatNormal        0b1000
#define formatPosition3D    0b10000

void vertex_format_add_colour(){
    format |= formatColour;
}

void vertex_format_add_position(){
    format |= formatPosition;
}

void vertex_format_add_position3D(){
    format |= formatPosition3D;
}

void vertex_format_add_texcoord(){
    format |= formatTexCoord;
}

void vertex_format_add_normal(){
    format |= formatNormal;
}

u8 vertex_format_end(){
    return format;
}

#define VB_CHUNK_VERTS 1024

typedef struct VBChunk {
    struct VBChunk* next;

    u16 count;  // vértices usados en este chunk

    f32 pos[VB_CHUNK_VERTS][3];
    s16 nrm[VB_CHUNK_VERTS][3];
    f32 tex[VB_CHUNK_VERTS][2];
    u32 col[VB_CHUNK_VERTS];
} VBChunk;


typedef struct vertexbuffer {
    /* data */
} vertexbuffer;