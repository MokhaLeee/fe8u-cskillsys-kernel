#include "common-chax.h"
#include "skill-system.h"

/**
 * 0: generic use
 * 1: battle actor
 * 2: battle target
 */
extern struct SkillList sSkillList[3];

#define SkillListGeneric (&sSkillList[0])
#define SkillListBattleActor (&sSkillList[1])
#define SkillListBattleTarget (&sSkillList[2])

void GenerateSkillListExt(struct Unit *unit, struct SkillList *list)
{
	int i, weapon, sid;
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

	/* Weapon */
	weapon = ITEM_NONE;

	if (unit == &gBattleActor.unit || unit == &gBattleTarget.unit)
		weapon = ITEM_INDEX(((struct BattleUnit *)unit)->weaponBefore);
	else
		weapon = ITEM_INDEX(GetUnitEquippedWeapon(unit));

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

void UnitToBattle_SetupSkillList(struct Unit *unit, struct BattleUnit *bu)
{
	/* Same as UnitToBattle_ExecNihilSkills() */
	if (bu == &gBattleTarget)
		SetupBattleSkillList();
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
