#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

/**
 * <!> WARNNING
 * We did not inject CON-Getters to a lot of vanilla function.
 * thus this function should not modified.
 */
int _GetUnitCon(struct Unit *unit)
{
	int status = UNIT_CON(unit);

	return status;
}
