#include "z64shrink_window.h"
#include "z64view.h"
#include "disable_letterbox.h"

RECOMP_PATCH void View_ApplyLetterbox(View* view) {
    s32 letterboxY;
    s32 letterboxX;
    s32 pad1;
    s32 ulx;
    s32 uly;
    s32 lrx;
    s32 lry;

    if (TWEAK_CONFIG_NO_LETTERBOX) {
        return;
    }

    OPEN_DISPS(view->gfxCtx);

    letterboxY = ShrinkWindow_Letterbox_GetSize();

    letterboxX = -1; // The following is optimized to varX = 0 but affects codegen

    if (letterboxX < 0) {
        letterboxX = 0;
    }
    if (letterboxX > (SCREEN_WIDTH / 2)) {
        letterboxX = SCREEN_WIDTH / 2;
    }

    if (letterboxY < 0) {
        letterboxY = 0;
    } else if (letterboxY > (SCREEN_HEIGHT / 2)) {
        letterboxY = SCREEN_HEIGHT / 2;
    }

    ulx = view->viewport.leftX + letterboxX;
    uly = view->viewport.topY + letterboxY;
    lrx = view->viewport.rightX - letterboxX;
    lry = view->viewport.bottomY - letterboxY;

    gDPPipeSync(POLY_OPA_DISP++);
    {
        s32 pad2;
        Gfx* polyOpa;

        polyOpa = POLY_OPA_DISP;
        View_SetScissor(&polyOpa, ulx, uly, lrx, lry);
        POLY_OPA_DISP = polyOpa;
    }

    gDPPipeSync(POLY_XLU_DISP++);
    {
        Gfx* polyXlu;
        s32 pad3;

        polyXlu = POLY_XLU_DISP;
        View_SetScissor(&polyXlu, ulx, uly, lrx, lry);
        POLY_XLU_DISP = polyXlu;
    }

    CLOSE_DISPS(view->gfxCtx);
}

RECOMP_PATCH void ShrinkWindow_Draw(GraphicsContext* gfxCtx) {
    Gfx* gfx;
    s8 letterboxSize = sShrinkWindowPtr->letterboxSize;
    s8 pillarboxSize = sShrinkWindowPtr->pillarboxSize;

    if (TWEAK_CONFIG_NO_LETTERBOX) {
        return;
    }

    if (letterboxSize > 0) {
        OPEN_DISPS(gfxCtx);

        gfx = OVERLAY_DISP;

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetRenderMode(gfx++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(gfx++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));
        gDPFillRectangle(gfx++, 0, 0, gScreenWidth - 1, letterboxSize - 1);
        gDPFillRectangle(gfx++, 0, gScreenHeight - letterboxSize, gScreenWidth - 1, gScreenHeight - 1);

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 0);
        gDPFillRectangle(gfx++, 0, letterboxSize, gScreenWidth, letterboxSize + 1);
        gDPFillRectangle(gfx++, 0, gScreenHeight - letterboxSize - 1, gScreenWidth, gScreenHeight - letterboxSize);

        gDPPipeSync(gfx++);
        OVERLAY_DISP = gfx++;

        CLOSE_DISPS(gfxCtx);
    }

    if (pillarboxSize > 0) {
        OPEN_DISPS(gfxCtx);

        gfx = OVERLAY_DISP;

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetRenderMode(gfx++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetFillColor(gfx++, (GPACK_RGBA5551(0, 0, 0, 1) << 16) | GPACK_RGBA5551(0, 0, 0, 1));

        gDPFillRectangle(gfx++, 0, 0, pillarboxSize - 1, gScreenHeight - 1);
        gDPFillRectangle(gfx++, gScreenWidth - pillarboxSize, 0, gScreenWidth - 1, gScreenHeight - 1);

        gDPPipeSync(gfx++);
        gDPSetCycleType(gfx++, G_CYC_1CYCLE);
        gDPSetRenderMode(gfx++, G_RM_XLU_SURF, G_RM_XLU_SURF2);
        gDPSetPrimColor(gfx++, 0, 0, 0, 0, 0, 0);

        gDPFillRectangle(gfx++, pillarboxSize, 0, pillarboxSize + 2, gScreenHeight);
        gDPFillRectangle(gfx++, gScreenWidth - pillarboxSize - 2, 0, gScreenWidth - pillarboxSize, gScreenHeight);

        gDPPipeSync(gfx++);
        OVERLAY_DISP = gfx++;

        CLOSE_DISPS(gfxCtx);
    }
}
