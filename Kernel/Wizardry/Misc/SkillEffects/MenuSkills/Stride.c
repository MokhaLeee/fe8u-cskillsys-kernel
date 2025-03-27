#include "common-chax.h"
#include "debuff.h"
#include "weapon-range.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

#if defined(SID_Stride) && (COMMON_SKILL_VALID(SID_Stride))
STATIC_DECLAR void AddTargetForStride(struct Unit *unit)
{
	if (UnitOnMapAvaliable(unit) && AreUnitsAllied(gSubjectUnit->index, unit->index))
		AddTarget(unit->xPos, unit->yPos, unit->index, 1);
}

STATIC_DECLAR void MakeTargetListForStride(struct Unit *unit)
{
	int x = unit->xPos;
	int y = unit->yPos;

	gSubjectUnit = unit;
	BmMapFill(gBmMapRange, 0);
	MapAddInBoundedRange(x, y, 1, 3);

	InitTargets(x, y);
	ForEachUnitInRange(AddTargetForStride);
}

u8 Stride_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (!HasSelectTarget(gActiveUnit, MakeTargetListForStride))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

int Stride_Hover(struct MenuProc *menu, struct MenuItemProc *item)
{
	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);
	GenerateUnitStandingReachRange(gActiveUnit, 0b1001);
	DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
	return 0;
}

int Stride_Unhover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	HideMoveRangeGraphics();
	return 0;
}

u8 Stride_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_MenuSkill_Stride_FRtext);
		return MENU_ACT_SND6B;
	}

	gActionData.unk08 = SID_Stride;
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
	int i;

	MakeTargetListForStride(gActiveUnit);

	for (i = 0; i < GetSelectTargetCount(); i++) {
		struct Unit *unit = GetUnit(GetTarget(i)->uid);

		if (!UnitOnMapAvaliable(unit) && unit != gActiveUnit)
			continue;

		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_STRIDE);
	}
}

bool Action_Stride(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, gActionData.unk08, callback_anim, callback_exec);
	return true;
}
#endif
