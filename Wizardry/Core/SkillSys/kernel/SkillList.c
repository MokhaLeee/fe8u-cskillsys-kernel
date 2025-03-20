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

extern void (*gpExternalSkillListGenerator)(struct Unit * unit, struct SkillList * list, u8 * ref);

#ifdef CONFIG_FE8SRR
int NextSeededRN_Simple(u32 rn)
{
    // This generates a pseudorandom string of 32 bits
    u32 rn0 = rn & 0xFFFF;
    u32 rn1 = rn >> 16;
    rn = (rn1 << 11) + (rn0 >> 5) + (rn0 << 16);

    // Shift state[2] one bit
    rn0 *= 2;

    // "carry" the top bit of state[1] to state[0]
    if (rn1 & 0x8000)
        rn0++;

    rn ^= rn0;
    return rn;
}
u32 Mod(int a, int b)
{
    if ((!a) || (!b))
    {
        return 0;
    }
    return DivRem(a, b);
}

u32 InitSeededRN_Simple(int seed, u32 currentRN)
{
    // This table is a collection of 8 possible initial rn state
    // 3 entries will be picked based of which "seed" was given

    u16 initTable[8] = { 0xA36E, 0x924E, 0xB784, 0x4F67, 0x8092, 0x592D, 0x8E70, 0xA794 };

    int mod = Mod(seed, 7);

    currentRN = initTable[(mod++ & 7)];
    currentRN |= initTable[(mod++ & 7)] << 16;

    if (Mod(seed, 5) > 0)
    {
        for (mod = Mod(seed, 5); mod != 0; mod--)
        {
            currentRN = NextSeededRN_Simple(currentRN);
        }
    }

    return currentRN;
}

u32 GetNthRN_Simple(int n, int seed, u32 currentRN)
{
    n = (n ^ (n >> 4)) & 0xF;
    if (!currentRN)
    {
        currentRN = InitSeededRN_Simple(seed, currentRN);
    }
    for (int i = 0; i < n; i++)
    {
        currentRN = NextSeededRN_Simple(currentRN);
    }
    return currentRN;
}
u16 HashByte_Global(int number, int max, int noise[], int offset)
{
    offset += noise[0] + noise[1] + noise[2] + noise[3] + number;
    // offset &= 0xFF; // GetNthRN_Simple does this anyway
    int currentRN = 0;
    currentRN = GetNthRN_Simple(offset, (*(int *)0x203EA34) & 0xFFFFFF, currentRN);
    currentRN = ((*(int *)0x203EA34) & 0xFF) + (currentRN << 6) + (currentRN << 16) - currentRN;
    currentRN = (((*(int *)0x203EA34) & 0xFF00) >> 8) + (currentRN << 6) + (currentRN << 16) - currentRN;
    // currentRN = ((RandValues->seed&0xFF0000) >> 16) + (currentRN << 6) + (currentRN << 16) - currentRN;
    currentRN = ((currentRN << 5) + currentRN) ^ ((*(int *)0x203EA34) & 0xFFFFFF);
    currentRN = ((currentRN << 5) + currentRN) ^ number;
    for (int c = 0; c < 4; ++c)
    {
        for (int i = 0; i < (noise[c] & 0xF); ++i)
        {
            currentRN = NextSeededRN_Simple(currentRN);
        }
    }

    return Mod((currentRN & 0x2FFFFFFF), max);
}

int RandSkill(int id, struct Unit * unit)
{
    if (!id)
    {
        return 0;
    }
    if ((id & 0xFF) == 0xFF)
    {
        return id;
    }
    const struct CharacterData * table = unit->pCharacterData;
    int noise[4] = { table->number, id, id, table->portraitId };
    id = (HashByte_Global(id, 0xFF, noise, 12) + 1);
    if (!gpSkillInfos[id].desc)
    {
        id = 0;
    }
    return id;
}

void GenerateSkillListExt(struct Unit * unit, struct SkillList * list)
{
    FORCE_DECLARE int weapon;
    int i, sid;
    int pid = UNIT_CHAR_ID(unit);
    int jid = UNIT_CLASS_ID(unit);
    int player = UNIT_FACTION(unit) == FACTION_BLUE;
    int class = 0;
    if (player)
    {
        player = pid;
        class = jid;
    }

    u8 * tmp_list = gGenericBuffer;

    memset(list, 0, sizeof(*list));
    memset(tmp_list, 0, MAX_SKILL_NUM + 1);

    /* person */
    sid = RandSkill(gpConstSkillTable_Person[pid * 2], unit);
    if (COMMON_SKILL_VALID(sid))
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    sid = RandSkill(gpConstSkillTable_Person[pid * 2 + 1] + player, unit);
    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    /* job */
    sid = RandSkill(gpConstSkillTable_Job[jid * 2], unit);
    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    sid = RandSkill(gpConstSkillTable_Job[jid * 2 + 1] + class, unit);
    if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
    {
        tmp_list[sid] = true;
        list->sid[list->amt++] = sid;
    }

    /* item */
    for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        u8 iid = ITEM_INDEX(unit->items[i]);

        sid = RandSkill(gpConstSkillTable_Item[iid * 2], unit);
        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
            tmp_list[sid] = true;
            list->sid[list->amt++] = sid;
        }

        sid = RandSkill(gpConstSkillTable_Item[iid * 2 + 1], unit);
        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
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
		sid = RandSkill(gpConstSkillTable_Weapon[weapon * 2], unit);
		if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
			tmp_list[sid] = true;
			list->sid[list->amt++] = sid;
		}

		sid = RandSkill(gpConstSkillTable_Weapon[weapon * 2 + 1], unit);
		if (COMMON_SKILL_VALID(sid) && !tmp_list[sid]) {
			tmp_list[sid] = true;
			list->sid[list->amt++] = sid;
		}
	}
#endif

    /* generic */
    for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
    {
        sid = RandSkill(UNIT_RAM_SKILLS(unit)[i], unit);
        if (COMMON_SKILL_VALID(sid) && !tmp_list[sid])
        {
            tmp_list[sid] = true;
            list->sid[list->amt++] = sid;
        }
    }

    /* external */
    if (gpExternalSkillListGenerator)
        gpExternalSkillListGenerator(unit, list, tmp_list);

    WriteUnitList(unit, &list->header);
}

#endif

#ifndef CONFIG_FE8SRR
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

		/* If we're looking at a weapon, check it's equipped to apply its skill icon */
		if (GetItemMight(unit->items[i]) > 0)
			if(GetUnitEquippedWeapon(unit) != unit->items[i])
				continue;

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
#endif

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

bool _SkillListTester(struct Unit *unit, const u16 sid)
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
bool _BattleSkillTester(struct BattleUnit *bu, const u16 sid)
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
		nihil_on_actor  = _SkillListTester(&gBattleActor.unit,  SID_Nihil);
		nihil_on_target = _SkillListTester(&gBattleTarget.unit, SID_Nihil);

		if (nihil_on_actor)
			DisableUnitSkilLList(&gBattleActor.unit);

		if (nihil_on_target)
			DisableUnitSkilLList(&gBattleTarget.unit);
#endif

		SetupBattleSkillFastList();
	}
}