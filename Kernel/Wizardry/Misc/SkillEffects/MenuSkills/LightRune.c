#include "common-chax.h"
#include "map-anims.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

#if defined(SID_LightRune) && (COMMON_SKILL_VALID(SID_LightRune))
u8 LightRune_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (GetUnitCurrentHp(gActiveUnit) <= SKILL_EFF0(SID_LightRune))
		return MENU_DISABLED;

	if (!HasSelectTarget(gActiveUnit, MakeTargetListForLightRune))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

static u8 LightRune_OnSelectTarget(ProcPtr proc, struct SelectTarget *target)
{
	gActionData.xOther = target->x;
	gActionData.yOther = target->y;

	HideMoveRangeGraphics();

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(BG2_SYNC_BIT);

	gActionData.unk08 = SID_LightRune;
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

u8 LightRune_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_MenuSkill_CommonFail);
		return MENU_ACT_SND6B;
	}

	ClearBg0Bg1();

	MakeTargetListForLightRune(gActiveUnit);
	BmMapFill(gBmMapMovement, -1);

	StartSubtitleHelp(
		NewTargetSelection_Specialized(&gSelectInfo_PutTrap, LightRune_OnSelectTarget),
		GetStringFromIndex(0x87E));

	PlaySoundEffect(0x6A);
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void callback_anim(ProcPtr proc)
{
	StartLightRuneAnim(proc, gActionData.xOther, gActionData.yOther);
}

static void callback_exec(ProcPtr proc)
{
	AddLightRune(gActionData.xOther, gActionData.yOther);
	AddUnitHp(gActiveUnit, -SKILL_EFF0(SID_LightRune));
}

bool Action_LightRune(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, gActionData.unk08, callback_anim, callback_exec);
	return true;
}
#endif
