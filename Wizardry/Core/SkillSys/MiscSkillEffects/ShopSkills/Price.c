#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(GetItemPurchasePrice);
u16 GetItemPurchasePrice(struct Unit * unit, int item)
{
    int cost = GetItemCost(item);

    if (gBmSt.gameStateBits & BM_FLAG_PREPSCREEN)
        cost = cost + (cost / 2);

    if (UnitHasItem(unit, ITEM_SILVERCARD))
        cost = cost / 2;

#if (defined(SID_ShopSkill_Undefined) && COMMON_SKILL_VALID(SID_ShopSkill_Undefined))
    cost = Div(cost * 80, 100);
#endif

    return cost;
}
