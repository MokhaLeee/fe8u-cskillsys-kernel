#include "common-chax.h"
#include "kernel-lib.h"
#include "strmag.h"
#include "debuff.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

struct ProcPrePhaseChill {
	PROC_HEADER;

	int cur;

	/* targets */
	s8 actor_uids[UNIT_STATUS_MAX];
	s8 target_uids[UNIT_STATUS_MAX];
	s8 target_uids_anim[UNIT_STATUS_MAX];
};

static const int ChillSkillList[UNIT_STATUS_MAX] = {
#if defined(SID_ChillPow) && (COMMON_SKILL_VALID(SID_ChillPow))
	[UNIT_STATUS_POW] = SID_ChillPow,
#endif

#if defined(SID_ChillMag) && (COMMON_SKILL_VALID(SID_ChillMag))
	[UNIT_STATUS_MAG] = SID_ChillMag,
#endif

#if defined(SID_ChillSkl) && (COMMON_SKILL_VALID(SID_ChillSkl))
	[UNIT_STATUS_SKL] = SID_ChillSkl,
#endif

#if defined(SID_ChillSpd) && (COMMON_SKILL_VALID(SID_ChillSpd))
	[UNIT_STATUS_SPD] = SID_ChillSpd,
#endif

#if defined(SID_ChillLck) && (COMMON_SKILL_VALID(SID_ChillLck))
	[UNIT_STATUS_LCK] = SID_ChillLck,
#endif

#if defined(SID_ChillDef) && (COMMON_SKILL_VALID(SID_ChillDef))
	[UNIT_STATUS_DEF] = SID_ChillDef,
#endif

#if defined(SID_ChillRes) && (COMMON_SKILL_VALID(SID_ChillRes))
	[UNIT_STATUS_RES] = SID_ChillRes,
#endif
};

static const u8 ChillDebuffList[UNIT_STATUS_MAX] = {
	[UNIT_STATUS_POW] = UNIT_STAT_DEBUFF_CHILL_POW,
	[UNIT_STATUS_MAG] = UNIT_STAT_DEBUFF_CHILL_MAG,
	[UNIT_STATUS_SKL] = UNIT_STAT_DEBUFF_CHILL_SKL,
	[UNIT_STATUS_SPD] = UNIT_STAT_DEBUFF_CHILL_SPD,
	[UNIT_STATUS_LCK] = UNIT_STAT_DEBUFF_CHILL_LCK,
	[UNIT_STATUS_DEF] = UNIT_STAT_DEBUFF_CHILL_DEF,
	[UNIT_STATUS_RES] = UNIT_STAT_DEBUFF_CHILL_RES,
};

void PrePhaseChillAnim_ExecActor(struct Proc *proc)
{
	struct ProcPrePhaseChill *parent = proc->proc_parent;
	struct Unit *unit = GetUnit(parent->actor_uids[parent->cur]);
	int sid = ChillSkillList[parent->cur];

	if (UnitOnMapAvaliable(unit) && COMMON_SKILL_VALID(sid)) {
		gActiveUnit = unit;
			NewMuSkillAnimOnActiveUnitWithDeamon(proc, sid, NULL, NULL);
	}
}

void PrePhaseChillAnim_MoveCamerOnTarget(struct Proc *proc)
{
	struct ProcPrePhaseChill *parent = proc->proc_parent;
	struct Unit *target = GetUnit(parent->target_uids_anim[parent->cur]);

	if (UnitOnMapAvaliable(target))
		EnsureCameraOntoPosition(proc, target->xPos, target->yPos);
}

void PrePhaseChillAnim_ShowDebuffAnim(struct Proc *proc)
{
	struct ProcPrePhaseChill *parent = proc->proc_parent;
	struct Unit *target = GetUnit(parent->target_uids_anim[parent->cur]);

	if (UnitOnMapAvaliable(target))
		CallMapAnim_HeavyGravity(proc, target->xPos, target->yPos);
}

const struct ProcCmd ProcScr_PrePhaseChillAnim[] = {
	PROC_YIELD,
	PROC_CALL(PrePhaseChillAnim_ExecActor),
	PROC_YIELD,
	PROC_CALL(PrePhaseChillAnim_MoveCamerOnTarget),
	PROC_YIELD,
	PROC_CALL(PrePhaseChillAnim_ShowDebuffAnim),
	PROC_YIELD,
	PROC_WHILE(MapAnimHeavyGravityExists),
	PROC_CALL_ARG(RemoveMuForActiveUnit, 5),
	PROC_YIELD,
	PROC_END
};

void PrePhaseChill_Init(struct ProcPrePhaseChill *proc)
{
	proc->cur = 0;

	memset(proc->actor_uids, 0, sizeof(proc->actor_uids));
	memset(proc->target_uids, 0, sizeof(proc->target_uids));
	memset(proc->target_uids_anim, 0, sizeof(proc->target_uids_anim));
}

void PrePhaseChill_CollectActors(struct ProcPrePhaseChill *proc)
{
	int i;

	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		for (i = 0; i < UNIT_STATUS_MAX; i++) {
			int _sid = ChillSkillList[i];

			if (COMMON_SKILL_VALID(_sid) && SkillTester(unit, _sid))
				proc->actor_uids[i] = unit->index;
		}
	})
}

void PrePhaseChill_CollectTargets(struct ProcPrePhaseChill *proc)
{
	int i, tuid;
	int max_lut[UNIT_STATUS_MAX] = { 0 };
	bool seen[0x100];

	for (tuid = 1; tuid < 0xC0; tuid++) {
		struct Unit *tunit;

		if (AreUnitsAllied(tuid, gPlaySt.faction + 1))
			continue;

		tunit = GetUnit(tuid);
		if (!UnitOnMapAvaliable(tunit))
			continue;

		if (proc->actor_uids[UNIT_STATUS_POW] != 0) {
			// int _status = GetUnitPower(tunit);
			int _status = tunit->pow;

			if (max_lut[UNIT_STATUS_POW] < _status) {
				max_lut[UNIT_STATUS_POW] = _status;
				proc->target_uids[UNIT_STATUS_POW] = tuid;
			}
		}

		if (proc->actor_uids[UNIT_STATUS_MAG] != 0) {
			// int _status = GetUnitMagic(tunit);
			int _status = UNIT_MAG(tunit);

			if (max_lut[UNIT_STATUS_MAG] < _status) {
				max_lut[UNIT_STATUS_MAG] = _status;
				proc->target_uids[UNIT_STATUS_MAG] = tuid;
			}
		}

		if (proc->actor_uids[UNIT_STATUS_SKL] != 0) {
			// int _status = GetUnitSkill(tunit);
			int _status = tunit->skl;

			if (max_lut[UNIT_STATUS_SKL] < _status) {
				max_lut[UNIT_STATUS_SKL] = _status;
				proc->target_uids[UNIT_STATUS_SKL] = tuid;
			}
		}

		if (proc->actor_uids[UNIT_STATUS_SPD] != 0) {
			// int _status = GetUnitSpeed(tunit);
			int _status = tunit->spd;

			if (max_lut[UNIT_STATUS_SPD] < _status) {
				max_lut[UNIT_STATUS_SPD] = _status;
				proc->target_uids[UNIT_STATUS_SPD] = tuid;
			}
		}

		if (proc->actor_uids[UNIT_STATUS_LCK] != 0) {
			// int _status = GetUnitLuck(tunit);
			int _status = tunit->lck;

			if (max_lut[UNIT_STATUS_LCK] < _status) {
				max_lut[UNIT_STATUS_LCK] = _status;
				proc->target_uids[UNIT_STATUS_LCK] = tuid;
			}
		}

		if (proc->actor_uids[UNIT_STATUS_DEF] != 0) {
			// int _status = GetUnitDefense(tunit);
			int _status = tunit->def;

			if (max_lut[UNIT_STATUS_DEF] < _status) {
				max_lut[UNIT_STATUS_DEF] = _status;
				proc->target_uids[UNIT_STATUS_DEF] = tuid;
			}
		}

		if (proc->actor_uids[UNIT_STATUS_RES] != 0) {
			// int _status = GetUnitResistance(tunit);
			int _status = tunit->res;

			if (max_lut[UNIT_STATUS_RES] < _status) {
				max_lut[UNIT_STATUS_RES] = _status;
				proc->target_uids[UNIT_STATUS_RES] = tuid;
			}
		}
	}

	/* Remove same animed targets */
	memset(seen, 0, sizeof(seen));

	for (i = 0; i < UNIT_STATUS_MAX; i++) {
		u8 __tuid = (proc->target_uids[i] & 0xFF);

		if (!seen[__tuid]) {
			seen[__tuid] = true;
			proc->target_uids_anim[i] = __tuid;
		}
	}

	for (i = 0; i < UNIT_STATUS_MAX; i++) {
		LTRACEF("[%d]Dump actor=%x, target=%x, anim=%x",
					i,
					proc->actor_uids[i] & 0xFF,
					proc->target_uids[i] & 0xFF,
					proc->target_uids_anim[i] & 0xFF);
	}
}

void PrePhaseChill_ExecAnim(struct ProcPrePhaseChill *proc)
{
	/**
	 * Try skip anim
	 */
	if (CheckKernelHookSkippingFlag())
		return;

	if (proc->actor_uids[proc->cur] != 0 && proc->target_uids_anim[proc->cur] != 0) {
		LTRACEF("[%d] Exec anim: actor=%x, target=%x",
					proc->cur,
					proc->actor_uids[proc->cur] & 0xFF,
					proc->target_uids_anim[proc->cur] & 0xFF);

		Proc_StartBlocking(ProcScr_PrePhaseChillAnim, proc);
	}
}

void PrePhaseChill_ApplyDebuff(struct ProcPrePhaseChill *proc)
{
	if (proc->target_uids[proc->cur] != 0) {
		SetUnitStatDebuff(
			GetUnit(proc->target_uids[proc->cur]),
			ChillDebuffList[proc->cur]
		);
	}
}

void PrePhaseChill_Advacne(struct ProcPrePhaseChill *proc)
{
	proc->cur++;

	if (proc->cur >= UNIT_STATUS_MAX)
		Proc_Goto(proc, 99);
}

const struct ProcCmd ProcScr_PrePhaseChill[] = {
	PROC_CALL(PrePhaseChill_Init),
	PROC_CALL(PrePhaseChill_CollectActors),
	PROC_CALL(PrePhaseChill_CollectTargets),
	PROC_YIELD,

PROC_LABEL(0),
	PROC_CALL(PrePhaseChill_ExecAnim),
	PROC_YIELD,
	PROC_CALL(PrePhaseChill_ApplyDebuff),
	PROC_CALL(PrePhaseChill_Advacne),
	PROC_GOTO(0),

PROC_LABEL(99),
	PROC_END
};

bool PrePhaseFunc_Chill(ProcPtr proc)
{
	Proc_StartBlocking(ProcScr_PrePhaseChill, proc);
	return true;
}

bool PostActionRemoveChillDebuff(ProcPtr proc)
{
	ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_DEBUFF_CHILL_POW);
	ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_DEBUFF_CHILL_MAG);
	ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_DEBUFF_CHILL_SKL);
	ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_DEBUFF_CHILL_SPD);
	ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_DEBUFF_CHILL_LCK);
	ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_DEBUFF_CHILL_DEF);
	ClearUnitStatDebuff(gActiveUnit, UNIT_STAT_DEBUFF_CHILL_RES);

	return false;
}
