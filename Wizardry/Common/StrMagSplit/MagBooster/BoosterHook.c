#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "bmusemind.h"
#include "constants/items.h"

#include "common-chax.h"
#include "strmag.h"

extern struct Unit gStatGainSimUnit;

int ApplyStatBoostItem(struct Unit * unit, int itemIdx)
{
    const struct ItemStatBonuses * statBonuses;

    int item = unit->items[itemIdx];

    if (GetItemIndex(item) == ITEM_METISSTOME)
    {
        unit->state |= US_GROWTH_BOOST;
        UnitUpdateUsedItem(unit, itemIdx);
        return 0x1D; /* Maturity increased */
    }

    statBonuses = GetItemStatBonuses(item);

    unit->maxHP += statBonuses->hpBonus;
    unit->curHP += statBonuses->hpBonus;
    unit->pow += statBonuses->powBonus;
    unit->skl += statBonuses->sklBonus;
    unit->spd += statBonuses->spdBonus;
    unit->def += statBonuses->defBonus;
    unit->res += statBonuses->resBonus;
    unit->lck += statBonuses->lckBonus;
    unit->movBonus += statBonuses->movBonus;
    unit->conBonus += statBonuses->conBonus;

    UNIT_MAG(unit) += ITEM_MAG_BONUS(statBonuses);

    UnitCheckStatCaps(unit);
    UnitUpdateUsedItem(unit, itemIdx);

    if (statBonuses->hpBonus > 0)
        return 0x1C;
    else if (statBonuses->powBonus > 0)
        return 0x13;
    else if (ITEM_MAG_BONUS(statBonuses) > 0)
        return 0x14;
    else if (statBonuses->sklBonus > 0)
        return 0x15;
    else if (statBonuses->spdBonus > 0)
        return 0x16;
    else if (statBonuses->lckBonus > 0)
        return 0x17;
    else if (statBonuses->defBonus > 0)
        return 0x18;
    else if (statBonuses->resBonus > 0)
        return 0x19;
    else if (statBonuses->movBonus > 0)
        return 0x1A;
    else if (statBonuses->conBonus > 0)
        return 0x1B;

    return 0;
}

bool CanUnitUseStatGainItem(struct Unit * unit, int item)
{
    const struct ItemStatBonuses * bonuses = GetItemStatBonuses(item);

    ClearUnit(&gStatGainSimUnit);

    gStatGainSimUnit.pCharacterData = unit->pCharacterData;
    gStatGainSimUnit.pClassData     = unit->pClassData;

    gStatGainSimUnit.maxHP = unit->maxHP + bonuses->hpBonus;
    gStatGainSimUnit.pow = unit->pow + bonuses->powBonus;
    gStatGainSimUnit.skl = unit->skl + bonuses->sklBonus;
    gStatGainSimUnit.spd = unit->spd + bonuses->spdBonus;
    gStatGainSimUnit.def = unit->def + bonuses->defBonus;
    gStatGainSimUnit.res = unit->res + bonuses->resBonus;
    gStatGainSimUnit.lck = unit->lck + bonuses->lckBonus;
    gStatGainSimUnit.movBonus = unit->movBonus + bonuses->movBonus;
    gStatGainSimUnit.conBonus = unit->conBonus + bonuses->conBonus;

    UNIT_MAG(&gStatGainSimUnit) = ITEM_MAG_BONUS(bonuses);

    UnitCheckStatCaps(&gStatGainSimUnit);

    if (UNIT_MAG(&gStatGainSimUnit) != UNIT_MAG(unit))
        return true;

    if (gStatGainSimUnit.maxHP != unit->maxHP)
        return true;

    if (gStatGainSimUnit.pow != unit->pow)
        return true;

    if (gStatGainSimUnit.skl != unit->skl)
        return true;

    if (gStatGainSimUnit.spd != unit->spd)
        return true;

    if (gStatGainSimUnit.def != unit->def)
        return true;

    if (gStatGainSimUnit.res != unit->res)
        return true;

    if (gStatGainSimUnit.lck != unit->lck)
        return true;

    if (gStatGainSimUnit.movBonus != unit->movBonus)
        return true;

    if (gStatGainSimUnit.conBonus != unit->conBonus)
        return true;

    return false;
}
