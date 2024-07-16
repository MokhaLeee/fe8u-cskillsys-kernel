#include "common-chax.h"
#include "debuff.h"
#include "action-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

extern u8 gPostActionGaleforceFlag;

bool PostActionGaleForce(ProcPtr parent)
{
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    gActionDataExpa.refrain_action = false;

    if (!UNIT_IS_VALID(unit))
        return false;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
#if defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce))
        if (SkillTester(unit, SID_Galeforce) && gBattleActorGlobalFlag.skill_activated_galeforce)
            goto L_exec_rafrain_action_anim;
#endif

#if defined(SID_FailGale) && (COMMON_SKILL_VALID(SID_FailGale))
        if (SkillTester(unit, SID_FailGale))
        {
            struct Unit * unit_tar = GetUnit(gActionData.targetIndex);
            struct BattleUnit * battle_target = &gBattleTarget;

            if (unit_tar->curHP == battle_target->hpInitial)
                goto L_exec_rafrain_action_anim;
        }
#endif

    /* fall through */

    case UNIT_ACTION_STAFF:
#if defined(SID_PowerStaff) && (COMMON_SKILL_VALID(SID_PowerStaff))
        if (SkillTester(unit, SID_PowerStaff) && Roll1RN(unit->lck))
            goto L_exec_rafrain_action_anim;
#endif

    /* fall through */

    default:
#if defined(SID_Tsuzuku) && (COMMON_SKILL_VALID(SID_Tsuzuku))
        if (SkillTester(unit, SID_Tsuzuku))
            goto L_exec_rafrain_action_anim;
#endif
        break;

    case UNIT_ACTION_WAIT:
        break;
    }

    return false;

L_exec_rafrain_action_anim:
    if (!UNIT_ALIVE(unit) || UNIT_STONED(unit))
        return false;

    gActionDataExpa.refrain_action = true;
    EndAllMus();
    StartStatusHealEffect(unit, parent);
    return true;
}
