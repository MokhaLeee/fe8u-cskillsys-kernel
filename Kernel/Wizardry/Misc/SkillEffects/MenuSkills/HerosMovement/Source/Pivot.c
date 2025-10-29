#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "weapon-range.h"
#include "heros-movement.h"

STATIC_DECLAR void AddTargetForPivot(struct Unit *unit)
{
	if (UnitOnMapAvaliable(unit) && AreUnitsAllied(gSubjectUnit->index, unit->index)) {
		int tx = unit->xPos + (unit->xPos - gSubjectUnit->xPos);
		int ty = unit->yPos + (unit->yPos - gSubjectUnit->yPos);

		if (CanUnitStandOnPosition(gSubjectUnit, tx, ty))
			AddTarget(unit->xPos, unit->yPos, unit->index, 1);
	}
}

STATIC_DECLAR void MakeTargetListForPivot(struct Unit *unit)
{
	int x = unit->xPos;
	int y = unit->yPos;

	gSubjectUnit = unit;

	ForEachAdjacentUnit(x, y, AddTargetForPivot);
}

u8 Pivot_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (!HasSelectTarget(gActiveUnit, MakeTargetListForPivot))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

int Pivot_Hover(struct MenuProc *menu, struct MenuItemProc *item)
{
#if 0
	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);
	GenerateUnitStandingReachRange(gActiveUnit, 0b10);
	DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
#endif
	return 0;
}

int Pivot_Unhover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
#if 0
	HideMoveRangeGraphics();
#endif
	return 0;
}

static u8 select_target_on_switchin(ProcPtr proc, struct SelectTarget* target)
{
	int x, y;

	BmMapFill(gBmMapMovement, -1);

	x = 2 * target->x - gActiveUnit->xPos;
	y = 2 * target->y - gActiveUnit->yPos;

	gBmMapMovement[y][x] = 1;
	DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
	return 0;
}

static u8 select_target_on_select(ProcPtr proc, struct SelectTarget *target)
{
	gActionData.xOther = target->x;
	gActionData.yOther = target->y;

	HideMoveRangeGraphics();

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(BG2_SYNC_BIT);

#if defined(SID_Pivot) && (COMMON_SKILL_VALID(SID_Pivot))
	gActionData.unk08 = SID_Pivot;
#endif

	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

static u8 select_target_on_cancel(ProcPtr proc, struct SelectTarget* target)
{
	HideMoveRangeGraphics();

	return GenericSelection_BackToUM(proc, target);
}

STATIC_DECLAR const struct SelectInfo sSelectInfo_HmuPivot = {
	.onInit = NULL,
	.onEnd = NULL,
	.onSwitchIn = select_target_on_switchin,
	.onSelect = select_target_on_select,
	.onCancel = select_target_on_cancel,
	.onHelp = NULL,
};

u8 Pivot_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_HMU_ERROR_TERRAIN);
		return MENU_ACT_SND6B;
	}

	ClearBg0Bg1();

	MakeTargetListForPivot(gActiveUnit);
	BmMapFill(gBmMapMovement, -1);

	StartSubtitleHelp(
		NewTargetSelection(&sSelectInfo_HmuPivot),
		GetStringFromIndex(MSG_HMU_SELECTTARGET_SUBTITLEHELP));

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void action_init(struct ProcHmu *proc)
{
	struct MuProc *mu = GetUnitMu(gActiveUnit);
	if (!mu) {
		HideUnitSprite(gActiveUnit);
		mu = StartMu(gActiveUnit);
	}

	proc->counter = 0;
	proc->mu = mu;

	SetMuFacing(mu, GetFacingDirection(gActiveUnit->xPos, gActiveUnit->yPos, gActionData.xOther, gActionData.yOther));
}

static void action_loop(struct ProcHmu *proc)
{
	Mu_OnStateMovement(proc->mu);

	if (proc->mu->move_clock_q4 == 0) {
		proc->counter++;

		if (proc->counter >= 2)
			Proc_Break(proc);
	}
}

static void action_end(struct ProcHmu *proc)
{
	gActionData.xMove = gActiveUnit->xPos = 2 * gActionData.xOther - gActiveUnit->xPos;
	gActionData.yMove = gActiveUnit->yPos = 2 * gActionData.yOther - gActiveUnit->yPos;
}

STATIC_DECLAR const struct ProcCmd ProcScr_ActionPivot[] = {
	PROC_NAME("ActionPivot"),
	PROC_YIELD,
	PROC_CALL(action_init),
	PROC_YIELD,
	PROC_REPEAT(action_loop),
	PROC_YIELD,
	PROC_CALL(action_end),
	PROC_END
};

static void callback_anim(ProcPtr proc)
{
	Proc_StartBlocking(ProcScr_ActionPivot, proc);
}

bool Action_Pivot(ProcPtr parent)
{
	int sid = gActionData.unk08;

	if (SkillListTester(gActiveUnit, sid))
		NewMuSkillAnimOnActiveUnitWithDeamon(parent, sid, callback_anim, NULL);
	else
		callback_anim(parent);

	return true;
}
