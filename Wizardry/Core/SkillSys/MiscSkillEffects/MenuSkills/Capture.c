#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "battle-system.h"
#include "unit-expa.h"

#if defined(SID_Capture) && (COMMON_SKILL_VALID(SID_Capture))
u8 Capture_Usability(const struct MenuItemDef * def, int number)
{
    int i;
    bool enemies = false;

    if (gActiveUnit->state & US_CANTOING || gActiveUnit->state & US_RESCUING)
        return MENU_NOTSHOWN;

    /**
     * Since we can't use gBattleStats.range at this point, we check to
     * see if there are any adjacent enemies instead
     */
    for (i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
    {
        int _x = gActiveUnit->xPos + gVecs_1x1[i].x;
        int _y = gActiveUnit->yPos + gVecs_1x1[i].y;

        struct Unit * unit_enemy = GetUnitAtPosition(_x, _y);
        if (!UNIT_IS_VALID(unit_enemy))
            continue;

        if (unit_enemy->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

        if ((gActiveUnit->conBonus + gActiveUnit->pClassData->baseCon + gActiveUnit->pCharacterData->baseCon) <=
            (unit_enemy->conBonus + unit_enemy->pClassData->baseCon + unit_enemy->pCharacterData->baseCon))
            continue;

        if (!AreUnitsAllied(gActiveUnit->index, unit_enemy->index))
        {
            enemies = true;
            break;
        }
    }

    if (!enemies)
        return MENU_NOTSHOWN;

    if (AttackCommandUsability(def, number) == MENU_ENABLED)
        return MENU_ENABLED;

    return MENU_NOTSHOWN;
}

u8 Capture_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, 0x858); // TODO: msgid "There's no more ammo for[NL]the ballista.[.]"
        return MENU_ACT_SND6B;
    }

    ResetIconGraphics();

    LoadIconPalettes(4);

    if (gActiveUnit->state & US_IN_BALLISTA)
    {
        return StartUnitBallistaSelect(menu, item);
    }

    gActionData.unk08 = SID_Capture;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    /**
     * Usually SetBitUES is preferred, but for some fucking insane reason
     * the bit won't persist when I check unitB (the active unit) in the 'KillUnitOnCombatDeath' function.
     * So I'm having to set the bit directly using its positional value in unit-expa.h
     */
    // SetBitUES(gActiveUnit, UES_BIT_CAPTURE_SKILL_USED);
    gActiveUnit->_u3A = 8;

    return StartUnitWeaponSelect(menu, item);
}

bool Action_Capture(ProcPtr parent)
{
    return true;
}
#endif