#include "global.h"
#include "bmunit.h"
#include "fontgrp.h"
#include "statscreen.h"
#include "anime.h"
#include "ekrbattle.h"
#include "ekrdragon.h"
#include "efxbattle.h"
#include "hardware.h"

#include "common-chax.h"
#include "debuff.h"

/* LynJump */
void SetUnitStatus(struct Unit * unit, int status)
{
    if (status == 0 || status >= NEW_UNIT_STATUS_MAX)
    {
        Errorf("Status error: %d", status);

        SetUnitStatusIndex(unit, 0);
        SetUnitStatusDuration(unit, 0);
    }
    else
    {
        int duration = gpDebuffInfos[status].duration;
        if (duration == 0)
            duration = 3;

        SetUnitStatusIndex(unit, status);
        SetUnitStatusDuration(unit, duration);
    }
}

/* LynJump */
void SetUnitStatusExt(struct Unit * unit, int status, int duration)
{
    if (status >= NEW_UNIT_STATUS_MAX)
    {
        Errorf("Status overflow: %d", status);
        return;
    }

    if (duration == 0)
        duration = 3;

    SetUnitStatusIndex(unit, status);
    SetUnitStatusDuration(unit, duration);
}

/* LynJump */
char * GetUnitStatusName(struct Unit * unit)
{
    int msg = gpDebuffInfos[GetUnitStatusIndex(unit)].name;

    if (msg == 0)
        msg = 0x52B; /* --[X] */

    return GetStringFromIndex(msg);
}

/* LynJump */
char * GetUnitRescueName(struct Unit * unit)
{
    if (!unit->rescue)
        return GetStringFromIndex(gpDebuffInfos[UNIT_STATUS_NONE].name);

    return GetStringFromIndex(GetUnit(unit->rescue)->pCharacterData->nameTextId);
}

/* LynJump */
void HbPopulate_SSStatus(struct HelpBoxProc * proc)
{
	u8 index = GetUnitStatusIndex(gStatScreen.unit);
	proc->mid = gpDebuffInfos[index].desc;
}

/* LynJump */
void MMB_DrawStatusText(s16 * buffer, struct Unit * unit)
{
    int tileIdx = 0x16F;
    const u8 * img;

    if (!unit)
        return;

    img = gpDebuffInfos[GetUnitStatusIndex(unit)].img;

    if (img)
    {
        CpuFastCopy(img, (void *)BG_VRAM + 0x2DE0, 0xA0);

        buffer[0] = tileIdx++;
        buffer[1] = tileIdx++;
        buffer[2] = tileIdx++;
        buffer[3] = tileIdx++;
        buffer[4] = tileIdx++;
        buffer[5] = 0;
        buffer[6] = unit->statusDuration + 0x1128;
    }
}
