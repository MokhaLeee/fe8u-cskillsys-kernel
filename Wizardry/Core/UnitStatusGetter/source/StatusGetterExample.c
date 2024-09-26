#include "common-chax.h"
#include "debug-kit.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int StatusGetterTest(int status, struct Unit *unit)
{
	Printf("%s: Unit got +1 bonus!", __func__);
	status += 1;

	if (UNIT_CHAR_ID(unit) == CHARACTER_EIRIKA) {
		Printf("%s: Eirika got +1 bonus!", __func__);
		status += 1;
	}

	return status;
}
