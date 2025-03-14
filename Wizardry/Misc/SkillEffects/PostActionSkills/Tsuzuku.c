#include "common-chax.h"
#include "debuff.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "combat-art.h"
#include "kernel-lib.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

extern u8 gPostActionGaleforceFlag;

bool PostActionTsuzuku(ProcPtr parent)
{
	FORCE_DECLARE struct Unit *unit = gActiveUnit;

	if (!UNIT_IS_VALID(unit) || CheckBitUES(unit, UES_BIT_TSZUKU_SKILL_USED))
		return false;

	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
#if defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce))
		if (SkillListTester(unit, SID_Galeforce) && gBattleActorGlobalFlag.skill_activated_galeforce)
			goto L_exec_rafrain_action_anim;
#endif

#if defined(SID_FailGale) && (COMMON_SKILL_VALID(SID_FailGale))
		if (SkillListTester(unit, SID_FailGale) && !gBattleActor.nonZeroDamage)
			goto L_exec_rafrain_action_anim;
#endif
		if ((GetCombatArtInForce(unit) == CID_Galeforce) && gBattleActorGlobalFlag.enimy_defeated)
			goto L_exec_rafrain_action_anim;

	/* fall through */

	case UNIT_ACTION_STAFF:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicStaff:
#if defined(SID_PowerStaff) && (COMMON_SKILL_VALID(SID_PowerStaff))
		if (CheckActiveUnitSkillActivate(SID_PowerStaff, GetUnitLuck(unit)))
			goto L_exec_rafrain_action_anim;
#endif

	/* fall through */

	default:
#if defined(SID_Tsuzuku) && (COMMON_SKILL_VALID(SID_Tsuzuku))
		if (CheckActiveUnitSkillActivate(SID_Tsuzuku, GetUnitSkill(unit)))
			goto L_exec_rafrain_action_anim;
#endif
		break;

	case UNIT_ACTION_WAIT:
		break;
	}

	return false;

L_exec_rafrain_action_anim:
	if (!UnitAvaliable(unit) || UNIT_STONED(unit))
		return false;

	SetBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
	gActionDataExpa.refrain_action = true;
	EndAllMus();
	StartStatusHealEffect(unit, parent);
	return true;
}
