#include "common-chax.h"

typedef int (* ClearUnitFunc_t)(struct Unit * unit);
extern const ClearUnitFunc_t gClearUnitHooks[];

/* LynJump */
void ClearUnit(struct Unit * unit)
{
    u8 id = unit->index;

#if CHAX
    const ClearUnitFunc_t * it;
    for (it = gClearUnitHooks; *it; it++)
        (*it)(unit);
#endif

    CpuFill16(0, unit, sizeof(struct Unit));
    unit->index = id;
}
