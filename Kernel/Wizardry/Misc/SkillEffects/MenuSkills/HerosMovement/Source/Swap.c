#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "weapon-range.h"
#include "heros-movement.h"

static void try_add_target(struct Unit *unit)
{
	if (UnitOnMapAvaliable(unit) && AreUnitsAllied(gSubjectUnit->index, unit->index)) {
		if (!CanUnitCrossTerrain(unit, gBmMapTerrain[gActiveUnit->yPos][gActiveUnit->xPos]))
			return;

		if (!CanUnitCrossTerrain(gActiveUnit, gBmMapTerrain[unit->yPos][unit->xPos]))
			return;

		AddTarget(unit->xPos, unit->yPos, unit->index, 1);
	}
}

static void make_target_list(struct Unit *unit)
{
	int x = unit->xPos;
	int y = unit->yPos;

	gSubjectUnit = unit;

	BmMapFill(gBmMapRange, 0);
	ForEachAdjacentUnit(x, y, try_add_target);
}

u8 Swap_Usability(const struct MenuItemDef *def, int number)
{
	if (gActiveUnit->state & US_CANTOING)
		return MENU_NOTSHOWN;

	if (!HasSelectTarget(gActiveUnit, make_target_list))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

static u8 select_target_on_switchin(ProcPtr proc, struct SelectTarget* target)
{
	int x, y;

	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);

	x = target->x;
	y = target->y;

	gBmMapRange[y][x] = 1;
	DisplayMoveRangeGraphics(MOVLIMITV_RMAP_BLUE);
	return 0;
}

static u8 select_target_on_select(ProcPtr proc, struct SelectTarget *target)
{
	gActionData.xOther = target->x;
	gActionData.yOther = target->y;
	gActionData.targetIndex = target->uid;

	HideMoveRangeGraphics();

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(BG2_SYNC_BIT);

#if defined(SID_Swap) && (COMMON_SKILL_VALID(SID_Swap))
	gActionData.unk08 = SID_Swap;
#endif

	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

	return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A | TARGETSELECTION_ACTION_CLEARBGS;
}

static u8 select_target_on_cancel(ProcPtr proc, struct SelectTarget* target)
{
	HideMoveRangeGraphics();

	return GenericSelection_BackToUM(proc, target);
}

static const struct SelectInfo select_info = {
	.onInit = NULL,
	.onEnd = NULL,
	.onSwitchIn = select_target_on_switchin,
	.onSelect = select_target_on_select,
	.onCancel = select_target_on_cancel,
	.onHelp = NULL,
};

u8 Swap_OnSelected(struct MenuProc *menu, struct MenuItemProc *item)
{
	if (item->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_HMU_ERROR_TERRAIN);
		return MENU_ACT_SND6B;
	}

	ClearBg0Bg1();

	make_target_list(gActiveUnit);
	BmMapFill(gBmMapMovement, -1);

	StartSubtitleHelp(
		NewTargetSelection(&select_info),
		GetStringFromIndex(MSG_HMU_SELECTTARGET_SUBTITLEHELP));

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void action_init(struct ProcHmu *proc)
{
	struct Unit *unit = GetUnit(gActionData.targetIndex);

	proc->mu1 = GetUnitMu(gActiveUnit);
	if (!proc->mu1) {
		HideUnitSprite(gActiveUnit);
		proc->mu1 = StartMu(gActiveUnit);
	}
	SetMuFacing(proc->mu1, GetFacingDirection(gActiveUnit->xPos, gActiveUnit->yPos, gActionData.xOther, gActionData.yOther));

	proc->mu2 = GetUnitMu(unit);
	if (!proc->mu2) {
		HideUnitSprite(unit);
		proc->mu2 = StartMu(unit);
	}
	SetMuFacing(proc->mu2, GetFacingDirection(gActionData.xOther, gActionData.yOther, gActiveUnit->xPos, gActiveUnit->yPos));

	DisableMuCamera(proc->mu2);
	proc->timer1 = proc->timer2 = 0;
}

static void action_loop(struct ProcHmu *proc)
{
	if (proc->timer1 < 1) {
		Mu_OnStateMovement(proc->mu1);

		if (proc->mu1->move_clock_q4 == 0)
			proc->timer1++;
	}

	if (proc->timer2 < 1) {
		Mu_OnStateMovement(proc->mu2);

		if (proc->mu2->move_clock_q4 == 0)
			proc->timer2++;
	}

	if (proc->timer1 >= 1 && proc->timer2 >= 1)
		Proc_Break(proc);
}

static void action_end(struct ProcHmu *proc)
{
	struct Unit *unit = GetUnit(gActionData.targetIndex);

	unit->xPos = gActiveUnit->xPos;
	unit->yPos = gActiveUnit->yPos;

	gActionData.xMove = gActiveUnit->xPos = gActionData.xOther;
	gActionData.yMove = gActiveUnit->yPos = gActionData.yOther;
}

static const struct ProcCmd proc_scr_action[] = {
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
	Proc_StartBlocking(proc_scr_action, proc);
}

bool Action_Swap(ProcPtr parent)
{
	int sid = gActionData.unk08;

	if (SkillListTester(gActiveUnit, sid))
		NewMuSkillAnimOnActiveUnitWithDeamon(parent, sid, callback_anim, NULL);
	else
		callback_anim(parent);

	return true;
}
