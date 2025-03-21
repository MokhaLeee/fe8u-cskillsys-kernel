#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "combo-attack.h"
#include "weapon-range.h"
#include "kernel-tutorial.h"

void ResetComboAtkList(void)
{
	u32 i;

	CpuFill16(0, gComboAtkList, sizeof(gComboAtkList));

	for (i = 0; i < ARRAY_COUNT(gComboAtkList); i++)
		gComboAtkList[i].uid = COMBO_ATK_UID_INVALID;
}

STATIC_DECLAR bool ChecComboMagi(int weapon)
{
	if (GetItemAttributes(weapon) & (IA_MAGIC | IA_MAGICDAMAGE))
		return true;

	return true;
}

STATIC_DECLAR bool CheckComboMelee(int weapon)
{
	int max_range;

	if (GetItemAttributes(weapon) & (IA_MAGIC | IA_MAGICDAMAGE))
		return false;

	max_range = GetItemData(ITEM_INDEX(weapon))->encodedRange & 0xF;

	if (max_range >= 2)
		return false;

	return true;
}

/* This is only valid in after battle unit inited */
void BattleGenerateComboAtkList(void)
{
	struct Unit *unit;
	u16 item;
	int range, battle_range;
	bool melee_attack;
	int i, cnt = 0;

	ResetComboAtkList();

	/**
	 * If far-far attack, no combo
	 */
	battle_range = RECT_DISTANCE(
		gBattleActor.unit.xPos,  gBattleActor.unit.yPos,
		gBattleTarget.unit.xPos, gBattleTarget.unit.yPos);

	if (battle_range > 3)
		return;

	if (battle_range <= 1)
		melee_attack = true;
	else
		melee_attack = false;

	for (i = 1; i < 0x100; i++) {
		if (!(cnt < COMBO_ATK_MAX))
			break;

		if ((i & 0xC0) != UNIT_FACTION(&gBattleActor.unit))
			continue;

		/* If not valid unit */
		unit = GetUnit(i);
		if (!UnitOnMapAvaliable(unit) || UNIT_STONED(unit))
			continue;

		if (unit->index == gBattleActor.unit.index)
			continue;

		/* If no weapon */
		item = GetUnitEquippedWeapon(unit);
		if (!item)
			continue;

		/**
		 * Well I decide to directly lock the monster to combo
		 */
		if (GetItemAttributes(item) & IA_LOCK_3)
			continue;

		switch (GetItemType(item)) {
		case ITYPE_SWORD:
		case ITYPE_AXE:
		case ITYPE_LANCE:
		case ITYPE_ANIMA:
		case ITYPE_LIGHT:
		case ITYPE_DARK:
			break;

		default:
			continue;
		}

		if (!ChecComboMagi(item) && CheckComboMelee(item) != melee_attack)
			continue;

		/**
		 * ! check in range
		 */
		range = RECT_DISTANCE(
			unit->xPos, unit->yPos,
			gBattleTarget.unit.xPos, gBattleTarget.unit.yPos);

		if (!IsItemCoveringRangeRework(item, range, unit))
			continue;

		gComboAtkList[cnt].uid = i;
		gComboAtkList[cnt].weapon = ITEM_INDEX(item);
		cnt++;
	}

	if (cnt > 0)
		TriggerKtutorial(KTUTORIAL_COMBO_ATK);
}
