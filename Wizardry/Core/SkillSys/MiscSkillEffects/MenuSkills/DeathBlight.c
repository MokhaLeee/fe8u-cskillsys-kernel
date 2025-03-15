#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"
#include "debuff.h"
#include "jester_headers/miscellaenous.h"

#if defined(SID_DeathBlight) && (COMMON_SKILL_VALID(SID_DeathBlight))

u8 DeathBlight_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (CheckBitUES(gActiveUnit, UES_BIT_DEATHBLIGHT_SKILL_USED))
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForAdjacentEnemies))
		return MENU_NOTSHOWN;

    return MENU_ENABLED;
}

static u8 DeathBlight_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    gActionData.unk08 = SID_DeathBlight;
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 DeathBlight_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_No_Allies);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForAdjacentEnemies(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, DeathBlight_OnSelectTarget),
        GetStringFromIndex(MSG_MenuSkill_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
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
	struct Unit* unit_tar = GetUnit(gActionData.targetIndex);
	SetUnitStatus(unit_tar, NEW_UNIT_STATUS_DOOM);
}

bool Action_DeathBlight(ProcPtr parent)
{
    SetBitUES(gActiveUnit, UES_BIT_DEATHBLIGHT_SKILL_USED);
	NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
	return true;
}

#endif