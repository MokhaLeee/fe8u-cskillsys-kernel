#include "global.h"
#include "bmmap.h"
#include "bmunit.h"

#include "common-chax.h"

bool IsPositionValid(s8 x, s8 y)
{
    if (x < 0)
        return false;

    if (y < 0)
        return false;

    if (x > gBmMapSize.x)
        return false;

    if (y > gBmMapSize.y)
        return false;

    return true;
}

struct Unit * GetUnitAtPosition(s8 x, s8 y)
{
    s8 uid;

    if (!IsPositionValid(x, y))
        return NULL;

    uid = gBmMapUnit[y][x];
    if (!uid)
        return NULL;

    return GetUnit(uid);
}
