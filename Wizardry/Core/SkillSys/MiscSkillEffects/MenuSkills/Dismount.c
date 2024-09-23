#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "debuff.h"
#include "class-pairs.h"

#if defined(SID_Dismount) && (COMMON_SKILL_VALID(SID_Dismount))
// Predefine an array of key-value pairs
const int dismountPairs[14][2] = {
    { CLASS_EIRIKA_MASTER_LORD, CLASS_EIRIKA_LORD },
    { CLASS_EPHRAIM_MASTER_LORD, CLASS_EPHRAIM_LORD },
    { CLASS_PALADIN, CLASS_SOLDIER },
    { CLASS_PALADIN_F, CLASS_SOLDIER },
    { CLASS_GREAT_KNIGHT, CLASS_SOLDIER },
    { CLASS_GREAT_KNIGHT_F, CLASS_SOLDIER },
    { CLASS_PEGASUS_KNIGHT, CLASS_SOLDIER },
    { CLASS_FALCON_KNIGHT, CLASS_SOLDIER },
    { CLASS_WYVERN_RIDER, CLASS_SOLDIER },
    { CLASS_WYVERN_RIDER_F, CLASS_SOLDIER },
    { CLASS_WYVERN_KNIGHT, CLASS_SOLDIER },
    { CLASS_WYVERN_KNIGHT_F, CLASS_SOLDIER },
    { CLASS_WYVERN_LORD, CLASS_SOLDIER },
    { CLASS_WYVERN_LORD_F, CLASS_SOLDIER },
};

// Define the size of the array
const int dismountListSize = sizeof(dismountPairs) / sizeof(dismountPairs[0]);

u8 Dismount_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

u8 Dismount_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = SID_Dismount;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void callback_anim(ProcPtr proc)
{
    // PlaySoundEffect(0x269);
    // Proc_StartBlocking(ProcScr_DanceringAnim, proc);

    // BG_SetPosition(
    //     BG_0,
    //     -SCREEN_TILE_IX(gActiveUnit->xPos - 1),
    //     -SCREEN_TILE_IX(gActiveUnit->yPos - 2));
}

static void callback_exec(ProcPtr proc)
{
    for (int i = 0; i < dismountListSize; i++)
    {
        if (gActiveUnit->pClassData->number == dismountPairs[i][0])
            gActiveUnit->pClassData = GetClassData(dismountPairs[i][1]);
        else if (gActiveUnit->pClassData->number == dismountPairs[i][1])
            gActiveUnit->pClassData = GetClassData(dismountPairs[i][0]);
    }
}

bool Action_Dismount(ProcPtr parent)
{
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
    return true;
}
#endif