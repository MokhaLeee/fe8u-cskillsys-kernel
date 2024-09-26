#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "weapon-range.h"
#include "status-getter.h"

LYN_REPLACE_CHECK(AiAttemptOffensiveAction);
s8 AiAttemptOffensiveAction(s8 (*isEnemy)(struct Unit *unit))
{
	enum { TARGET_COUNT_TRIGLEVEL = 5, };

	struct AiCombatSimulationSt tmpResult = {0};
	struct AiCombatSimulationSt finalResult = {0};

#ifdef CONFIG_PERFORMANCE_OPTIMIZATION
	int target_count = 0;
#endif
	int i, uid;
	bool ret = 0;

	if (gActiveUnit->state & US_IN_BALLISTA) {
		BmMapFill(gBmMapMovement, -1);
		gBmMapMovement[gActiveUnit->yPos][gActiveUnit->xPos] = 0;

		if (GetRiddenBallistaAt(gActiveUnit->xPos, gActiveUnit->yPos) != 0)
			goto try_ballist_combat;

		TryRemoveUnitFromBallista(gActiveUnit);
	} else {
		if (UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL) {
			if (GetUnitItemCount(gActiveUnit) < UNIT_ITEM_COUNT) {
				GenerateUnitMovementMap(gActiveUnit);
				MarkMovementMapEdges();

				if (AiAttemptStealActionWithinMovement() == 1)
					return 0;
			}
		}

		if (gAiState.flags & AI_FLAG_STAY) {
			BmMapFill(gBmMapMovement, -1);
			gBmMapMovement[gActiveUnit->yPos][gActiveUnit->xPos] = 0;
		} else
			GenerateUnitMovementMap(gActiveUnit);

		if (UnitHasMagicRank(gActiveUnit))
			GenerateMagicSealMap(-1);
	}

	SetWorkingBmMap(gBmMapRange);

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
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

		for (uid = 1; uid < 0xC0; uid++) {
			struct Unit *unit = GetUnit(uid);

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
				if (Roll1RN(unit->lck))
					continue;
#endif

			if (!isEnemy(unit))
				continue;

			AiFillReversedAttackRangeMap(unit, item);

			tmpResult.targetId = unit->index;

			ret = AiSimulateBestBattleAgainstTarget(&tmpResult);
			if (!ret)
				continue;

			if (tmpResult.score >= finalResult.score) {
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
	if (UNIT_CATTRIBUTES(gActiveUnit) & CA_BALLISTAE) {
		ret = AiAttemptBallistaCombat(isEnemy, &tmpResult);
		if (ret == 1)
			if (tmpResult.score >= finalResult.score)
				finalResult = tmpResult;
	}

	if ((finalResult.score != 0) || (finalResult.targetId != 0)) {
		AiSetDecision(finalResult.xMove, finalResult.yMove, 1, finalResult.targetId, finalResult.itemSlot, 0, 0);

		if ((s8)finalResult.itemSlot != -1)
			TryRemoveUnitFromBallista(gActiveUnit);
	}

	return ret;
}
