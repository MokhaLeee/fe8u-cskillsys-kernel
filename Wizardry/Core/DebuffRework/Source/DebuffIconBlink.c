#include "global.h"
#include "bmunit.h"
#include "bmudisp.h"
#include "ctc.h"

#include "common-chax.h"
#include "debuff.h"

extern u16 sSprite_SysUpArrowA[], sSprite_SysUpArrowB[], sSprite_SysUpArrowC[];
extern u16 sSprite_SysDownArrowA[], sSprite_SysDownArrowB[], sSprite_SysDownArrowC[];

STATIC_DECLAR void PutUnitStatusBuffIcon(struct Unit * unit)
{
    int ix = unit->xPos * 16 - gBmSt.camera.x;
    int iy = unit->yPos * 16 - gBmSt.camera.y;

    if (ix < -16 || ix > DISPLAY_WIDTH)
        return;

    if (iy < -16 || iy > DISPLAY_HEIGHT)
        return;

    if ((GetGameClock() & 0x3F) < 0x28)
        return;

    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix - 1),
        OAM0_Y(0x100 + iy),
        sSprite_SysUpArrowA,
        0);
}

STATIC_DECLAR void PutUnitStatusDebuffIcon(struct Unit * unit)
{
    int ix = unit->xPos * 16 - gBmSt.camera.x;
    int iy = unit->yPos * 16 - gBmSt.camera.y;

    if (ix < -16 || ix > DISPLAY_WIDTH)
        return;

    if (iy < -16 || iy > DISPLAY_HEIGHT)
        return;

    if ((GetGameClock() & 0x3F) < 0x28)
        return;

    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix - 1),
        OAM0_Y(0x100 + iy),
        sSprite_SysDownArrowA,
        0);
}

/* Called from HpBar */
void PutUnitStatusIcon(struct Unit * unit)
{
    int status = GetUnitStatusIndex(unit);
    if (status != 0)
    {
        const struct DebuffInfo * info = &gpDebuffInfos[status];
        if (info->on_draw)
            info->on_draw(unit);
        else
        {
            switch (info->type) {
            case STATUS_INFO_TYPE_DEBUFF:
                PutUnitStatusDebuffIcon(unit);
                break;

            case STATUS_INFO_TYPE_BUFF:
                PutUnitStatusBuffIcon(unit);
                break;
            }
        }
    }
}

/* External on-draw functions */
void PutUnitDanceRingBuffIcon(struct Unit * unit)
{
    const u16 obj[] = {
        2,
        0x4000, 0x0000, OAM2_PAL(0x1) + OAM2_LAYER(0x2) + OAM2_CHR(0x600 / 0x20),
        0x0008, 0x0000, OAM2_PAL(0x1) + OAM2_LAYER(0x2) + OAM2_CHR(0xA00 / 0x20),
    };

    int ix = unit->xPos * 16 - gBmSt.camera.x;
    int iy = unit->yPos * 16 - gBmSt.camera.y;

    if (ix < -16 || ix > DISPLAY_WIDTH)
        return;

    if (iy < -16 || iy > DISPLAY_HEIGHT)
        return;

    if ((GetGameClock() & 0x3F) < 0x28)
        return;

    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix - 1),
        OAM0_Y(0x100 + iy - 5),
        obj,
        0);    
}
