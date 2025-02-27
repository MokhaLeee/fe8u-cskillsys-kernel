#include "common-chax.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

#if defined(SID_Mine) && (COMMON_SKILL_VALID(SID_Mine))
u8 Mine_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (GetUnitCurrentHp(gActiveUnit) <= SKILL_EFF0(SID_Mine))
		return MENU_DISABLED;

	if (!HasSelectTarget(gActiveUnit, MakeTargetListForMine))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

static u8 Mine_OnSelectTarget(ProcPtr proc, struct SelectTarget *target)
{
	gActionData.xOther = target->x;
	gActionData.yOther = target->y;

	HideMoveRangeGraphics();

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(BG2_SYNC_BIT);

	gActionData.unk08 = SID_Mine;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 Mine_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
		return MENU_ACT_SND6B;
	}

	ClearBg0Bg1();

	MakeTargetListForMine(gActiveUnit);
	BmMapFill(gBmMapMovement, -1);

	StartSubtitleHelp(
		NewTargetSelection_Specialized(&gSelectInfo_PutTrap, Mine_OnSelectTarget),
		GetStringFromIndex(0x87E));

	PlaySoundEffect(0x6A);
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void callback_anim(ProcPtr proc)
{
	StartMineAnim(proc, gActionData.xOther, gActionData.yOther);
}

static void callback_exec(ProcPtr proc)
{
	AddTrap(gActionData.xOther, gActionData.yOther, TRAP_MINE, 0);
	AddUnitHp(gActiveUnit, -SKILL_EFF0(SID_Mine));
}

bool Action_Mine(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, gActionData.unk08, callback_anim, callback_exec);
	return true;
}
#endif
