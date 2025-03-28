#include "common-chax.h"
#include "strmag.h"

extern struct Unit gStatGainSimUnit;

LYN_REPLACE_CHECK(CanUnitUseStatGainItem);
bool CanUnitUseStatGainItem(struct Unit *unit, int item)
{
	const struct ItemStatBonuses *bonuses = GetItemStatBonuses(item);

	ClearUnit(&gStatGainSimUnit);

	gStatGainSimUnit.pCharacterData = unit->pCharacterData;
	gStatGainSimUnit.pClassData	 = unit->pClassData;

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
