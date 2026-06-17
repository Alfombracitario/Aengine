// format conversions
#if platformColorFormat == 1555 && platformColorOrder == colorOrderARGB
#define colorRGBA(r,g,b,a) \
    (u16)( (((a) > 127 ? 1 : 0) << 15) | \
           (((r) >> 3)          << 10) | \
           (((g) >> 3)          <<  5) | \
            ((b) >> 3) )

#elif platformColorFormat == 555 && platformColorOrder == colorOrderBGR
// SNES native: BGR555
#define colorRGBA(r,g,b,a) \
    (u16)( (((b) >> 3) << 10) | \
           (((g) >> 3) <<  5) | \
            ((r) >> 3) )

#elif platformColorFormat == 565 && platformColorOrder == colorOrderRGB
#define colorRGBA(r,g,b,a) \
    (u16)( (((r) >> 3) << 11) | \
           (((g) >> 2) <<  5) | \
            ((b) >> 3) )

#elif platformColorFormat == 8888 && platformColorOrder == colorOrderRGBA
#define colorRGBA(r,g,b,a) \
    (u32)( ((u32)(r) << 24) | \
           ((u32)(g) << 16) | \
           ((u32)(b) <<  8) | \
            (u32)(a) )

#elif platformColorFormat == 8888 && platformColorOrder == colorOrderARGB
#define colorRGBA(r,g,b,a) \
    (u32)( ((u32)(a) << 24) | \
           ((u32)(r) << 16) | \
           ((u32)(g) <<  8) | \
            (u32)(b) )

#elif platformColorFormat == 8888 && platformColorOrder == colorOrderBGRA
#define colorRGBA(r,g,b,a) \
    (u32)( ((u32)(b) << 24) | \
           ((u32)(g) << 16) | \
           ((u32)(r) <<  8) | \
            (u32)(a) )

#elif platformColorFormat == 888 && platformColorOrder == colorOrderRGB
// no alpha channel
#define colorRGBA(r,g,b,a) \
    (u32)( ((u32)(r) << 16) | \
           ((u32)(g) <<  8) | \
            (u32)(b) )

#else
// maybe in a future I could add more formats

#endif

// BPP channel scale + compile-time range check
// v must be in [0, (2^bpp)-1] or compilation fails
#define _BPP_SCALE(v, bpp) \
    ( _Static_assert((v) <= (1 << (bpp)) - 1, \
        "colorBpp: channel value out of range for given bpp"), \
      ((v) * 255) / ((1 << (bpp)) - 1) )

// ─────────────────────────────────────────────────────────────────
//  Public macros
// ─────────────────────────────────────────────────────────────────

// from 0 to 1.0f
#define colorF(r,g,b,a) \
    colorRGBA( (u8)((r) * 255), \
               (u8)((g) * 255), \
               (u8)((b) * 255), \
               (u8)((a) * 255) )

// u32 RGBA 8888
#define colorH8888(rgba) \
    colorRGBA( ((rgba) >> 24) & 0xFF, \
               ((rgba) >> 16) & 0xFF, \
               ((rgba) >>  8) & 0xFF, \
                (rgba)        & 0xFF )

// u16 ARGB1555
#if platformColorFormat == 1555 && platformColorOrder == colorOrderARGB
#define colorH1555(argb) \
    (u16)(argb)
#else
#define colorH1555(argb) \
    colorRGBA( (((argb) >> 10) & 0x1F) * 255 / 31, \
               (((argb) >>  5) & 0x1F) * 255 / 31, \
                ((argb)        & 0x1F) * 255 / 31, \
                ((argb) >> 15) ? 0xFF : 0x00 )
#endif

// -- no alpha --

// u32 RGB888
#define colorH888(rgb) \
    colorRGBA( ((rgb) >> 16) & 0xFF, \
               ((rgb) >>  8) & 0xFF, \
                (rgb)        & 0xFF, \
               0xFF )

// u16 RGB565
#define colorH565(rgb) \
    colorRGBA( (((rgb) >> 11) & 0x1F) * 255 / 31, \
               (((rgb) >>  5) & 0x3F) * 255 / 63, \
                ((rgb)        & 0x1F) * 255 / 31, \
               0xFF )

// compile error if any channel exceeds the valid range
#define colorBpp(r,g,b,a,bpp) \
    colorRGBA( _BPP_SCALE(r, bpp), \
               _BPP_SCALE(g, bpp), \
               _BPP_SCALE(b, bpp), \
               _BPP_SCALE(a, bpp) )

#define cBlack   colorH888(0x000000)
#define cMarron  colorH888(0x800000)
#define cGreen   colorH888(0x008000)
#define cOlive   colorH888(0x808000)
#define cNavy    colorH888(0x000080)
#define cPurple  colorH888(0x800080)
#define cTeal    colorH888(0x008080)
#define cGray    colorH888(0x808080)
#define cSilver  colorH888(0xC0C0C0)
#define cRed     colorH888(0xFF0000)
#define cLime    colorH888(0x00FF00)
#define cYellow  colorH888(0xFFFF00)
#define cOrange  colorH888(0xFF7F00)
#define cBlue    colorH888(0x0000FF)
#define cFuchsia colorH888(0xFF00FF)
#define cAqua    colorH888(0x00FFFF)
#define cWhite   colorH888(0xFFFFFF)

//not compile time functions
inline u32 mergeColor(u32 oldColor, u32 newColor, float t){
    float s = 1.0f - t;
    u8 r = (u8)(((oldColor >> 16) & 0xFF) * s + ((newColor >> 16) & 0xFF) * t);
    u8 g = (u8)(((oldColor >>  8) & 0xFF) * s + ((newColor >>  8) & 0xFF) * t);
    u8 b = (u8)(( oldColor        & 0xFF) * s + ( newColor        & 0xFF) * t);
    u8 a = (u8)(((oldColor >> 24) & 0xFF) * s + ((newColor >> 24) & 0xFF) * t);
    return (a << 24) | (r << 16) | (g << 8) | b;
}

inline u32 mergeColorInt(u32 oldColor, u32 newColor, u8 t){
    u8 s = 255 - t;
    u8 r = (((oldColor >> 16) & 0xFF) * s + ((newColor >> 16) & 0xFF) * t) >> 8;
    u8 g = (((oldColor >>  8) & 0xFF) * s + ((newColor >>  8) & 0xFF) * t) >> 8;
    u8 b = (( oldColor        & 0xFF) * s + ( newColor        & 0xFF) * t) >> 8;
    u8 a = (((oldColor >> 24) & 0xFF) * s + ((newColor >> 24) & 0xFF) * t) >> 8;
    return (a << 24) | (r << 16) | (g << 8) | b;
}