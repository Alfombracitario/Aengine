//emulación de HDMA usando la GX (más motores retro deberían tener esto lol)
#include "engine/gc/hdma.h"
#include "engine/gc/render.h"

float layerSLxoffset[HDMA_MAX_LINES];
float layerSLyoffset[HDMA_MAX_LINES];
u32   layerSLcolor [HDMA_MAX_LINES];
float layerSLxscale[HDMA_MAX_LINES];

extern u32 surfaceHeight;
extern u32 surfaceWidth;

//para facilitarme la vida
#define uo layerSLxoffset[i]
#define vo layerSLyoffset[i]
#define c layerSLcolor[i]

void init_hdma(){
    for(int i = 0; i < HDMA_MAX_LINES; i++){
        uo = 0;
        vo = i;
        c = 0xFFFFFFFF;
    }
}
void draw_texture_hdma(const Texture* tex) {
    bool filter = gpu_get_tex_filter();
    bool repeat = gpu_get_tex_repeat();
    gpu_set_tex_filter(false);
    gpu_set_tex_repeat(true);

    gpu_set_texture(tex);

    const f32 ur = surfaceWidth / tex->width;
    const f32 step = 1.0f / tex->height; // paso basado en la textura, no en las líneas a dibujar
    const u32 lines = surfaceHeight; // iteramos toda la pantalla

    GX_Begin(GX_QUADS, GX_VTXFMT0, 4 * lines);
    for (int i = 0; i < lines; i++) {
        float v0 = step * i + vo;       // el wrap lo maneja el sampler
        float v1 = step * (i + 1) + vo;

        GX_Position3f32(surfaceWidth, i + 1, 0.0f); GX_Color1u32(c); GX_TexCoord2f32(uo + ur, v0);
        GX_Position3f32(0,            i + 1, 0.0f); GX_Color1u32(c); GX_TexCoord2f32(uo,      v0);
        GX_Position3f32(0,            i,     0.0f); GX_Color1u32(c); GX_TexCoord2f32(uo,      v1);
        GX_Position3f32(surfaceWidth, i,     0.0f); GX_Color1u32(c); GX_TexCoord2f32(uo + ur, v1);
    }
    GX_End();

    gpu_set_tex_filter(filter);
    gpu_set_tex_repeat(repeat);
}
