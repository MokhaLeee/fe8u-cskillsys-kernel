#include "common-chax.h"
#include "debuff.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "combat-art.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

extern u8 gPostActionGaleforceFlag;

bool PostActionTsuzuku(ProcPtr parent)
{
    FORCE_DECLARE struct Unit *unit = gActiveUnit;

    if (!UNIT_IS_VALID(unit) || CheckBitUES(unit, UES_BIT_TSZUKU_SKILL_USED))
        return false;

    switch (gActionData.unitActionType)
    {
    case UNIT_ACTION_COMBAT:
#if defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce))
        if (SkillTester(unit, SID_Galeforce) && gBattleActorGlobalFlag.skill_activated_galeforce)
            goto L_exec_rafrain_action_anim;
#endif

#if defined(SID_FailGale) && (COMMON_SKILL_VALID(SID_FailGale))
        if (SkillTester(unit, SID_FailGale) && !gBattleActor.nonZeroDamage)
            goto L_exec_rafrain_action_anim;
#endif

#if defined(SID_LeadByExample) && (COMMON_SKILL_VALID(SID_LeadByExample))
        if (SkillTester(unit, SID_LeadByExample) && gBattleActorGlobalFlag.skill_activated_lead_by_example)
            goto L_exec_rafrain_action_anim_aura;
#endif

        if ((GetCombatArtInForce(unit) == CID_Galeforce) && gBattleActorGlobalFlag.enimy_defeated)
            goto L_exec_rafrain_action_anim;

        /* fall through */

    case UNIT_ACTION_STAFF:
#if defined(SID_PowerStaff) && (COMMON_SKILL_VALID(SID_PowerStaff))
        if (SkillTester(unit, SID_PowerStaff) && Roll1RN(LckGetter(unit)))
            goto L_exec_rafrain_action_anim;
#endif

        /* fall through */

    default:
#if defined(SID_Tsuzuku) && (COMMON_SKILL_VALID(SID_Tsuzuku))
        if (SkillTester(unit, SID_Tsuzuku) && Roll1RN(SklGetter(unit)))
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

    SetBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
    gActionDataExpa.refrain_action = true;
    EndAllMus();
    StartStatusHealEffect(unit, parent);
    return true;

L_exec_rafrain_action_anim_aura:
    for (int i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
    {
        int _x = unit->xPos + gVecs_1x1[i].x;
        int _y = unit->yPos + gVecs_1x1[i].y;

        struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

        if (!UNIT_IS_VALID(unit_ally) || UNIT_STONED(unit_ally) || !AreUnitsAllied(unit->index, unit_ally->index))
            continue;

        SetBitUES(unit_ally, UES_BIT_TSZUKU_SKILL_USED);
        unit_ally->state &= ~( US_UNSELECTABLE | US_HAS_MOVED | US_HAS_MOVED_AI );
    }
    SetBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
    return true;
}
