#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "list-verify.h"

bool JudgeUnitListHeader(struct Unit * unit, struct UnitListHeader * ref)
{
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

    if (ref->weapon != ITEM_INDEX(GetUnitEquippedWeapon(unit)))
        return false;

    return true;
}

void WriteUnitListHeader(struct Unit * unit, struct UnitListHeader * out)
{
    if (!UNIT_IS_VALID(unit) || !out)
        return;

    out->uid = unit->index;
    out->pid = UNIT_CHAR_ID(unit);
    out->jid = UNIT_CLASS_ID(unit);
    out->level = unit->level;
    out->weapon = ITEM_INDEX(GetUnitEquippedWeapon(unit));
}
