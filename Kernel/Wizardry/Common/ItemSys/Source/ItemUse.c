#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "constants/skills.h"

#define LOCAL_TRACE 0

FORCE_DECLARE static int find_item_slot(struct Unit *unit, int item)
{
	int i;
	int iid = ITEM_INDEX(item);

	for (i = 0; i < UNIT_ITEM_COUNT; i++)
		if (ITEM_INDEX(unit->items[i]) == iid)
			return i;

	return -1;
}

LYN_REPLACE_CHECK(BattleApplyItemEffect);
void BattleApplyItemEffect(struct Proc *proc)
{
	BattleApplyItemExpGains();

	if (gBattleActor.canCounter) {
		if (GetItemAttributes(gBattleActor.weapon) & IA_STAFF) {
			LTRACE("staff weapon broken!");
			gBattleActor.weaponBroke = TRUE;
		}

		gBattleActor.weapon = GetItemAfterUse(gBattleActor.weapon);
		gBattleActor.unit.items[gBattleActor.weaponSlotIndex] = gBattleActor.weapon;

		if (gBattleActor.weapon)
			gBattleActor.weaponBroke = FALSE;

#if (defined(SID_WeaponHeal) && (COMMON_SKILL_VALID(SID_WeaponHeal)))
		if (CheckBattleSkillActivate(&gBattleActor, &gBattleTarget, SID_WeaponHeal, SKILL_EFF0(SID_WeaponHeal))) {
			if (GetItemType(gBattleActor.weaponBefore) == ITYPE_STAFF && AreUnitsAllied(gBattleActor.unit.index, gBattleTarget.unit.index)) {
				int _weapon = gBattleTarget.weaponBefore;

				if (!(GetItemAttributes(_weapon) & IA_UNBREAKABLE) && (ITEM_USES(_weapon) < GetItemMaxUses(_weapon))) {
					int _slot = find_item_slot(&gBattleTarget.unit, _weapon);

					if (_slot > 0) {
						gBattleTarget.unit.items[_slot] = gBattleTarget.weapon = _weapon + 1;
						gBattleTarget.weaponSlotIndex = _slot;
						gBattleTarget.canCounter = true;

						RegisterActorEfxSkill(GetBattleHitRound(gBattleHitIterator), SID_WeaponHeal);
					}
				}
			}
		}
#endif
	}

	(++gBattleHitIterator)->info = BATTLE_HIT_INFO_END;
	Proc_StartBlocking(sProcScr_BattleAnimSimpleLock, proc);
}

/**
 * This is a special hook on: 0x0802FC62,
 * where we may hook on GetItem in function: ActionStaffDoorChestUseItem()
 */
int AutoGetItemUseActionItem(void)
{
	struct Unit *unit = GetUnit(gActionData.subjectIndex);
	int item = GetItemFromSlot(unit, gActionData.itemSlotIndex);

	return GetItemIndex(item);
}
