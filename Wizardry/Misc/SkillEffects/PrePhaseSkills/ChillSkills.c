#include "common-chax.h"
#include "kernel-lib.h"
#include "strmag.h"
#include "debuff.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"

#include "pre-phase.h"

#define LOCAL_TRACE 1

struct ProcPrePhaseChill {
	PROC_HEADER;

	int cur;

	/* targets */
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

STATIC_DECLAR void PrePhaseChillAnim_ExecActor(struct Proc *proc)
{
	struct ProcPrePhaseChill *parent = proc->proc_parent;
	struct Unit *unit = GetUnit(gPrePhaseSkillBuf.Chill_ActorUids[parent->cur]);
	int sid = ChillSkillList[parent->cur];

	if (UNIT_ALIVE(unit) && COMMON_SKILL_VALID(sid)) {
		gActiveUnit = unit;
		NewMuSkillAnimOnActiveUnit(sid, NULL, NULL);
	}
}

STATIC_DECLAR void PrePhaseChillAnim_MoveCamerOnTarget(struct Proc *proc)
{
	struct ProcPrePhaseChill *parent = proc->proc_parent;
	struct Unit *target = GetUnit(parent->target_uids_anim[parent->cur]);

	if (UNIT_ALIVE(target))
		EnsureCameraOntoPosition(proc, target->xPos, target->yPos);
}

STATIC_DECLAR void PrePhaseChillAnim_ShowDebuffAnim(struct Proc *proc)
{
	struct ProcPrePhaseChill *parent = proc->proc_parent;
	struct Unit *target = GetUnit(parent->target_uids_anim[parent->cur]);

	if (UNIT_ALIVE(target))
		CallMapAnim_HeavyGravity(proc, target->xPos, target->yPos);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PrePhaseChillAnim[] = {
	PROC_YIELD,
	PROC_CALL(PrePhaseChillAnim_ExecActor),
	PROC_YIELD,
	PROC_CALL(PrePhaseChillAnim_MoveCamerOnTarget),
	PROC_YIELD,
	PROC_CALL(PrePhaseChillAnim_ShowDebuffAnim),
	PROC_YIELD,
	PROC_WHILE(MapAnimHeavyGravityExists),
	PROC_END
};

STATIC_DECLAR void PrePhaseChill_Init(struct ProcPrePhaseChill *proc)
{
	proc->cur = 0;

	memset(proc->target_uids_anim, 0, sizeof(proc->target_uids_anim));
}

STATIC_DECLAR void PrePhaseChill_CollectActors(struct ProcPrePhaseChill *proc)
{
	/**
	 * Already done in common
	 */
}

STATIC_DECLAR void PrePhaseChill_CollectTargets(struct ProcPrePhaseChill *proc)
{
	int i;
	bool seen[0x100] = { false };

	/**
	 * Targets collection has done in SetupChillTargets
	 */

	/* Remove same animed targets */
	for (i = 0; i < UNIT_STATUS_MAX; i++) {
		u8 _uid = gPrePhaseSkillBuf.Chill_TargetUids[i] & 0xFF;

		if (_uid != 0 && seen[_uid] == false) {
			proc->target_uids_anim[i] = _uid;
			seen[_uid] = true;
		}
	}
}

STATIC_DECLAR void PrePhaseChill_ExecAnim(struct ProcPrePhaseChill *proc)
{
	if (gPrePhaseSkillBuf.Chill_ActorUids[proc->cur] != 0 && proc->target_uids_anim[proc->cur] != 0) {
		LTRACEF("[%d] Exec anim: actor=%x, target=%x",
					proc->cur,
					gPrePhaseSkillBuf.Chill_ActorUids[proc->cur] & 0xFF,
					proc->target_uids_anim[proc->cur] & 0xFF);

		Proc_StartBlocking(ProcScr_PrePhaseChillAnim, proc);
	}
}

STATIC_DECLAR void PrePhaseChill_ApplyDebuff(struct ProcPrePhaseChill *proc)
{
	if (gPrePhaseSkillBuf.Chill_TargetUids[proc->cur] != 0) {
		SetUnitStatDebuff(
			GetUnit(gPrePhaseSkillBuf.Chill_TargetUids[proc->cur]),
			ChillDebuffList[proc->cur]
		);
	}
}

STATIC_DECLAR void PrePhaseChill_Advacne(struct ProcPrePhaseChill *proc)
{
	proc->cur++;

	if (proc->cur >= UNIT_STATUS_MAX)
		Proc_Goto(proc, 99);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PrePhaseChill[] = {
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
	LTRACEF("Dump chill actors:  %02X %02X %02X %02X %02X %02X %02X",
			gPrePhaseSkillBuf.Chill_ActorUids[0] & 0xFF,
			gPrePhaseSkillBuf.Chill_ActorUids[1] & 0xFF,
			gPrePhaseSkillBuf.Chill_ActorUids[2] & 0xFF,
			gPrePhaseSkillBuf.Chill_ActorUids[3] & 0xFF,
			gPrePhaseSkillBuf.Chill_ActorUids[4] & 0xFF,
			gPrePhaseSkillBuf.Chill_ActorUids[5] & 0xFF,
			gPrePhaseSkillBuf.Chill_ActorUids[6] & 0xFF);

	Proc_StartBlocking(ProcScr_PrePhaseChill, proc);
	return true;
}

void SetupChillActors(struct Unit *unit)
{
	/**
	 * Exec in common, FOR_UNITS_FACTION:
	 */
	int i;

	for (i = 0; i < UNIT_STATUS_MAX; i++) {
		if (gPrePhaseSkillBuf.Chill_ActorUids[i] == 0) {
			int _sid = ChillSkillList[i];

			if (COMMON_SKILL_VALID(_sid) && SkillTester(unit, _sid))
				gPrePhaseSkillBuf.Chill_ActorUids[i] = unit->index;
		}
	}
}

void SetupChillTargets(struct Unit *unit)
{
	/**
	 * Exec in common, FOR_UNITS_ALL:
	 */
	if (AreUnitsAllied(unit->index, gPlaySt.faction))
		return;

	if (gPrePhaseSkillBuf.Chill_ActorUids[UNIT_STATUS_POW] != 0) {
		// int _status = GetUnitPower(unit);
		int _status = unit->pow;

		if (gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_POW] < _status) {
			gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_POW] = _status;
			gPrePhaseSkillBuf.Chill_TargetUids[UNIT_STATUS_POW] = unit->index;
		}
	}

	if (gPrePhaseSkillBuf.Chill_ActorUids[UNIT_STATUS_MAG] != 0) {
		// int _status = GetUnitMagic(unit);
		int _status = UNIT_MAG(unit);

		if (gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_MAG] < _status) {
			gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_MAG] = _status;
			gPrePhaseSkillBuf.Chill_TargetUids[UNIT_STATUS_MAG] = unit->index;
		}
	}

	if (gPrePhaseSkillBuf.Chill_ActorUids[UNIT_STATUS_SKL] != 0) {
		// int _status = GetUnitSkill(unit);
		int _status = unit->skl;

		if (gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_SKL] < _status) {
			gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_SKL] = _status;
			gPrePhaseSkillBuf.Chill_TargetUids[UNIT_STATUS_SKL] = unit->index;
		}
	}

	if (gPrePhaseSkillBuf.Chill_ActorUids[UNIT_STATUS_SPD] != 0) {
		// int _status = GetUnitSpeed(unit);
		int _status = unit->spd;

		if (gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_SPD] < _status) {
			gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_SPD] = _status;
			gPrePhaseSkillBuf.Chill_TargetUids[UNIT_STATUS_SPD] = unit->index;
		}
	}

	if (gPrePhaseSkillBuf.Chill_ActorUids[UNIT_STATUS_LCK] != 0) {
		// int _status = GetUnitLuck(unit);
		int _status = unit->lck;

		if (gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_LCK] < _status) {
			gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_LCK] = _status;
			gPrePhaseSkillBuf.Chill_TargetUids[UNIT_STATUS_LCK] = unit->index;
		}
	}

	if (gPrePhaseSkillBuf.Chill_ActorUids[UNIT_STATUS_DEF] != 0) {
		// int _status = GetUnitDefense(unit);
		int _status = unit->def;

		if (gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_DEF] < _status) {
			gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_DEF] = _status;
			gPrePhaseSkillBuf.Chill_TargetUids[UNIT_STATUS_DEF] = unit->index;
		}
	}

	if (gPrePhaseSkillBuf.Chill_ActorUids[UNIT_STATUS_RES] != 0) {
		// int _status = GetUnitResistance(unit);
		int _status = unit->res;

		if (gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_RES] < _status) {
			gPrePhaseSkillBuf.Chill_MaxStatusRef[UNIT_STATUS_RES] = _status;
			gPrePhaseSkillBuf.Chill_TargetUids[UNIT_STATUS_RES] = unit->index;
		}
	}
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
