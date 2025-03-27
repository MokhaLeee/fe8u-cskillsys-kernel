#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "constants/skills.h"
#include "weapon-range.h"
#include "status-getter.h"
#include "gaiden-magic.h"

#define LOCAL_TRACE 0

struct AiSimuSlotEnt {
	u8 slot, action_type;
	u16 item;
};

extern u8 sAiSimuSlotBuf[0x100]; // I think it cannot overflow?
struct AiSimuSlotEnt *const gpAiSimuSlotBuf = (void *)sAiSimuSlotBuf;

void CollectAiSimuSlots(struct Unit *unit, struct AiSimuSlotEnt *buf)
{
	int i;
	struct AiSimuSlotEnt *it = buf;

	CpuFastFill(0, gpAiSimuSlotBuf, sizeof(sAiSimuSlotBuf));

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		u16 item = unit->items[i];

		if (item == 0)
			break;

		if (ITEM_INDEX(item) == ITEM_NIGHTMARE)
			continue;

		if (CanUnitUseWeapon(unit, item)) {
			it->slot = i;
			it->item = item;
			it->action_type = AI_ACTION_COMBAT;

			it++;
		}
	}

	/* Gaiden B.Mag */
	if (gpKernelDesigerConfig->gaiden_magic_en && gpKernelDesigerConfig->gaiden_magic_ai_en) {
		struct GaidenMagicList *gmaglist = GetGaidenMagicList(unit);

		for (i = 0; i < GAIDEN_MAGIC_LIST_LEN; i++) {
			u16 item = gmaglist->bmags[i];

			if (item == 0)
				break;

			if (CanUnitUseGaidenMagic(unit, item)) {
				it->slot = CHAX_BUISLOT_GAIDEN_BMAG1 + i;
				it->item = item;
				it->action_type = AI_ACTION_COMBAT;

				it++;
			}
		}
	}

	/* terminator */
	it->item = 0;
}

void CollectAiSimuStaffSlots(struct Unit *unit, struct AiSimuSlotEnt *buf)
{
	int i;
	struct AiSimuSlotEnt *it = buf;

	CpuFastFill(0, gpAiSimuSlotBuf, sizeof(sAiSimuSlotBuf));

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		u16 item = unit->items[i];

		if (item == 0)
			break;

		if (GetItemAttributes(item) & IA_STAFF) {
			it->slot = i;
			it->item = item;
			it++;
		}
	}

	/* Gaiden w.Mag */
	if (gpKernelDesigerConfig->gaiden_magic_en && gpKernelDesigerConfig->gaiden_magic_ai_en) {
		struct GaidenMagicList *gmaglist = GetGaidenMagicList(unit);

		for (i = 0; i < GAIDEN_MAGIC_LIST_LEN; i++) {
			u16 item = gmaglist->wmags[i];

			if (item == 0)
				break;

			if (!(GetItemAttributes(item) & IA_STAFF))
				continue;

			if (CanUnitUseGaidenMagic(unit, item)) {
				it->slot = CHAX_BUISLOT_GAIDEN_WMAG1 + i;
				it->item = item;
				it++;
			}
		}
	}
}

LYN_REPLACE_CHECK(AiAttemptOffensiveAction);
s8 AiAttemptOffensiveAction(s8 (*isEnemy)(struct Unit *unit))
{
	enum { TARGET_COUNT_TRIGLEVEL = 5, };

	struct AiCombatSimulationSt tmpResult = {0};
	struct AiCombatSimulationSt finalResult = {0};

	struct AiSimuSlotEnt *it, *final_slot = NULL;

#ifdef CONFIG_PERFORMANCE_OPTIMIZATION
	int target_count = 0;
#endif
	int uid;
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

	CollectAiSimuSlots(gActiveUnit, gpAiSimuSlotBuf);

	for (it = gpAiSimuSlotBuf; it->item != 0; it++) {
#ifdef CONFIG_PERFORMANCE_OPTIMIZATION
		int move_distance;
#endif
		u16 item = it->item;

		LTRACEF("[%d] item=0x%04X, slot=0x%02X", it - gpAiSimuSlotBuf, it->item, it->slot);

		tmpResult.itemSlot = it->slot;

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
				finalResult.itemSlot = it->slot;
				final_slot = it;

				LTRACEF("Find slot: %d", it - gpAiSimuSlotBuf);
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
			if (tmpResult.score >= finalResult.score) {
				finalResult = tmpResult;
				final_slot = NULL;
			}
	}

	if ((finalResult.score != 0) || (finalResult.targetId != 0)) {
		int action_type = AI_ACTION_COMBAT;

		if (final_slot)
			action_type = final_slot->action_type;

		LTRACEF("Set decision: x=%d, y=%d, action=%d, slot=%d",
				finalResult.xMove, finalResult.yMove, action_type, finalResult.itemSlot);

		AiSetDecision(finalResult.xMove, finalResult.yMove, action_type, finalResult.targetId, finalResult.itemSlot, 0, 0);

		if ((s8)finalResult.itemSlot != -1)
			TryRemoveUnitFromBallista(gActiveUnit);

		return ret;
	}

	return ret;
}

LYN_REPLACE_CHECK(AiTryDoStaff);
bool AiTryDoStaff(s8 (*isEnemy)(struct Unit *unit))
{
	struct AiSimuSlotEnt *it;
	u8 exp = 0;

	if (GetUnitStatusIndex(gActiveUnit) == UNIT_STATUS_SILENCED)
		return gAiDecision.actionPerformed;

	CollectAiSimuStaffSlots(gActiveUnit, gpAiSimuSlotBuf);

	for (it = gpAiSimuSlotBuf; it->item != 0; it++) {
		int index;
		u16 item = it->item;

		if (GetItemRequiredExp(item) < exp)
			continue;

		index = GetAiStaffFuncIndex(item);
		if (index != -1) {
			sAiStaffFuncLut[index].func(it->slot, isEnemy);

			if (gAiDecision.actionPerformed != false)
				exp = GetItemRequiredExp(item);
		}
	}

	return gAiDecision.actionPerformed;
}
