#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"
#include "weapon-range.h"
#include "status-getter.h"

extern void CpDecide_Main(ProcPtr proc);

// Function to find an adjacent ally with higher HP and swap positions with the defender
void SwapDefenderWithAllyIfNecessary(struct Unit* defender) {
    int adjLookup[4][2] = {
        {-1, 0}, // Left
        {1, 0},  // Right
        {0, -1}, // Up
        {0, 1}   // Down
    };

    struct Unit* adjacentAlly = NULL;
    int defenderHP = defender->curHP;
    int x = defender->xPos;
    int y = defender->yPos;

    // Iterate over adjacent positions
    for (int i = 0; i < 4; ++i) {
        int adjX = x + adjLookup[i][0];
        int adjY = y + adjLookup[i][1];

        if (adjX < 0 || adjY < 0 || adjX >= gBmMapSize.x || adjY >= gBmMapSize.y)
            continue;

        struct Unit* unit = GetUnit(gBmMapUnit[adjY][adjX]);
        if (unit && AreUnitsAllied(defender->index, unit->index) && unit->curHP > defenderHP) {
            adjacentAlly = unit;
            defenderHP = unit->curHP;
        }
    }

    // Swap positions if a suitable ally is found
    if (adjacentAlly) {
        int tempX = defender->xPos;
        int tempY = defender->yPos;
        defender->xPos = adjacentAlly->xPos;
        defender->yPos = adjacentAlly->yPos;
        adjacentAlly->xPos = tempX;
        adjacentAlly->yPos = tempY;

        // Update the positions in the map
        gBmMapUnit[defender->yPos][defender->xPos] = defender->index;
        gBmMapUnit[adjacentAlly->yPos][adjacentAlly->xPos] = adjacentAlly->index;

        // Update the AI decision to target the new defender
        gAiDecision.targetId = adjacentAlly->index;
    }
}

LYN_REPLACE_CHECK(CpDecide_Main);
void CpDecide_Main(ProcPtr proc)
{
next_unit:
    gAiState.decideState = 0;

    if (*gAiState.unitIt)
    {
        gAiState.unk7C = 0;

        gActiveUnitId = *gAiState.unitIt;
        gActiveUnit = GetUnit(gActiveUnitId);

        if (gActiveUnit->state & (US_DEAD | US_UNSELECTABLE) || !gActiveUnit->pCharacterData)
        {
            gAiState.unitIt++;
            goto next_unit;
        }

        do
        {
            RefreshEntityBmMaps();
            RenderBmMap();
            RefreshUnitSprites();

            AiUpdateNoMoveFlag(gActiveUnit);

            gAiState.combatWeightTableId = (gActiveUnit->ai_config & AI_UNIT_CONFIG_COMBATWEIGHT_MASK) >> AI_UNIT_CONFIG_COMBATWEIGHT_SHIFT;

            gAiState.dangerMapFilled = FALSE;
            AiInitDangerMap();

            AiClearDecision();
            AiDecideMainFunc();

#if (defined(SID_Guardian) && COMMON_SKILL_VALID(SID_Guardian))
            if (SkillTester(GetUnit(gAiDecision.targetId), SID_Guardian))
            {
                // Assuming gAiDecision.targetId is the defender's unit ID
                struct Unit* defender = GetUnit(gAiDecision.targetId);
                if (defender && AreUnitsAllied(defender->index, gActiveUnit->index) == FALSE) {
                    SwapDefenderWithAllyIfNecessary(defender);
                }
            }
#endif

            gActiveUnit->state |= US_HAS_MOVED_AI;

            if (!gAiDecision.actionPerformed ||
                (gActiveUnit->xPos == gAiDecision.xMove && gActiveUnit->yPos == gAiDecision.yMove && gAiDecision.actionId == AI_ACTION_NONE))
            {
                // Ignoring actions that are just moving to the same square

                gAiState.unitIt++;
                Proc_Goto(proc, 0);
            }
            else
            {
                gAiState.unitIt++;
                Proc_StartBlocking(gProcScr_CpPerform, proc);
            }
        } while (0);
    }
    else
    {
        Proc_End(proc);
    }
}

LYN_REPLACE_CHECK(AiAttemptOffensiveAction);
s8 AiAttemptOffensiveAction(s8 (* isEnemy)(struct Unit * unit))
{
    enum { TARGET_COUNT_TRIGLEVEL = 5, };

    struct AiCombatSimulationSt tmpResult = {0};
    struct AiCombatSimulationSt finalResult = {0};

#ifdef CONFIG_PERFORMANCE_OPTIMIZATION
    int target_count = 0;
#endif
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
#ifdef CONFIG_PERFORMANCE_OPTIMIZATION
        int move_distance;
#endif
        u16 item = gActiveUnit->items[i];

        if (item == 0)
            break;

        if (item == ITEM_NIGHTMARE)
            continue;

        if (!CanUnitUseWeapon(gActiveUnit, item))
            continue;

        tmpResult.itemSlot = i;

#ifdef CONFIG_PERFORMANCE_OPTIMIZATION
        move_distance = MovGetter(gActiveUnit) + GetItemMaxRangeRework(item, gActiveUnit);
#endif

        for (uid = 1; uid < 0xC0; uid++)
        {
            struct Unit * unit = GetUnit(uid);

            if (!UNIT_IS_VALID(unit))
                continue;

            if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;

#ifdef CONFIG_PERFORMANCE_OPTIMIZATION
            if (move_distance < RECT_DISTANCE(unit->xPos, unit->yPos, gActiveUnit->xPos, gActiveUnit->yPos))
                continue;
#else
            if (!AiReachesByBirdsEyeDistance(gActiveUnit, unit, item))
                continue;
#endif

#if defined(SID_ShadePlus) && (COMMON_SKILL_VALID(SID_ShadePlus))
            if (SkillTester(unit, SID_ShadePlus))
                continue;
#endif

#if defined(SID_Shade) && (COMMON_SKILL_VALID(SID_Shade))
            /* Shade skill may make unit avoid to be target */
            if (SkillTester(unit, SID_Shade))
            {
                if (Roll1RN(unit->lck))
                    continue;
            }
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

#ifdef CONFIG_PERFORMANCE_OPTIMIZATION
            if (++target_count >= TARGET_COUNT_TRIGLEVEL)
                break;
#endif
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

/**
 * Add unit to AI list
 */
extern void DecideScriptA(void);
// extern void CpOrderBerserkInit(ProcPtr proc);
// LYN_REPLACE_CHECK(CpOrderBerserkInit);
// void CpOrderBerserkInit(ProcPtr proc)
// {
//     int i, aiNum = 0;

//     u32 faction = gPlaySt.faction;

//     int factionUnitCountLut[3] = {62, 20, 50}; // TODO: named constant for those

//     for (i = 0; i < factionUnitCountLut[faction >> 6]; ++i)
//     {
//         struct Unit *unit = GetUnit(faction + i + 1);

//         if (!unit->pCharacterData)
//             continue;

//         if 
//         (
//             unit->statusIndex != UNIT_STATUS_BERSERK &&
// #if (defined(SID_Rampage) && (COMMON_SKILL_VALID(SID_Rampage)))
//                 !SkillTester(unit, SID_Rampage)
// #else
//                 1
// #endif
//         )
//             continue;

//         if (unit->state & (US_HIDDEN | US_UNSELECTABLE | US_DEAD | US_RESCUED | US_HAS_MOVED_AI))
//             continue;

//         gAiState.units[aiNum++] = faction + i + 1;
//     }

//     if (aiNum != 0)
//     {
//         gAiState.units[aiNum] = 0;
//         gAiState.unitIt = gAiState.units;

//         AiDecideMainFunc = AiDecideMain;

//         Proc_StartBlocking(gProcScr_CpDecide, proc);
//     }
// }

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