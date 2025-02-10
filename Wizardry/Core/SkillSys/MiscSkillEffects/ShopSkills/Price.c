#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "item-sys.h"

const int dict_size = sizeof(dict_skills) / sizeof(dict_skills[0]);

// Custom binary search function
int binary_search_skills(const KeyValuePair * array, int size, const char * key, int value)
{
    int low = 0;
    int high = size - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = strcmp(array[mid].key, key);

        if (cmp == 0)
        {
            return array[mid].values[value]; // Key found
        }
        else if (cmp < 0)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1; // Key not found
}

LYN_REPLACE_CHECK(GetItemPurchasePrice);
u16 GetItemPurchasePrice(struct Unit * unit, int item)
{
    int cost = 0;
    int value = -1;

#if CHAX
    // Check if it's a skill scroll
    if (IsDuraItem(item))
    {
        char * key = GetDuraItemName(item);
        value = binary_search_skills(dict_skills, dict_size, key, 0);
        if (value != -1)
            cost = value;
    }
    else
        cost = GetItemCost(item);
#else
    cost = GetItemCost(item);
#endif

    if (gBmSt.gameStateBits & BM_FLAG_PREPSCREEN)
        cost = cost + (cost / 2);

    if (UnitHasItem(unit, ITEM_SILVERCARD))
    {
        cost = cost / 2;
        return cost;
    }

#if (defined(SID_Bargain) && COMMON_SKILL_VALID(SID_Bargain))
    if (SkillTester(unit, SID_Bargain))
    {
        cost = Div(cost * 50, 1000);
        return cost;
    }
#endif

#if (defined(SID_Deal) && COMMON_SKILL_VALID(SID_Deal))
    if (SkillTester(unit, SID_Deal))
    {
        cost = Div(cost * 80, 1000);
        return cost;
    }
#endif

    return cost;
}

LYN_REPLACE_CHECK(GetItemSellPrice);
u16 GetItemSellPrice(int item)
{
#if (defined(SID_Trader) && COMMON_SKILL_VALID(SID_Trader))
    if (SkillTester(gActiveUnit, SID_Trader))
        return (GetItemCost(item) / 4) * 3;
#endif   
    return GetItemCost(item) / 2;
}