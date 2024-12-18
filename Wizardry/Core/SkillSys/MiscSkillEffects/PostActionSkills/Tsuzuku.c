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
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    if (!UNIT_IS_VALID(unit) || CheckBitUES(unit, UES_BIT_TSZUKU_SKILL_USED))
        return false;

#if defined(SID_Cultured) && (COMMON_SKILL_VALID(SID_Cultured))
    bool nice_thighs = false;

    if (SkillTester(unit, SID_Cultured))
        for (int i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
        {
            int _x = unit->xPos + gVecs_1x1[i].x;
            int _y = unit->yPos + gVecs_1x1[i].y;

            unit = GetUnitAtPosition(_x, _y);

#if defined(SID_NiceThighs) && (COMMON_SKILL_VALID(SID_NiceThighs))
            if (SkillTester(unit, SID_NiceThighs))
            {
                nice_thighs = true;
                break;
            }
#endif
        }

    if (nice_thighs)
        goto refresh_turn_once;
#endif

#if defined(SID_AdrenalineRush) && (COMMON_SKILL_VALID(SID_AdrenalineRush))
            if (SkillTester(unit, SID_AdrenalineRush))
            {
                if (unit->curHP <= unit->maxHP / 4)
                    goto refresh_turn_once;
            }
#endif

    switch (gActionData.unitActionType)
    {
    case UNIT_ACTION_COMBAT:
#if defined(SID_Galeforce) && (COMMON_SKILL_VALID(SID_Galeforce))
        if (SkillTester(unit, SID_Galeforce) && gBattleActorGlobalFlag.skill_activated_galeforce)
            goto refresh_turn_once;
#endif

#if defined(SID_FailGale) && (COMMON_SKILL_VALID(SID_FailGale))
        if (SkillTester(unit, SID_FailGale) && !gBattleActor.nonZeroDamage)
            goto refresh_turn_once;
#endif

#if defined(SID_LeadByExample) && (COMMON_SKILL_VALID(SID_LeadByExample))
        if (SkillTester(unit, SID_LeadByExample) && gBattleActorGlobalFlag.skill_activated_lead_by_example)
            goto refresh_turn_once_aura;
#endif

        if ((GetCombatArtInForce(unit) == CID_Galeforce) && gBattleActorGlobalFlag.enimy_defeated)
            goto refresh_turn_once;
            
        [[fallthrough]];

        /* fall through */

    case UNIT_ACTION_STAFF:
#if defined(SID_PowerStaff) && (COMMON_SKILL_VALID(SID_PowerStaff))
        if (SkillTester(unit, SID_PowerStaff) && Roll1RN(LckGetter(unit)))
            goto refresh_turn_once;
#endif
        [[fallthrough]];

    case UNIT_ACTION_USE_ITEM:
#if defined(SID_QuickHands) && (COMMON_SKILL_VALID(SID_QuickHands))
        if (SkillTester(unit, SID_QuickHands))
            goto refresh_turn_repeatedly;
#endif
        [[fallthrough]];

        /* fall through */

    default:
#if defined(SID_Tsuzuku) && (COMMON_SKILL_VALID(SID_Tsuzuku))
        if (SkillTester(unit, SID_Tsuzuku) && Roll1RN(SklGetter(unit)))
            goto refresh_turn_once;
#endif
        break;

    case UNIT_ACTION_WAIT:
        break;
    }

    return false;

refresh_turn_once:
    if (!UNIT_ALIVE(unit) || UNIT_STONED(unit))
        return false;

    SetBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
    gActionDataExpa.refrain_action = true;
    EndAllMus();
    StartStatusHealEffect(unit, parent);
    return true;

/**
 *  This is a stopgap measure to ensure the branch isn't unused when all skills are disabled.
 *  If more skills are added, this will need to be replaced.
 */

#if defined(SID_QuickHands) && (COMMON_SKILL_VALID(SID_QuickHands))
refresh_turn_repeatedly:
    if (!UNIT_ALIVE(unit) || UNIT_STONED(unit))
        return false;

    gActionDataExpa.refrain_action = true;
    EndAllMus();
    StartStatusHealEffect(unit, parent);
    return true;
#endif

/**
 *  This is a stopgap measure to ensure the branch isn't unused when all skills are disabled.
 *  If more skills are added, this will need to be replaced.
 */
#if defined(SID_LeadByExample) && (COMMON_SKILL_VALID(SID_LeadByExample))
refresh_turn_once_aura:
    for (int i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
    {
        int _x = unit->xPos + gVecs_1x1[i].x;
        int _y = unit->yPos + gVecs_1x1[i].y;

        struct Unit *unit_ally = GetUnitAtPosition(_x, _y);

        if (!UNIT_IS_VALID(unit_ally) || UNIT_STONED(unit_ally) || !AreUnitsAllied(unit->index, unit_ally->index))
            continue;

        SetBitUES(unit_ally, UES_BIT_TSZUKU_SKILL_USED);
        unit_ally->state &= ~(US_UNSELECTABLE | US_HAS_MOVED | US_HAS_MOVED_AI);
    }
    SetBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
    return true;
#endif
}
