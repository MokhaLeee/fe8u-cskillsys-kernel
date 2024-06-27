#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "constants/skills.h"

int GetUnitItemHealAmount(struct Unit * unit, int item)
{
    int result = 0;

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

    if (GetItemAttributes(item) & IA_STAFF)
        result += MagGetter(unit);

#if (defined(SID_GentleFlower) && COMMON_SKILL_VALID(SID_GentleFlower))
    if (SkillTester(unit, SID_GentleFlower))
        result += result / 2;
#endif

    if (result > 80)
        result = 80;

    return result;
}
