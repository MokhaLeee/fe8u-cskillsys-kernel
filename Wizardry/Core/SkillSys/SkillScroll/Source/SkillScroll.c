#include "common-chax.h"
#include "skill-system.h"
#include "icon-rework.h"
#include "kernel-lib.h"
#include "constants/texts.h"

/* External hooks */
char *GetSkillScrollItemName(int item)
{
	return GetSkillNameStr(ITEM_USES(item));
}

int GetSkillScrollItemDescId(int item)
{
	return GetSkillDescMsg(ITEM_USES(item));
}

int GetSkillScrollItemUseDescId(int item)
{
	// return GetSkillDescMsg(ITEM_USES(item));
	return MSG_ITEM_SkillScrollUseDesc;
}

int GetSkillScrollItemIconId(int item)
{
	return SKILL_ICON(ITEM_USES(item));
}

/* Item use */
STATIC_DECLAR void call_remove_skill_menu(void)
{
	StartSubtitleHelp(
		StartOrphanMenu(&RemoveSkillMenuDef),
		GetStringFromIndex(MSG_RemoveSkillSubtitle)
	);
}

STATIC_DECLAR const struct ProcCmd ProcScr_SkillScrollUseSoftLock[] = {
	PROC_YIELD,
	PROC_CALL(call_remove_skill_menu),
	PROC_END
};

void ItemUseEffect_SkillScroll(struct Unit *unit)
{
	gActionData.unk08 = -1;
	if (gpKernelDesigerConfig->gen_new_scroll == false) {
		/**
		 * If the unit has been filled with equipable skills,
		 * player need to select to remove a equipped skill.
		 **/
		if (GetFreeSkillSlot(unit) == -1)
			Proc_StartBlocking(ProcScr_SkillScrollUseSoftLock, Proc_Find(gProcScr_PlayerPhase));
	}
}

void ItemUseAction_SkillScroll(ProcPtr proc)
{
	struct Unit *unit = GetUnit(gActionData.subjectIndex);
	int slot = gActionData.itemSlotIndex;
	int item = unit->items[slot];

	if (gActionData.unk08 != (u16)-1) {
		/* Replace skill */
		int slot_rep = gActionData.unk08;
		int sid_rep = UNIT_RAM_SKILLS(unit)[slot_rep];

		unit->items[slot] = ITEM_INDEX(item) | (sid_rep << 8);
		RemoveSkill(unit, sid_rep);
		AddSkill(unit, ITEM_USES(item));
	} else {
		/* Simply add a new skill */
		AddSkill(unit, ITEM_USES(item));
		UnitUpdateUsedItem(unit, slot);
	}

	NewPopup_VerySimple(MSG_SkillLearned, 0x5A, proc);
}

bool ItemUsbility_SkillScroll(struct Unit *unit, int item)
{
	return !IsSkillLearned(unit, ITEM_USES(item));
}

/* Prep item use */
STATIC_DECLAR void PrepItemUseScroll_OnDraw(struct ProcPrepItemUseJunaFruit *proc, int item, int x, int y)
{
	int skill = ITEM_USES(item);
	const char *str = GetStringFromIndex(MSG_SkillLearned);
	struct Text *text = &gPrepItemTexts[TEXT_PREPITEM_POPUP];
	int icon = SKILL_ICON(skill);
	int width = GetStringTextLen(str);

	DrawIcon(TILEMAP_LOCATED(gBG2TilemapBuffer, x, y), icon, 0x4000);

	ClearText(text);
	PutDrawText(
		text,
		TILEMAP_LOCATED(gBG2TilemapBuffer, x + 2, y),
		TEXT_COLOR_SYSTEM_WHITE,
		0, 0, str
	);

	BG_EnableSyncByMask(BG2_SYNC_BIT);

	proc->xpos = x * 8 - 4;
	proc->ypos = y * 8 - 4;
	proc->width = width / 8 + 3;
	proc->height = 2;
}

STATIC_DECLAR void PrepItemUseScroll_OnInit(struct ProcPrepItemUseJunaFruit *proc)
{
	struct ProcPrepItemUse *parent = proc->proc_parent;

	DrawPrepScreenItemUseStatBars(parent->unit, 0);
	DrawPrepScreenItemUseStatValues(parent->unit);

	PrepItemUseScroll_OnDraw(proc, parent->unit->items[parent->slot], 0x11, 0x0E);

	proc->timer = 0x78;
	PlaySoundEffect(0x5A);
}

STATIC_DECLAR void PrepItemUseScroll_OnEnd(struct ProcPrepItemUseJunaFruit *proc)
{
	struct ProcPrepItemUse *parent = proc->proc_parent;

	AddSkill(parent->unit, ITEM_USES(parent->unit->items[parent->slot]));
	UnitUpdateUsedItem(parent->unit, parent->slot);
	PrepItemUseJuna_OnEnd(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PrepItemUseScroll[] = {
	PROC_SET_END_CB(PrepItemUseScroll_OnEnd),
	PROC_CALL(PrepItemUseScroll_OnInit),
	PROC_REPEAT(PrepItemUseJuna_IDLE),
	PROC_END
};

void PrepItemEffect_SkillScroll(struct ProcPrepItemUse *proc, u16 item)
{
	Proc_StartBlocking(ProcScr_PrepItemUseScroll, proc);
}

bool PrepItemUsbility_SkillScroll(struct Unit *unit, int item)
{
	if (gpKernelDesigerConfig->gen_new_scroll == false) {
		/**
		 * If skillsys is configured unequipable,
		 * then we need to find a free-slot to equip the skill.
		 */
		if (GetFreeSkillSlot(unit) == -1)
			return false;

		return true;
	}

	/**
	 * If player can equip skill by themself,
	 * then they just need to avoid from learned skill.
	 */
	return !IsSkillLearned(unit, ITEM_USES(item));
}
