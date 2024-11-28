#include "z64shrink_window.h"
#include "z64view.h"
#include "disable_letterbox.h"

RECOMP_PATCH void View_ApplyLetterbox(View* view) {
    return;
}

RECOMP_PATCH void ShrinkWindow_Draw(GraphicsContext* gfxCtx) {
    return;
}
