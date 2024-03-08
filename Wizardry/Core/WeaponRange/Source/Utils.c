#include "global.h"
#include "bmunit.h"
#include "bmitem.h"
#include "bmmap.h"
#include "bmidoten.h"

#include "common-chax.h"
#include "status-getter.h"
#include "weapon-range.h"

STATIC_DECLAR u32 GetRangeMask(int min, int max)
{
    int i;
    u32 mask = 0;

    for (i = 0; i < 32; i++)
    {
        if (i < min)
            continue;

        if (i > max)
            continue;

        mask |= 1 << i;
    }
    return mask;
}

/* External jump at 0x080170D4 */
u32 GetItemReachBitsRework(u16 item, struct Unit * unit)
{
    int min = GetItemMinRangeRework(item, unit);
    int max = GetItemMaxRangeRework(item, unit);
    return GetRangeMask(min, max);
}

/* External jump at 0x08016B8C */
bool IsItemCoveringRangeRework(int item, int range, struct Unit * unit)
{
    if (range > GetItemMaxRangeRework(item, unit))
        return false;

    if (range < GetItemMinRangeRework(item, unit))
        return false;

    return true;
}

void AddMap(int x, int y, u32 mask, int on, int off)
{
    int ix, iy;

    int X1 = x - 32;
    int X2 = x + 32;
    int Y1 = y - 32;
    int Y2 = y + 32;

    LIMIT_AREA(X1, 0, gBmMapSize.x);
    LIMIT_AREA(X2, 0, gBmMapSize.x);
    LIMIT_AREA(Y1, 0, gBmMapSize.y);
    LIMIT_AREA(Y2, 0, gBmMapSize.y);

    for (iy = Y1; iy < Y2; iy++)
    {
        for (ix = X1; ix < X2; ix++)
        {
            int distance = RECT_DISTANCE(x, y, ix, iy);
            if (mask & (1 << distance))
                gWorkingBmMap[iy][ix] += on;
            else
                gWorkingBmMap[iy][ix] += off;
        }
    }
}

void AddMapForItem(struct Unit * unit, u16 item)
{
    int min = GetItemMinRangeRework(item, unit);
    int max = GetItemMaxRangeRework(item, unit);

    if (max == 0)
        max = GetUnitMagBy2Range(unit);

    MapAddInRange(unit->xPos, unit->yPos, max, 1);

    if (min != 0)
        MapAddInRange(unit->xPos, unit->yPos, min - 1, -1);
}

void ForEachUnit(void (* func)(struct Unit *), u8 ** map, const int off)
{
    int x, y;

    if (gBmMapSize.x < 0 || gBmMapSize.y < 0)
        return;

    for (y = 0; y < gBmMapSize.y; y++)
    {
        for (x = 0; x < gBmMapSize.x; x++)
        {
            if (off == map[y][x])
                continue;

            if (gBmMapUnit[y][x] != 0)
                func(GetUnit(gBmMapUnit[y][x]));
        }
    }
}
