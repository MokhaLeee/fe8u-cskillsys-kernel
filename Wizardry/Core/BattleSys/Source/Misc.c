#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "strmag.h"
#include "constants/skills.h"

void ClearBattleGlobalFlags(void)
{
	Assert(sizeof(gBattleActorGlobalFlag) <= 0x10);
	Assert(sizeof(gBattleTargetGlobalFlag) <= 0x10);

	CpuFill16(0, &gBattleActorGlobalFlag, sizeof(gBattleActorGlobalFlag));
	CpuFill16(0, &gBattleTargetGlobalFlag, sizeof(gBattleTargetGlobalFlag));
}

void RegisterHitCnt(struct BattleUnit *bu, bool miss)
{
	struct BattleGlobalFlags *flags;

	if (!(gBattleStats.config & BATTLE_CONFIG_REAL))
		return;

	flags = GetBattleGlobalFlags(bu);
	flags->round_cnt++;
	if (!miss) {
		flags->hitted = true;
		flags->round_cnt_hit++;
	} else
		flags->round_cnt_avo++;
}

void ResetRoundEfxSkills(void)
{
	memset(&sEfxSkillQueue, 0, sizeof(sEfxSkillQueue));
}

void EnqueueRoundEfxSkill(u16 sid)
{
	if (sEfxSkillQueue.max < (sizeof(sEfxSkillQueue.skill_pool) - 1))
		sEfxSkillQueue.skill_pool[sEfxSkillQueue.max++] = sid;
}

u16 DequeueRoundEfxSkill(void)
{
	if (sEfxSkillQueue.cur < sEfxSkillQueue.max)
		return sEfxSkillQueue.skill_pool[sEfxSkillQueue.cur++];

	return 0;
}

int UnitToBattle_ExecBonusDoublers(struct Unit *unit, struct BattleUnit *bu)
{
#if defined(SID_BonusDoubler) && (COMMON_SKILL_VALID(SID_BonusDoubler))
	if (SkillTester(unit, SID_BonusDoubler)) {
		int diff;

		diff = bu->unit.pow - unit->pow;
		if (diff > 0) {
			int new_status = bu->unit.pow + diff;

			LIMIT_AREA(new_status, 0, 127);
			bu->unit.pow = new_status;
		}

		diff = UNIT_MAG(&bu->unit) - UNIT_MAG(unit);
		if (diff > 0) {
			int new_status = UNIT_MAG(&bu->unit) + diff;

			LIMIT_AREA(new_status, 0, 127);
			UNIT_MAG(&bu->unit) = new_status;
		}

		diff = bu->unit.skl - unit->skl;
		if (diff > 0) {
			int new_status = bu->unit.skl + diff;

			LIMIT_AREA(new_status, 0, 127);
			bu->unit.skl = new_status;
		}

		diff = bu->unit.spd - unit->spd;
		if (diff > 0) {
			int new_status = bu->unit.spd + diff;

			LIMIT_AREA(new_status, 0, 127);
			bu->unit.spd = new_status;
		}

		diff = bu->unit.lck - unit->lck;
		if (diff > 0) {
			int new_status = bu->unit.lck + diff;

			LIMIT_AREA(new_status, 0, 127);
			bu->unit.lck = new_status;
		}

		diff = bu->unit.def - unit->def;
		if (diff > 0) {
			int new_status = bu->unit.def + diff;

			LIMIT_AREA(new_status, 0, 127);
			bu->unit.def = new_status;
		}

		diff = bu->unit.res - unit->res;
		if (diff > 0) {
			int new_status = bu->unit.res + diff;

			LIMIT_AREA(new_status, 0, 127);
			bu->unit.res = new_status;
		}
	}
#endif

	return 0;
}
