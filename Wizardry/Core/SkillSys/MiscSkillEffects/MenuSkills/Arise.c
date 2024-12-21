#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

#if defined(SID_Arise) && (COMMON_SKILL_VALID(SID_Arise))
u8 Arise_Usability(const struct MenuItemDef * def, int number)
{
    struct Unit * unit;

    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    for (int i = FACTION_BLUE + 1; i < (FACTION_BLUE + 0x40); i++) 
    {
        unit = GetUnit(i);
        
        if (!UNIT_IS_VALID(unit))
            return MENU_NOTSHOWN;

    /* The unit is dead, we're still on the chapter they died on and they haven't been revived before */
    if ((unit->state & US_DEAD) && (unit->state & US_HIDDEN)) 
        if ((unit->ballistaIndex == gPlaySt.chapterIndex) && (unit->ai1 != 0xFF))
            return MENU_ENABLED;
    }

    return MENU_NOTSHOWN;

}

u8 Arise_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = SID_Arise;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
    
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void callback_anim(ProcPtr proc)
{
    PlaySoundEffect(0x269);
    Proc_StartBlocking(ProcScr_DanceringAnim, proc);

    BG_SetPosition(
        BG_0,
        -SCREEN_TILE_IX(gActiveUnit->xPos - 1),
        -SCREEN_TILE_IX(gActiveUnit->yPos - 2));
}

static void callback_exec(ProcPtr proc)
{
    struct Unit * unit;

    for (int i = FACTION_BLUE + 1; i < (FACTION_BLUE + 0x40); i++) 
    {
        unit = GetUnit(i);
        if ((unit->state & US_DEAD) && (unit->state & US_HIDDEN)) 
        {
            /* Since we're dealing with player units, set a value to check in an AI bit to prevent multiple resurrections */
            if ((unit->ballistaIndex == gPlaySt.chapterIndex) && (unit->ai1 != 0xFF))
            {
                unit->state &= ~(US_HIDDEN | US_DEAD);
                unit->curHP = 1;
                unit->ai1 = 0xFF;
            }
        }
    }

    NewPopup_VerySimple(MSG_UnitRevived, 0x5A, proc);
}

bool Action_Arise(ProcPtr parent)
{
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
    return true;
}
#endif
