#include "z64shrink_window.h"
#include "z64view.h"
#include "disable_letterbox.h"

RECOMP_FORCE_PATCH void View_ApplyLetterbox(View* view) {
    return;
}

RECOMP_FORCE_PATCH void ShrinkWindow_Draw(GraphicsContext* gfxCtx) {
    return;
}
