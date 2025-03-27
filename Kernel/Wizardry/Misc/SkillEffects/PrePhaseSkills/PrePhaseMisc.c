#include "common-chax.h"
#include "unit-expa.h"

bool PrePhsae_ClearMiscUES(ProcPtr proc)
{
	int uid;
	struct Unit *unit;

	for (uid = 1; uid < 0xC0; uid++) {
		unit = GetUnit(uid);

		if (UNIT_IS_VALID(unit)) {
			ClearBitUES(unit, UES_BIT_GUARDBEAR_SKILL_USED);
			ClearBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
			ClearBitUES(unit, UES_BIT_TELEPOTRATION_SKILL_USED);
		}
	}
	return false;
}
