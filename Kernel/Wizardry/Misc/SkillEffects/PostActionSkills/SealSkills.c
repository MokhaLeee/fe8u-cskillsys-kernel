#include "common-chax.h"
#include "debuff.h"
#include "battle-system.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"

/**
 * Actor
 */
static void actor_callback_anim(ProcPtr proc)
{
	CallMapAnim_HeavyGravity(proc, gBattleTarget.unit.xPos, gBattleTarget.unit.yPos);
	BlockProcForMapAnimHeavyGravity(proc);
}

static void actor_callback_exec(ProcPtr proc)
{
	_maybe_unused struct BattleUnit *bu = &gBattleActor;
	_maybe_unused struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

#if defined(SID_SealDefense) && (COMMON_SKILL_VALID(SID_SealDefense))
	if (BattleFastSkillTester(bu, SID_SealDefense)) {
		SetDuraStatusVal(unit_tar, DURA_STATUS_DEF, -SKILL_EFF0(SID_SealDefense));
	}
#endif

#if defined(SID_SealLuck) && (COMMON_SKILL_VALID(SID_SealLuck))
	if (BattleFastSkillTester(bu, SID_SealLuck)) {
		SetDuraStatusVal(unit_tar, DURA_STATUS_LCK, -SKILL_EFF0(SID_SealLuck));
	}
#endif

#if defined(SID_SealMagic) && (COMMON_SKILL_VALID(SID_SealMagic))
	if (BattleFastSkillTester(bu, SID_SealMagic)) {
		SetDuraStatusVal(unit_tar, DURA_STATUS_MAG, -SKILL_EFF0(SID_SealMagic));
	}
#endif

#if defined(SID_SealResistance) && (COMMON_SKILL_VALID(SID_SealResistance))
	if (BattleFastSkillTester(bu, SID_SealResistance)) {
		SetDuraStatusVal(unit_tar, DURA_STATUS_RES, -SKILL_EFF0(SID_SealResistance));
	}
#endif

#if defined(SID_SealSkill) && (COMMON_SKILL_VALID(SID_SealSkill))
	if (BattleFastSkillTester(bu, SID_SealSkill)) {
		SetDuraStatusVal(unit_tar, DURA_STATUS_SKL, -SKILL_EFF0(SID_SealSkill));
	}
#endif

#if defined(SID_SealSpeed) && (COMMON_SKILL_VALID(SID_SealSpeed))
	if (BattleFastSkillTester(bu, SID_SealSpeed)) {
		SetDuraStatusVal(unit_tar, DURA_STATUS_SPD, -SKILL_EFF0(SID_SealSpeed));
	}
#endif

#if defined(SID_SealStrength) && (COMMON_SKILL_VALID(SID_SealStrength))
	if (BattleFastSkillTester(bu, SID_SealStrength)) {
		SetDuraStatusVal(unit_tar, DURA_STATUS_POW, -SKILL_EFF0(SID_SealStrength));
	}
#endif
}

bool PostActionActorSealSkills(ProcPtr parent)
{
	_maybe_unused struct BattleUnit *bu = &gBattleActor;
	struct Unit *unit = gActiveUnit;
	struct Unit *unit_tar;
	_maybe_unused int sid;

	if (gBattleActorGlobalFlag.hitted == false)
		return false;

	if (!UnitAvaliable(unit) || UNIT_STONED(unit))
		return false;

	unit_tar = GetUnit(gActionData.targetIndex);

	if (!UnitAvaliable(unit_tar) || UNIT_STONED(unit_tar))
		return false;

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
		break;

	default:
		return false;
	}

#if defined(SID_SealDefense) && (COMMON_SKILL_VALID(SID_SealDefense))
	if (BattleFastSkillTester(bu, SID_SealDefense)) {
		sid = SID_SealDefense;
		goto exec_seal;
	}
#endif

#if defined(SID_SealLuck) && (COMMON_SKILL_VALID(SID_SealLuck))
	if (BattleFastSkillTester(bu, SID_SealLuck)) {
		sid = SID_SealLuck;
		goto exec_seal;
	}
#endif

#if defined(SID_SealMagic) && (COMMON_SKILL_VALID(SID_SealMagic))
	if (BattleFastSkillTester(bu, SID_SealMagic)) {
		sid = SID_SealMagic;
		goto exec_seal;
	}
#endif

#if defined(SID_SealResistance) && (COMMON_SKILL_VALID(SID_SealResistance))
	if (BattleFastSkillTester(bu, SID_SealResistance)) {
		sid = SID_SealResistance;
		goto exec_seal;
	}
#endif

#if defined(SID_SealSkill) && (COMMON_SKILL_VALID(SID_SealSkill))
	if (BattleFastSkillTester(bu, SID_SealSkill)) {
		sid = SID_SealSkill;
		goto exec_seal;
	}
#endif

#if defined(SID_SealSpeed) && (COMMON_SKILL_VALID(SID_SealSpeed))
	if (BattleFastSkillTester(bu, SID_SealSpeed)) {
		sid = SID_SealSpeed;
		goto exec_seal;
	}
#endif

#if defined(SID_SealStrength) && (COMMON_SKILL_VALID(SID_SealStrength))
	if (BattleFastSkillTester(bu, SID_SealStrength)) {
		sid = SID_SealStrength;
		goto exec_seal;
	}
#endif

	if (0)
		goto exec_seal;

	/* No seal skills activated */
	return false;

exec_seal:
	NewMuSkillAnimOnActiveUnitWithDeamon(parent, sid, actor_callback_anim, actor_callback_exec);
	return true;
}
