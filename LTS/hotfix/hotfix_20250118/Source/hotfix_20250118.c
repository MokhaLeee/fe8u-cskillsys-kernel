#include "common-chax.h"
#include "debuff.h"
#include "strmag.h"

int ResGetterPureWater(int status, struct Unit *unit)
{
	status += unit->barrierDuration;

	// return status;
	return ResGetterDebuff(status, unit);
}

int MagGetterWeaponBonus(int status, struct Unit *unit)
{
	const struct ItemStatBonuses *bonus;
	u16 weapon = GetUnitEquippedWeapon(unit);

	if (weapon) {
		bonus = GetItemStatBonuses(weapon);

		if (bonus)
			status = status + ITEM_MAG_BONUS(bonus);
	}

	// return status;
	return MagGetterDebuff(status, unit);
}

void UnitAutolevelCore_Mag(struct Unit *unit, int count)
{
	UNIT_MAG(unit) += GetAutoleveledStatIncrease(gpMagicJInfos[UNIT_CLASS_ID(unit)].growth, count);
}
