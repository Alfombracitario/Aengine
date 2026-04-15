#ifndef HDMA_H
#define HDMA_H

#include "engine/texture.h"

#define HDMA_MAX_LINES 480

extern float layerSLxoffset[HDMA_MAX_LINES];
extern float layerSLyoffset[HDMA_MAX_LINES];
extern u32   layerSLcolor [HDMA_MAX_LINES];
extern float layerSLxscale[HDMA_MAX_LINES];

void draw_texture_hdma(const Texture* tex);
void init_hdma();
#endif