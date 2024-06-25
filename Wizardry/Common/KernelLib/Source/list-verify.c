#include "common-chax.h"
#include "list-verify.h"

bool JudgeUnitListHeader(struct Unit * unit, struct UnitListHeader * ref)
{
    int i;

    if (!UNIT_IS_VALID(unit) || !ref)
        return false;

    if (ref->uid != unit->index)
        return false;

    if (ref->pid != UNIT_CHAR_ID(unit))
        return false;

    if (ref->jid != UNIT_CLASS_ID(unit))
        return false;

    if (ref->level != unit->level)
        return false;

    for (i = 0; i < UNIT_ITEM_COUNT; i++)
        if (ref->items[i] != ITEM_INDEX(unit->items[i]))
            return false;

    return true;
}

void WriteUnitListHeader(struct Unit * unit, struct UnitListHeader * out)
{
    int i;

    if (!UNIT_IS_VALID(unit) || !out)
        return;

    out->uid = unit->index;
    out->pid = UNIT_CHAR_ID(unit);
    out->jid = UNIT_CLASS_ID(unit);
    out->level = unit->level;

    for (i = 0; i < UNIT_ITEM_COUNT; i++)
        out->items[i] = ITEM_INDEX(unit->items[i]);
}
