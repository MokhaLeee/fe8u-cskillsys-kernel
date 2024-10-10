#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"

/**
 * 0: generic use
 * 1: battle actor
 * 2: battle target
 */
extern struct SkillList sSkillList[3];
#define SkillListGeneric      (&sSkillList[0])
#define SkillListBattleActor  (&sSkillList[1])
#define SkillListBattleTarget (&sSkillList[2])

/**
 * Fast list
 */
extern u32 sSkillFastList[0x40];
#define SkillFastListActor  (&sSkillFastList[0])
#define SkillFastListTarget (&sSkillFastList[0x20])

void GenerateSkillListExt(struct Unit *unit, struct SkillList *list)
{
	FORCE_DECLARE int weapon;
	int i, sid;
	int pid = UNIT_CHAR_ID(unit);
	int jid = UNIT_CLASS_ID(unit);

	u8 *tmp_list = gGenericBuffer;

	memset(list, 0, sizeof(*list));
	memset(tmp_list, 0, MAX_SKILL_NUM + 1);

	/* person */
	sid = gpConstSkillTable_Person[pid * 2];
	if (COMMON_SKILL_VALID(sid)) {
		tmp_list[sid] = true;
		list->sid[list->amt++] = sid;
	}

	sid = gpConstSkillTable_Person[pid * 2 + 1];
	if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
		tmp_list[sid] = true;
		list->sid[list->amt++] = sid;
	}

	/* job */
	sid = gpConstSkillTable_Job[jid * 2];
	if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
		tmp_list[sid] = true;
		list->sid[list->amt++] = sid;
	}

	sid = gpConstSkillTable_Job[jid * 2 + 1];
	if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
		tmp_list[sid] = true;
		list->sid[list->amt++] = sid;
	}

	/* item */
	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		u8 iid = ITEM_INDEX(unit->items[i]);

		sid = gpConstSkillTable_Item[iid * 2];
		if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
			tmp_list[sid] = true;
			list->sid[list->amt++] = sid;
		}

		sid = gpConstSkillTable_Item[iid * 2 + 1];
		if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
			tmp_list[sid] = true;
			list->sid[list->amt++] = sid;
		}
	}

#if 0
	/* Weapon */
	weapon = ITEM_NONE;
	if (unit == &gBattleActor.unit || unit == &gBattleTarget.unit)
		weapon = ITEM_INDEX(((struct BattleUnit *)unit)->weaponBefore);

	if (weapon != ITEM_NONE) {
		sid = gpConstSkillTable_Weapon[weapon * 2];
		if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
			tmp_list[sid] = true;
			list->sid[list->amt++] = sid;
		}

		sid = gpConstSkillTable_Weapon[weapon * 2 + 1];
		if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
			tmp_list[sid] = true;
			list->sid[list->amt++] = sid;
		}
	}
#endif

	/* generic */
	for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++) {
		sid = UNIT_RAM_SKILLS(unit)[i];
		if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
			tmp_list[sid] = true;
			list->sid[list->amt++] = sid;
		}
	}
	WriteUnitList(unit, &list->header);
}

void ForceUpdateUnitSkillList(struct Unit *unit)
{
	struct SkillList *list = SkillListGeneric;

	if (unit == &gBattleActor.unit)
		list = SkillListBattleActor;
	else if (unit == &gBattleTarget.unit)
		list = SkillListBattleTarget;

	GenerateSkillListExt(unit, list);
}

struct SkillList *GetUnitSkillList(struct Unit *unit)
{
	struct SkillList *list = SkillListGeneric;

	if (unit == &gBattleActor.unit)
		list = SkillListBattleActor;
	else if (unit == &gBattleTarget.unit)
		list = SkillListBattleTarget;

	if (!JudgeUnitList(unit, &list->header))
		GenerateSkillListExt(unit, list);

	return list;
}

bool JudgeSkillViaList(struct Unit *unit, const u16 sid)
{
	int i;
	struct SkillList *list = GetUnitSkillList(unit);

	for (i = 0; i < list->amt; i++)
		if (list->sid[i] == sid)
			return true;

	return false;
}

void SetupBattleSkillList(void)
{
	GenerateSkillListExt(&gBattleActor.unit,  SkillListBattleActor);
	GenerateSkillListExt(&gBattleTarget.unit, SkillListBattleTarget);
}

void DisableUnitSkilLList(struct Unit *unit)
{
	struct SkillList *list = GetUnitSkillList(unit);

	list->amt = 0;
}

void ResetSkillLists(void)
{
	memset(&sSkillList, 0, sizeof(sSkillList));
}

STATIC_DECLAR void SetupBattleSkillFastList(void)
{
	int i;
	struct SkillList *list;
	u32 *fast_list;

	Assert(MAX_SKILL_NUM <= (sizeof(sSkillFastList) * 8 / 2));

	CpuFastFill(0, sSkillFastList, sizeof(sSkillFastList));

	/**
	 * Actor
	 */
	list = GetUnitSkillList(&gBattleActor.unit);
	fast_list = SkillFastListActor;

	for (i = 0; i < list->amt; i++)
		_BIT_SET(fast_list, list->sid[i]);

	/**
	 * Target
	 */
	list = GetUnitSkillList(&gBattleTarget.unit);
	fast_list = SkillFastListTarget;

	for (i = 0; i < list->amt; i++)
		_BIT_SET(fast_list, list->sid[i]);
}

#if 0
bool _JudgeSkillViaFastList(struct BattleUnit *bu, const u16 sid)
{
	u32 *fast_list;

	if (bu == &gBattleActor)
		fast_list = SkillFastListActor;
	else if (bu == &gBattleTarget)
		fast_list = SkillFastListTarget;
	else
		return false;

	return _BIT_CHK(fast_list, sid);
}
#endif

void UnitToBattle_SetupSkillList(struct Unit *unit, struct BattleUnit *bu)
{
	FORCE_DECLARE bool nihil_on_actor, nihil_on_target;

	/**
	 * Here we hold 3 assumption:
	 *
	 * 1. UnitToBattle routine stands at the very beginning of battle-generate
	 * 2. Battle target initialization is behind actor.
	 * 3. No skill activcated before during function: InitBattleUnit()
	 */
	if (bu == &gBattleTarget) {
		SetupBattleSkillList();

#if (defined(SID_Nihil) && COMMON_SKILL_VALID(SID_Nihil))
		nihil_on_actor  = JudgeSkillViaList(&gBattleActor.unit,  SID_Nihil);
		nihil_on_target = JudgeSkillViaList(&gBattleTarget.unit, SID_Nihil);

		if (nihil_on_actor)
			DisableUnitSkilLList(&gBattleActor.unit);

		if (nihil_on_target)
			DisableUnitSkilLList(&gBattleTarget.unit);
#endif

		SetupBattleSkillFastList();
	}
}
