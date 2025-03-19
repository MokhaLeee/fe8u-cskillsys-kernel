#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "shield.h"
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

extern void (*gpExternalSkillListGenerator)(struct Unit *unit, struct SkillList *list, u8 *ref);

void GenerateSkillListExt(struct Unit *unit, struct SkillList *list)
{
	#define ADD_LIST(skill_index) \
	do { \
		int __tmp_sid = skill_index; \
		if (COMMON_SKILL_VALID(__tmp_sid) && !tmp_list[__tmp_sid]) { \
			tmp_list[__tmp_sid] = true; \
			list->sid[list->amt++] = __tmp_sid; \
		} \
	} while (0)

	int i, weapon;
	const struct ShieldInfo *shield;
	int pid = UNIT_CHAR_ID(unit);
	int jid = UNIT_CLASS_ID(unit);
	u8 *tmp_list = gGenericBuffer;

	memset(list, 0, sizeof(*list));

	if (!UNIT_IS_VALID(unit)) {
		WriteUnitList(unit, &list->header);
		return;
	}

	WARN_GENERIC_BUF_USED;
	memset(tmp_list, 0, MAX_SKILL_NUM + 1);

	/* person */
	ADD_LIST(gpConstSkillTable_Person[pid * 2 + 0]);
	ADD_LIST(gpConstSkillTable_Person[pid * 2 + 1]);

	/* job */
	ADD_LIST(gpConstSkillTable_Job[jid * 2 + 0]);
	ADD_LIST(gpConstSkillTable_Job[jid * 2 + 1]);

	/* item */
	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		u8 iid = ITEM_INDEX(unit->items[i]);

		if (iid == ITEM_NONE)
			break;

		ADD_LIST(gpConstSkillTable_Item[iid * 2 + 0]);
		ADD_LIST(gpConstSkillTable_Item[iid * 2 + 1]);
	}

	/* weapon & sheild*/
	if (unit == &gBattleActor.unit || unit == &gBattleTarget.unit) {
		struct BattleUnit *bu = (struct BattleUnit *)unit;

		weapon = ITEM_INDEX(bu->weapon);
		shield = GetBattleUnitShield(bu);
	} else {
		weapon = ITEM_INDEX(GetUnitEquippedWeapon(unit));
		shield = GetUnitShield(unit);
	}

	if (weapon != ITEM_NONE) {
		ADD_LIST(gpConstSkillTable_Weapon[weapon * 2 + 0]);
		ADD_LIST(gpConstSkillTable_Weapon[weapon * 2 + 1]);
	}

	if (shield) {
		ADD_LIST(shield->skills[0]);
		ADD_LIST(shield->skills[1]);
	}

	/* generic */
	for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
		ADD_LIST(UNIT_RAM_SKILLS(unit)[i]);

	/* external */
	if (gpExternalSkillListGenerator)
		gpExternalSkillListGenerator(unit, list, tmp_list);

	WARN_GENERIC_BUF_RELEASED;
	WriteUnitList(unit, &list->header);

	#undef ADD_LIST
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

	if (!JudgeUnitList(unit, &list->header)) {
		Errorf("Ops! regenerate skilllist: uid=%02X, pid=%02X", unit->index & 0xFF, UNIT_CHAR_ID(unit));

		GenerateSkillListExt(unit, list);
	}

	return list;
}

bool SkillListTester(struct Unit *unit, const u16 sid)
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
bool _BattleFastSkillTester(struct BattleUnit *bu, const u16 sid)
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

void AppendBattleUnitSkillList(struct BattleUnit *bu, u16 skill)
{
	u32 i;
	struct SkillList *list;
	u32 *fast_list;

	list = GetUnitSkillList(&bu->unit);

	/**
	 * Full filled
	 */
	if (list->amt >= ARRAY_COUNT(list->sid))
		return;

	/**
	 * Already in force
	 */
	for (i = 0; i < list->amt; i++) {
		if (skill == list->sid[i])
			return;
	}

	/**
	 * Add to list
	 */
	list->sid[list->amt++] = skill;


	if (bu == &gBattleActor)
		fast_list = SkillFastListActor;
	else if (bu == &gBattleTarget)
		fast_list = SkillFastListTarget;
	else
		return;

	/**
	 * Add to fast-list
	 */
	_BIT_SET(fast_list, skill);
}

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
		nihil_on_actor  = SkillListTester(&gBattleTarget.unit,  SID_Nihil);
		nihil_on_target = SkillListTester(&gBattleActor.unit, SID_Nihil);

		if (nihil_on_actor)
			DisableUnitSkilLList(&gBattleActor.unit);

		if (nihil_on_target)
			DisableUnitSkilLList(&gBattleTarget.unit);
#endif

		SetupBattleSkillFastList();
	}
}
