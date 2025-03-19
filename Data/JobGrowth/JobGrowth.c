#include <common-chax.h>
#include <lvup.h>

struct JobGrowthEnt const gJobGrowthList[0x100] = {
	[CLASS_EPHRAIM_LORD] = {{
		[UNIT_STATUS_POW] = 30,
		[UNIT_STATUS_SKL] = -10,
	}},
};
