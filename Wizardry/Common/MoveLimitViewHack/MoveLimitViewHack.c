#include "global.h"
#include "playerphase.h"
#include "hardware.h"

#include "common-chax.h"
#include "weapon-range.h"

#define gPalBlueRangeSquare gUnknown_08A02F34
#define gPalRedRangeSquare gUnknown_08A02F94
#define gPalGreenRangeSquare gUnknown_08A02FF4

void MoveLimitView_OnLoop(struct MoveLimitViewProc * proc)
{
    int frame = (GetGameClock() / 2) & 31;

    if (proc->flags & MOVLIMITV_MMAP_BLUE)
        CopyToPaletteBuffer(gPalBlueRangeSquare + frame, 0x82, 0x20);

    if (proc->flags & MOVLIMITV_RMAP_RED)
        CopyToPaletteBuffer(gPalRedRangeSquare + frame, 0xA2, 0x20);

    if (proc->flags & MOVLIMITV_RMAP_GREEN)
        CopyToPaletteBuffer(gPalGreenRangeSquare + frame, 0xA2, 0x20);

    if (proc->flags & MOVLIMITV_RMAP_BLUE)
        CopyToPaletteBuffer(gPalBlueRangeSquare + frame, 0xA2, 0x20);

    /* New */
    if (proc->flags & MOVLIMITV_MMAP_RED)
        CopyToPaletteBuffer(gPalRedRangeSquare + frame, 0x82, 0x20);

    if (proc->flags & MOVLIMITV_MMAP_GREEN)
        CopyToPaletteBuffer(gPalGreenRangeSquare + frame, 0x82, 0x20);
}
