#include "common-chax.h"

typedef void (*BeginActionFunc_t)(struct Unit *unit);
// extern const BeginActionFunc_t gBeginActionHooks[];
extern BeginActionFunc_t const *const gpBeginActionHooks;

LYN_REPLACE_CHECK(UnitBeginAction);
void UnitBeginAction(struct Unit *unit)
{
	gActiveUnit = unit;
	gActiveUnitId = unit->index;

	gActiveUnitMoveOrigin.x = unit->xPos;
	gActiveUnitMoveOrigin.y = unit->yPos;

	gActionData.subjectIndex = unit->index;
	gActionData.unitActionType = 0;
	gActionData.moveCount = 0;

	gBmSt.taken_action = 0;
	gBmSt.unk3F = 0xFF;

	sub_802C334();

	gActiveUnit->state |= US_HIDDEN;
	gBmMapUnit[unit->yPos][unit->xPos] = 0;

#if CHAX
{
	const BeginActionFunc_t *it;

	for (it = gpBeginActionHooks; *it; it++)
		(*it)(unit);
}
#endif
}
