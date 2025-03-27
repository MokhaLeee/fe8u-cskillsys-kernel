#include "common-chax.h"
#include "debuff.h"
#include "weapon-range.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

STATIC_DECLAR void AddTargetForRally(struct Unit *unit)
{
	if (UnitOnMapAvaliable(unit) && AreUnitsAllied(gSubjectUnit->index, unit->index))
		AddTarget(unit->xPos, unit->yPos, unit->index, 1);
}

STATIC_DECLAR void MakeTargetListForRally(struct Unit *unit)
{
	int x = unit->xPos;
	int y = unit->yPos;

	gSubjectUnit = unit;
	BmMapFill(gBmMapRange, 0);
	MapAddInBoundedRange(x, y, 1, 2);

	InitTargets(x, y);
	ForEachUnitInRange(AddTargetForRally);
}

u8 Rally_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (!HasSelectTarget(gActiveUnit, MakeTargetListForRally))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

int Rally_Hover(struct MenuProc *menu, struct MenuItemProc *item)
{
	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);
	GenerateUnitStandingReachRange(gActiveUnit, 0b110);
	DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
	return 0;
}

int Rally_Unhover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	HideMoveRangeGraphics();
	return 0;
}

u8 Rally_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_MenuSkill_Rally_FRtext);
		return MENU_ACT_SND6B;
	}

	gActionData.unk08 = UnitMenuSkills[MENU_SKILL_INDEX(item->def)];
	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

static void callback_anim(ProcPtr proc)
{
	StartLightRuneAnim(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void callback_exec(ProcPtr proc)
{
	int i;

	MakeTargetListForRally(gActiveUnit);

	for (i = 0; i < GetSelectTargetCount(); i++) {
		struct Unit *unit = GetUnit(GetTarget(i)->uid);

		if (!UnitOnMapAvaliable(unit))
			continue;

		switch (gActionData.unk08) {
#if defined(SID_RallyStrength) && (COMMON_SKILL_VALID(SID_RallyStrength))
		case SID_RallyStrength:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_POW);
			break;
#endif

#if defined(SID_RallyMagic) && (COMMON_SKILL_VALID(SID_RallyMagic))
		case SID_RallyMagic:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MAG);
			break;
#endif

#if defined(SID_RallySkill) && (COMMON_SKILL_VALID(SID_RallySkill))
		case SID_RallySkill:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SKL);
			break;
#endif

#if defined(SID_RallySpeed) && (COMMON_SKILL_VALID(SID_RallySpeed))
		case SID_RallySpeed:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SPD);
			break;
#endif

#if defined(SID_RallyLuck) && (COMMON_SKILL_VALID(SID_RallyLuck))
		case SID_RallyLuck:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_LCK);
			break;
#endif

#if defined(SID_RallyDefense) && (COMMON_SKILL_VALID(SID_RallyDefense))
		case SID_RallyDefense:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_DEF);
			break;
#endif

#if defined(SID_RallyResistance) && (COMMON_SKILL_VALID(SID_RallyResistance))
		case SID_RallyResistance:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_RES);
			break;
#endif

#if defined(SID_RallyMovement) && (COMMON_SKILL_VALID(SID_RallyMovement))
		case SID_RallyMovement:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MOV);
			break;
#endif

#if defined(SID_RallySpectrum) && (COMMON_SKILL_VALID(SID_RallySpectrum))
		case SID_RallySpectrum:
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_POW);
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MAG);
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SKL);
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_SPD);
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_LCK);
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_DEF);
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_RES);
			SetUnitStatDebuff(unit, UNIT_STAT_BUFF_RALLY_MOV);
			break;
#endif
		}
	}
}

bool Action_Rally(ProcPtr parent)
{
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, gActionData.unk08, callback_anim, callback_exec);
	return true;
}
