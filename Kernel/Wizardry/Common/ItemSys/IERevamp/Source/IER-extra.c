#include "common-chax.h"
#include "item-sys.h"
#include "status-getter.h"

#define LOCAL_TRACE 0

STATIC_DECLAR bool IER_CheckIList(int iid, const u8 *list)
{
	int i;

	for (i = 0; list[i] != ITEM_NONE; i++)
		if (iid == list[i])
			return true;

	return false;
}

/**
 * Desc
 */
// LYN__REPLACE_CHECK(GetItemCantUseMsgid);

/**
 * Promotion list
 */
LYN_REPLACE_CHECK(CanUnitUsePromotionItem);
bool CanUnitUsePromotionItem(struct Unit *unit, int item)
{
	int iid = ITEM_INDEX(item);
	const struct IER_PromoConfig *it = *pr_gpIER_PromotionItemTable;

	for (; it->item != ITEM_NONE && it->job_list != NULL; it++) {
		LTRACEF("item=0x%02X 0x%02X, job=0x%02X", it->item, iid, it->job_list[0]);

		if (it->item == iid) {
			int i;
			int jid = UNIT_CLASS_ID(unit);

			for (i = 0; ; i++) {
				int it_jid = it->job_list[i];

				if (it_jid == CLASS_NONE)
					return false;

				if (jid == it_jid)
					return true;
			}
			return false;
		}
	}
	return false;
}

// PlayerPhase_PrepareAction

/**
 * Heal
 */
LYN_REPLACE_CHECK(GetUnitItemHealAmount);
int GetUnitItemHealAmount(struct Unit *unit, int item)
{
	int result = 0;

#ifdef CONFIG_IER_EN
	result = GetItemMight(item) + IER_BYTE(item);

	if (result < 10)
		result = 10;
#else
	switch (GetItemIndex(item)) {
	case ITEM_STAFF_HEAL:
	case ITEM_STAFF_PHYSIC:
	case ITEM_STAFF_FORTIFY:
	case ITEM_VULNERARY:
	case ITEM_VULNERARY_2:
		result = 10;
		break;

	case ITEM_STAFF_MEND:
		result = 20;
		break;

	case ITEM_STAFF_RECOVER:
	case ITEM_ELIXIR:
		result = 80;
		break;
	} // switch (GetItemIndex(item))
#endif // IER_EN

	if (GetItemAttributes(item) & IA_STAFF)
		result += MagGetter(unit);

	if (result > 80)
		result = 80;

	return result;
}

/**
 * Key
 */
LYN_REPLACE_CHECK(AiGetChestUnlockItemSlot);
bool AiGetChestUnlockItemSlot(u8 *out)
{
	int i;

	*out = 0;

	if (GetUnitItemCount(gActiveUnit) == UNIT_ITEM_COUNT) {
		gActiveUnit->aiFlags |= AI_UNIT_FLAG_3;
		return false;
	}

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		u16 item = gActiveUnit->items[i];

		if (item == 0)
			return false;

		*out = i;

#if CHAX
		switch (GetItemData(ITEM_INDEX(item))->useEffectId) {
		case IER_LOCKPICK:
			if (UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL)
				return true;

			break;

		case IER_CHESTKEY:
		case IER_DOORKEY:
			return true;

		default:
			break;
		}
#else
		if (GetItemIndex(item) == ITEM_CHESTKEY)
			return true;

		if (GetItemIndex(item) == ITEM_LOCKPICK) {
			if (UNIT_CATTRIBUTES(gActiveUnit) & CA_STEAL)
				return true;
		}
#endif
	}

	return false;
}

LYN_REPLACE_CHECK(AiTryHealSelf);
bool AiTryHealSelf(void)
{
	int i;

	for (i = 0; i < UNIT_ITEM_COUNT; i++) {
		u16 item = gActiveUnit->items[i];

		if (item == 0)
			return 0;

		switch (GetItemData(ITEM_INDEX(item))->useEffectId) {
		case IER_VULNERARY:
		case IER_ELIXIR:
			if (!(gAiState.flags & AI_FLAG_STAY) && !(gActiveUnit->ai_config & AI_UNIT_CONFIG_FLAG_STAY)) {
				/**
				 * If unit can move around (rather than stick on position)
				 * he may try escape to a safe place then heal itself.
				 */
				struct Vec2 position;

				if (AiFindSafestReachableLocation(gActiveUnit, &position) == true) {
					AiSetDecision(position.x, position.y, AI_ACTION_USEITEM, 0, i, 0, 0);
					return true;
				}
			} else {
				AiSetDecision(gActiveUnit->xPos, gActiveUnit->yPos, AI_ACTION_USEITEM, 0, i, 0, 0);
				return true;
			}
			break;

		default:
			break;
		}
	}
	return false;
}
