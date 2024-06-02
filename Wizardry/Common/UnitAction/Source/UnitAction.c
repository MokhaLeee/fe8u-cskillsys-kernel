#include "common-chax.h"
#include "UnitAction.h"

/* LynJump */
unsigned int ApplyUnitAction(ProcPtr proc)
{
    const UnitActionFunc_t * it;

    gActiveUnit = GetUnit(gActionData.subjectIndex);

    if (gActionData.unitActionType >= CONFIG_UNIT_ACTION_AMT)
        return true;

    it = gpUnitActionTable + gActionData.unitActionType;
    if (!it)
        return true;

    return (*it)(proc);
}
