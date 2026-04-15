#ifndef RENDER_H
#define RENDER_H
#include <gccore.h>
#include "engine/texture.h"

//variables
extern u32 surfaceWidth;
extern u32 surfaceHeight;

//funciones auxiliares
void invalidateTexCache();

//texturas
void gpu_set_tex_filter(bool linear);
void gpu_set_copy_filter(bool enable);
void gpu_set_tex_repeat(bool repeat);
void gpu_set_texture(const Texture *tex);

bool gpu_get_tex_filter();
bool gpu_get_copy_filter();
bool gpu_get_tex_repeat();
//ztest
void gpu_set_ztestenable(bool enable);
void gpu_set_zfunc(u32 cmp_function);
void gpu_set_zwriteenable(bool enable);
//alpha
void gpu_set_alphatestenable(bool enable);
void gpu_set_alphatestref(u8 ref);
//cullmode
void gpu_set_cullmode(u32 cullmode);
//draw sprite (identico a GM)
void draw_sprite(const Texture *tex, const int frame, f32 x, f32 y);
void draw_sprite_ext(const Texture *tex, const int frame, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color);
//draw texture (custom)
void draw_texture(const Texture *tex, f32 xpos, f32 ypos);
void draw_texture_ext(const Texture *tex, const f32 x, const f32 y, const f32 scaleX, const f32 scaleY, const f32 degrees, const u32 color);
void draw_texture_stretched(const Texture* tex, const f32 x, const f32 y, const f32 w, const f32 h);
void draw_texture_tiled(const Texture* tex, const f32 x, const f32 y);
//draw shape
void draw_rectangle_colour(f32 x, f32 y, f32 w, f32 h, u32 colorTopLeft, u32 colorTopRight, u32 colorBottomRight, u32 colorBottomLeft);
//surfaces
Texture* surface_create(u16 width, u16 height);
void surface_copy(Texture *tex,u16 posx, u16 posy, bool clear);

//Macros
#define c_black   0x000000FF
#define c_marron  0x800000FF
#define c_green   0x008000FF
#define c_olive   0x808000FF
#define c_navy    0x000080FF
#define c_purple  0x800080FF
#define c_teal    0x008080FF
#define c_gray    0x808080FF
#define c_silver  0xC0C0C0FF
#define c_red     0xFF0000FF
#define c_lime    0x00FF00FF
#define c_yellow  0xFFFF00FF
#define c_orange  0xFF7F00FF
#define c_blue    0x0000FFFF
#define c_fuchsia 0xFF00FFFF
#define c_aqua    0x00FFFFFF
#define c_white   0xFFFFFFFF

//
#define cmpfunc_never        GX_NEVER
#define cmpfunc_less         GX_LESS
#define cmpfunc_equal        GX_EQUAL
#define cmpfunc_lessequal    GX_LEQUAL
#define cmpfunc_greater      GX_GREATER
#define cmpfunc_notequal     GX_NEQUAL
#define cmpfunc_greaterequal GX_GEQUAL
#define cmpfunc_always       GX_ALWAYS

#define cull_noculling          GX_CULL_NONE
#define cull_clockwise          GX_CULL_BACK
#define cull_counterclockwise   GX_CULL_FRONT
#endif