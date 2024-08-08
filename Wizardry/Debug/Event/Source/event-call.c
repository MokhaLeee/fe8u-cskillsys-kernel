#include "common-chax.h"
#include "debug-event.h"
#include "constants/skills.h"

void PrologueCallBack(void)
{
    struct Unit * unit;

    unit = GetUnitFromCharId(CHARACTER_EPHRAIM);
    if (unit)
    {
        UnitAddItem(unit, (SID_Fury << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
    }
}
