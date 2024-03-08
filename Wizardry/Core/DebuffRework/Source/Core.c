#include "global.h"
#include "bmunit.h"

#include "common-chax.h"
#include "debuff.h"

struct UnitStatusRework {
    u8 index : 6;
    u8 duration: 2;
};

#define UNIT_STATUS(unit) ((struct UnitStatusRework *)((u8 *)(unit) + 0x30))
#define UNIT_STATUS_INDEX(unit) (UNIT_STATUS(unit)->index)
#define UNIT_STATUS_DURATION(unit) (UNIT_STATUS(unit)->duration)

int GetUnitStatusIndex(struct Unit * unit)
{
    return UNIT_STATUS_INDEX(unit);
}

int GetUnitStatusDuration(struct Unit * unit)
{
    if (0 != UNIT_STATUS_INDEX(unit))
        return UNIT_STATUS_DURATION(unit) + 1;

    return 0;
}

void SetUnitStatusIndex(struct Unit * unit, int status)
{
    if (status < NEW_UNIT_STATUS_MAX)
        UNIT_STATUS_INDEX(unit) = status;
}

void SetUnitStatusDuration(struct Unit * unit, int duration)
{
    if (duration == 0)
    {
        UNIT_STATUS_INDEX(unit) = 0;
        UNIT_STATUS_DURATION(unit) = 0;
        return;
    }

    if (duration > NEW_UNIT_STATUS_MAX_DURATION)
    {
        Errorf("Status duration overflow: %d", duration);
        UNIT_STATUS_DURATION(unit) = 3;

    }

    UNIT_STATUS_DURATION(unit) = duration - 1;
}

int TryTickUnitStatusDuration(struct Unit * unit)
{
    if (UNIT_STATUS_INDEX(unit) == 0)
        return -1;

    if (UNIT_STATUS_DURATION(unit) != 0)
    {
        UNIT_STATUS_DURATION(unit)--;
    }
    else
    {
        UNIT_STATUS_INDEX(unit) = 0;
        UNIT_STATUS_DURATION(unit) = 0;
    }
    return 0;
}
