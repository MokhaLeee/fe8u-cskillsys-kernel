#include "global.h"
#include "cp_common.h"
#include "bmunit.h"
#include "bmitem.h"
#include "bmbattle.h"
#include "bmarch.h"
#include "bmidoten.h"
#include "cp_utility.h"
#include "cp_common.h"
#include "cp_data.h"
#include "rng.h"
#include "constants/items.h"

#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

extern const struct AiCombatScoreCoefficients * sCombatScoreCoefficients;

/* LynJump */
s8 AiAttemptOffensiveAction(s8 (* isEnemy)(struct Unit * unit))
{
    struct AiCombatSimulationSt tmpResult = {0};
    struct AiCombatSimulationSt finalResult = {0};

    int i, uid;
    bool ret = 0;

    if (gActiveUnit->state & US_IN_BALLISTA)
    {
        BmMapFill(gBmMapMovement, -1);
        gBmMapMovement[gActiveUnit->yPos][gActiveUnit->xPos] = 0;

        if (GetRiddenBallistaAt(gActiveUnit->xPos, gActiveUnit->yPos) != 0)
            goto try_ballist_combat;

        TryRemoveUnitFromBallista(gActiveUnit);
    }
    else
    {
        if (UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL)
        {
            if (GetUnitItemCount(gActiveUnit) < UNIT_ITEM_COUNT)
            {
                GenerateUnitMovementMap(gActiveUnit);
                MarkMovementMapEdges();

                if (AiAttemptStealActionWithinMovement() == 1)
                    return 0;
            }
        }

        if (gAiState.flags & AI_FLAG_1)
        {
            BmMapFill(gBmMapMovement, -1);
            gBmMapMovement[gActiveUnit->yPos][gActiveUnit->xPos] = 0;
        }
        else
            GenerateUnitMovementMap(gActiveUnit);

        if (UnitHasMagicRank(gActiveUnit))
            GenerateMagicSealMap(-1);
    }

    SetWorkingBmMap(gBmMapRange);

    for (i = 0; i < UNIT_ITEM_COUNT; i++)
    {
        u16 item = gActiveUnit->items[i];

        if (item == 0)
            break;

        if (item == ITEM_NIGHTMARE)
            continue;

        if (!CanUnitUseWeapon(gActiveUnit, item))
            continue;

        tmpResult.itemSlot = i;

        for (uid = 1; uid < 0xC0; uid++)
        {
            struct Unit* unit = GetUnit(uid);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

#if CHAX
            /* Shade skill may make unit avoid to be target */
            if (SkillTester(unit, SID_Shade))
            {
                if (Roll2RN(GetUnitLuck(unit)))
                    continue;
            }

            if (SkillTester(unit, SID_ShadePlus))
                continue;
#endif

            if (!isEnemy(unit))
                continue;

            if (!AiReachesByBirdsEyeDistance(gActiveUnit, unit, item))
                continue;

            AiFillReversedAttackRangeMap(unit, item);

            tmpResult.targetId = unit->index;

            ret = AiSimulateBestBattleAgainstTarget(&tmpResult);
            if (!ret)
                continue;

            if (tmpResult.score >= finalResult.score)
            {
                finalResult = tmpResult;
                finalResult.itemSlot = i;
            }
        }
    }

try_ballist_combat:
    if (UNIT_CATTRIBUTES(gActiveUnit) & CA_BALLISTAE)
    {
        ret = AiAttemptBallistaCombat(isEnemy, &tmpResult);
        if (ret == 1)
            if (tmpResult.score >= finalResult.score)
                finalResult = tmpResult;
    }

    if ((finalResult.score != 0) || (finalResult.targetId != 0))
    {
        AiSetDecision(finalResult.xMove, finalResult.yMove, 1, finalResult.targetId, finalResult.itemSlot, 0, 0);

        if ((s8)finalResult.itemSlot != -1)
            TryRemoveUnitFromBallista(gActiveUnit);
    }

    return ret;
}

STATIC_DECLAR int AiGetDamageDealtCombatScoreComponentVanilla(void)
{
    int score;

    if (gBattleTarget.unit.curHP == 0)
        return 50;

    score = (gBattleActor.battleAttack - gBattleTarget.battleDefense) * gBattleActor.battleEffectiveHitRate;

    if (score < 0)
        score = 0;

    score = Div(score, 100);
    score = sCombatScoreCoefficients->coeffDamageDealt * score;

    if (score > 40)
        score = 40;

    return score;
}

/* LynJump */
int AiGetDamageDealtCombatScoreComponent(void)
{
    int score = AiGetDamageDealtCombatScoreComponentVanilla();

    if (SkillTester(&gBattleTarget.unit, SID_Provoke))
        score += 50;

    return score;
}
