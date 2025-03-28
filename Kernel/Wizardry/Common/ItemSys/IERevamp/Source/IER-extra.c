#include "common-chax.h"
#include "item-sys.h"
#include "status-getter.h"

#define LOCAL_TRACE 1

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
