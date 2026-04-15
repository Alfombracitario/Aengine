#include <ogc/video.h>
#include <ogc/gx.h>
#include <ogc/system.h>
#include <malloc.h>
#include <string.h>
#include <ogc/gu.h>
#include "engine/gc/video.h"
#ifdef HW_RVL
#include <ogc/conf.h>
#endif

void* gpFifo = nullptr;
void* xfb[2] = { nullptr, nullptr };
int   currentFb = 0;
GXRModeObj* rmode = nullptr;

void prepareVideo() {
    VIDEO_Init();
    VIDEO_SetBlack(true);

    rmode = VIDEO_GetPreferredMode(nullptr);

    // Wii-specific aspect ratio correction
#if defined(HW_RVL)
    if (CONF_GetAspectRatio() == CONF_ASPECT_16_9)
        rmode->viWidth = 678;
    else
        rmode->viWidth = 672;
    rmode->viXOrigin = (VI_MAX_WIDTH_NTSC - rmode->viWidth) / 2;
#endif

    VIDEO_Configure(rmode);

    xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    VIDEO_SetNextFramebuffer(xfb[currentFb]);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE)
        VIDEO_WaitVSync();

    // GX FIFO
    gpFifo = memalign(32, 256 * 1024);
    memset(gpFifo, 0, 256 * 1024);
    GX_Init(gpFifo, 256 * 1024);

    GX_SetCopyClear((GXColor){ 0, 0, 0, 0 }, GX_MAX_Z24);
    GX_SetPixelFmt(rmode->aa ? GX_PF_RGB565_Z16 : GX_PF_RGB8_Z24, GX_ZC_LINEAR);

    // Display copy setup
    f32 yscale    = GX_GetYScaleFactor(rmode->efbHeight, rmode->xfbHeight);
    u32 xfbHeight = GX_SetDispCopyYScale(yscale);
    GX_SetDispCopySrc(0, 0, rmode->fbWidth, rmode->efbHeight);
    GX_SetDispCopyDst(rmode->fbWidth, xfbHeight);
    GX_SetCopyFilter(rmode->aa, rmode->sample_pattern, GX_TRUE, rmode->vfilter);
    GX_SetFieldMode(rmode->field_rendering,
        (rmode->viHeight == 2 * rmode->xfbHeight) ? GX_ENABLE : GX_DISABLE);
    GX_SetDispCopyGamma(GX_GM_1_0);

    // Vertex setup
    GX_ClearVtxDesc();
    GX_InvVtxCache();
    GX_InvalidateTexAll();

    GX_SetVtxDesc(GX_VA_POS,  GX_DIRECT);
    GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);   // override per draw call when texturing

    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS,  GX_POS_XYZ,  GX_F32,   0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST,   GX_F32,   0);

    GX_SetZMode(GX_FALSE, GX_LEQUAL, GX_TRUE);
    GX_SetNumChans(1);
    GX_SetNumTexGens(1);
    GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

    // 2D orthographic projection
    Mtx modelView;
    guMtxIdentity(modelView);
    guMtxTransApply(modelView, modelView, 0.0f, 0.0f, -100.0f);
    GX_LoadPosMtxImm(modelView, GX_PNMTX0);

    Mtx44 projection;
    guOrtho(projection, 0, rmode->efbHeight, 0, rmode->fbWidth, 0.0f, 1000.0f);
    GX_LoadProjectionMtx(projection, GX_ORTHOGRAPHIC);

    GX_SetViewport(0, 0, rmode->fbWidth, rmode->efbHeight, 0.0f, 1.0f);
    GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GX_SetAlphaUpdate(GX_TRUE);
    GX_SetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GX_SetColorUpdate(GX_ENABLE);
    GX_SetCullMode(GX_CULL_NONE);

    VIDEO_SetBlack(false);
}

void platformRender() {
    GX_DrawDone();
    GX_InvalidateTexAll();

    currentFb ^= 1;

    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    GX_CopyDisp(xfb[currentFb], GX_TRUE);

    VIDEO_SetNextFramebuffer(xfb[currentFb]);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE)
        VIDEO_WaitVSync();
}

void exitVideo() {
    // Fill black twice to clear both framebuffers
    platformRender();
    platformRender();

    GX_DrawDone();    // <-- critical, must complete before abort
    GX_AbortFrame();

    if (xfb[0]) { free(MEM_K1_TO_K0(xfb[0])); xfb[0] = nullptr; }
    if (xfb[1]) { free(MEM_K1_TO_K0(xfb[1])); xfb[1] = nullptr; }
    if (gpFifo) { free(gpFifo); gpFifo = nullptr; }
}