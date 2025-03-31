#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"

#if defined(SID_Sellsword) && (COMMON_SKILL_VALID(SID_Sellsword))
u8 Sellsword_Usability(const struct MenuItemDef * def, int number)
{
    int partyGold = GetPartyGoldAmount();

    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (CheckBitUES(gActiveUnit, UES_BIT_SELLSWORD_SKILL_USED))
        return MENU_NOTSHOWN;
    
    if (partyGold < 1000) 
        return MENU_NOTSHOWN;

    return MENU_ENABLED;

}

u8 Sellsword_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
        return MENU_ACT_SND6B;
    }

    gActionData.unk08 = SID_Sellsword;
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
    int partyGold = GetPartyGoldAmount();

    SetPartyGoldAmount(partyGold - 1000);

    NewPopup_VerySimple(MSG_ThousandGoldSpent, 0x5A, proc);
}

bool Action_Sellsword(ProcPtr parent)
{
    SetBitUES(gActiveUnit, UES_BIT_SELLSWORD_SKILL_USED);
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
    return true;
}
#endif
