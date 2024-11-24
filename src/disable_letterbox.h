#include "tweak_globals.h"
#include "global.h"

typedef struct {
    /* 0x0 */ s8 letterboxTarget;
    /* 0x1 */ s8 letterboxSize;
    /* 0x2 */ s8 pillarboxTarget;
    /* 0x3 */ s8 pillarboxSize;
} ShrinkWindow; // size = 0x4

ShrinkWindow sShrinkWindow;
ShrinkWindow* sShrinkWindowPtr;

void View_SetScissor(Gfx** gfx, s32 ulx, s32 uly, s32 lrx, s32 lry);
