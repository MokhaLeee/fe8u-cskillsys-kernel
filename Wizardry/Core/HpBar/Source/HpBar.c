#include "global.h"
#include "bmunit.h"
#include "chapterdata.h"
#include "bmudisp.h"
#include "ctc.h"

#include "common-chax.h"
#include "debuff.h"

STATIC_DECLAR int GetUnitHpBarIndex(struct Unit * unit)
{
    return Div(unit->curHP * 10, unit->maxHP);
}

void PutUnitHpBar(struct Unit * unit)
{
    int ix = unit->xPos * 16 - gBmSt.camera.x;
    int iy = unit->yPos * 16 - gBmSt.camera.y;
    int index;

    static const int vram_offsets[] = {
               0xE80, 0xE40,
        0xAC0, 0xA80, 0xA40,
        0x6C0, 0x680, 0x640,
        0x2C0, 0x280, 0x240
    };

    if (ix < -16 || ix > DISPLAY_WIDTH)
        return;

    if (iy < -16 || iy > DISPLAY_HEIGHT)
        return;

    index = GetUnitHpBarIndex(unit);

    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix),
        OAM0_Y(0x100 + iy + 10),
        gObject_16x8,
        OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(Div(vram_offsets[index], 0x20)));
}
