#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

extern const struct AiCombatScoreCoefficients *sCombatScoreCoefficients;

LYN_REPLACE_CHECK(AiAttemptOffensiveAction);
s8 AiAttemptOffensiveAction(s8 (*isEnemy)(struct Unit *unit))
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

        if (gAiState.flags & AI_FLAG_STAY)
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
            struct Unit *unit = GetUnit(uid);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

#if CHAX

#if defined(SID_Shade) && (COMMON_SKILL_VALID(SID_Shade))
            /* Shade skill may make unit avoid to be target */
            if (SkillTester(unit, SID_Shade))
            {
                if (Roll2RN(GetUnitLuck(unit)))
                    continue;
            }
#endif

#if defined(SID_ShadePlus) && (COMMON_SKILL_VALID(SID_ShadePlus))
            if (SkillTester(unit, SID_ShadePlus))
                continue;
#endif

#endif

#if defined(SID_Rampage) && (COMMON_SKILL_VALID(SID_Rampage))
            if (SkillTester(gActiveUnit, SID_Rampage))
            {
                if (AreUnitsAllied(gActiveUnit->index, unit->index))
                    continue;
                if (gActiveUnit->index == unit->index)
                    continue;
            }
            else
            {
                if (!isEnemy(unit))
                    continue;
            }
#else
            if (!isEnemy(unit))
                continue;
#endif

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

LYN_REPLACE_CHECK(AiGetDamageDealtCombatScoreComponent);
int AiGetDamageDealtCombatScoreComponent(void)
{
    int score = AiGetDamageDealtCombatScoreComponentVanilla();

#if defined(SID_Provoke) && (COMMON_SKILL_VALID(SID_Provoke))
    if (BattleSkillTester(&gBattleTarget, SID_Provoke))
        score += SKILL_EFF0(SID_Provoke);
#endif

    return score;
}

/**
 * Add unit to AI list
 */
LYN_REPLACE_CHECK(CpOrderBerserkInit);
void CpOrderBerserkInit(ProcPtr proc)
{
    int i, aiNum = 0;

    u32 faction = gPlaySt.faction;

    int factionUnitCountLut[3] = {62, 20, 50}; // TODO: named constant for those

    for (i = 0; i < factionUnitCountLut[faction >> 6]; ++i)
    {
        struct Unit *unit = GetUnit(faction + i + 1);

        if (!unit->pCharacterData)
            continue;

        if 
        (
            unit->statusIndex != UNIT_STATUS_BERSERK &&
#if (defined(SID_Rampage) && (COMMON_SKILL_VALID(SID_Rampage)))
                !SkillTester(unit, SID_Rampage)
#endif
        )
            continue;

        if (unit->state & (US_HIDDEN | US_UNSELECTABLE | US_DEAD | US_RESCUED | US_HAS_MOVED_AI))
            continue;

        gAiState.units[aiNum++] = faction + i + 1;
    }

    if (aiNum != 0)
    {
        gAiState.units[aiNum] = 0;
        gAiState.unitIt = gAiState.units;

        AiDecideMainFunc = AiDecideMain;

        Proc_StartBlocking(gProcScr_CpDecide, proc);
    }
}

LYN_REPLACE_CHECK(DecideScriptA);
void DecideScriptA(void)
{
    int i = 0;

    if (UNIT_IS_GORGON_EGG(gActiveUnit))
        return;

    if (gAiState.flags & AI_FLAG_BERSERKED)
    {
        AiDoBerserkAction();
        return;
    }
    else
    {
#if (defined(SID_Rampage) && (COMMON_SKILL_VALID(SID_Rampage)))
        if (SkillTester(gActiveUnit, SID_Rampage))
        {
            AiDoBerserkAction();
            return;
        }
#endif
    }

    for (i = 0; i < 0x100; ++i)
    {
        if (AiTryExecScriptA() == TRUE)
            return;
    }

    AiExecFallbackScriptA();
}