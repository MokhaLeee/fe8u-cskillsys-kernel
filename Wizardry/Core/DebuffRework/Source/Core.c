#include "common-chax.h"
#include "debuff.h"

#define LOCAL_TRACE 0

int GetUnitStatusIndex(struct Unit *unit)
{
	return UNIT_STATUS_INDEX(unit);
}

int GetUnitStatusDuration(struct Unit *unit)
{
	int ret = 0;

	if (UNIT_STATUS_INDEX(unit) != 0)
		ret = UNIT_STATUS_DURATION(unit) + 1;

	LTRACEF("[uid=0x%02X, pid=0x%02X] status=%d, dura=%d",
		unit->index & 0xFF, UNIT_CHAR_ID(unit),
		UNIT_STATUS_INDEX(unit), ret);

	return ret;
}

void SetUnitStatusIndex(struct Unit *unit, int status)
{
	if (status < NEW_UNIT_STATUS_MAX)
		UNIT_STATUS_INDEX(unit) = status;
}

void SetUnitStatusDuration(struct Unit *unit, int duration)
{
	if (duration == 0) {
		UNIT_STATUS_INDEX(unit) = 0;
		UNIT_STATUS_DURATION(unit) = 0;
		return;
	}

	if (duration > NEW_UNIT_STATUS_MAX_DURATION) {
		Errorf("Status duration overflow: %d", duration);
		UNIT_STATUS_DURATION(unit) = 3;

	}

	UNIT_STATUS_DURATION(unit) = duration - 1;
}

int TryTickUnitStatusDuration(struct Unit *unit)
{
	if (UNIT_STATUS_INDEX(unit) == 0)
		return -1;

	LTRACEF("[pid=0x%02X] status=%d, dura=%d",
		UNIT_CHAR_ID(unit), UNIT_STATUS_INDEX(unit), UNIT_STATUS_DURATION(unit));

	if (UNIT_STATUS_DURATION(unit) != 0) {
		UNIT_STATUS_DURATION(unit)--;
	} else {
		UNIT_STATUS_INDEX(unit) = 0;
		UNIT_STATUS_DURATION(unit) = 0;
	}
	return 0;
}
