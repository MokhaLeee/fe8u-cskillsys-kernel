#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "bmmap.h"
#include "bmidoten.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "weapon-range.h"
#include "constants/skills.h"

typedef int (* WeaponRangeGetterFunc_t)(int old, struct Unit * unit, u16 item);
extern const WeaponRangeGetterFunc_t gWeaponRangeGetters[];

int GetItemMinRangeRework(u16 item, struct Unit * unit)
{
    int status = GetItemMinRange(item);

    if (IS_UNIT_PTR(unit))
    {

    }
    return status;
}

int GetItemMaxRangeRework(u16 item, struct Unit * unit)
{
    const WeaponRangeGetterFunc_t * it;
    int status = GetItemMaxRange(item);
    if (IS_UNIT_PTR(unit))
    {
        for (it = gWeaponRangeGetters; *it; it++)
            status = (*it)(status, unit, item);
    }
    return status;
}

int WeaponRangeGetterSkills(int range, struct Unit * unit, u16 item)
{
    switch (GetItemType(item)) {
    case ITYPE_BOW:
        if (SkillTester(unit, SID_RangeBonusBow1))
            range = range + 1;

        if (SkillTester(unit, SID_RangeBonusBow2))
            range = range + 2;
        break;

    case ITYPE_ANIMA:
    case ITYPE_LIGHT:
    case ITYPE_DARK:
        if (SkillTester(unit, SID_RangeBonusBMag1))
            range = range + 1;

        if (SkillTester(unit, SID_RangeBonusBMag2))
            range = range + 2;
        break;
    }
    return range;
}
