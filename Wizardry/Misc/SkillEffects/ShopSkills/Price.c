#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(GetItemPurchasePrice);
u16 GetItemPurchasePrice(struct Unit *unit, int item)
{
	int cost = GetItemCost(item);

	if (gBmSt.gameStateBits & BM_FLAG_PREPSCREEN)
		cost = cost + (cost / 2);

	if (UnitHasItem(unit, ITEM_SILVERCARD)) {
		cost = cost / 2;
		return cost;
	}

#if (defined(SID_Bargain) && COMMON_SKILL_VALID(SID_Bargain))
	if (SkillListTester(unit, SID_Bargain)) {
		cost = Div(cost * 50, 100);
		return cost;
	}
#endif

#if (defined(SID_Deal) && COMMON_SKILL_VALID(SID_Deal))
	if (SkillListTester(unit, SID_Deal)) {
		cost = Div(cost * 80, 100);
		return cost;
	}
#endif

	return cost;
}
