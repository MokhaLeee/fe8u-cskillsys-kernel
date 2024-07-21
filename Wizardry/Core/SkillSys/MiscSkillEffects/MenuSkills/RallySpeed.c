#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "kernel-lib.h"
#include "debuff.h"

u8 RallySpeed_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    int i;
    bool allies_in_range = false;

    for (i = 0; i < ARRAY_COUNT_RANGE2x2; i++)
    {
        int x = gActiveUnit->xPos + gVecs_2x2[i].x;
        int y = gActiveUnit->yPos + gVecs_2x2[i].y;

        struct Unit * tunit = GetUnitAtPosition(x, y);
        if (!tunit)
            continue;

        if (tunit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

        if (AreUnitsAllied(gActiveUnit->index, tunit->index))
        {
            allies_in_range = true;
            break;
        }
    }
    
    if (!allies_in_range)
        return MENU_DISABLED;

    return MENU_ENABLED;
}

u8 RallySpeed_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_RallySpeed_FRtext);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = SID_RallySpeed;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

bool Action_RallySpeed(ProcPtr parent)
{
    int i;
    for (i = 0; i < ARRAY_COUNT_RANGE2x2; i++)
    {
        int x = gActiveUnit->xPos + gVecs_2x2[i].x;
        int y = gActiveUnit->yPos + gVecs_2x2[i].y;

        struct Unit * tunit = GetUnitAtPosition(x, y);
        if (!tunit)
            continue;

        if (tunit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

        if (AreUnitsAllied(gActiveUnit->index, tunit->index))
        {
            SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_RALLY_SPD);
        }
    }

    return true;
}
