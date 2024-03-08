#include "global.h"
#include "bmitem.h"
#include "bmunit.h"
#include "statscreen.h"

#include "common-chax.h"
#include "weapon-range.h"

extern struct MsgBuffer sMsgString;

/* 96 ---> 9 6 */
STATIC_DECLAR void SplitNumberDec2(int in, u8 out[])
{
    out[0] = Div(in, 10);
    out[1] = in - out[0] * 10;
}

STATIC_DECLAR char * ItemRangeToString(int min, int max)
{
    int i, count = 0;
    u8 min_splited[2], max_splited[2];
    char str[5];

    if (max == 0)
        return GetStringFromIndex(0x522); /* Mag / 2[X] */

    if (min > max || min < 0 || max < 0)
        return GetStringFromIndex(0x52B); /* "--\0" */

    SplitNumberDec2(min, min_splited);
    SplitNumberDec2(max, max_splited);

    for (i = 0; i < 2; i++)
        if (min_splited[i] != 0)
            break;

    for (; i < 2; i++)
        str[count++] = '0' + min_splited[i];

    if (max > min)
    {
        str[count++] = '-';

        for (i = 0; i < 2; i++)
            if (max_splited[i] != 0)
                break;

        for (; i < 2; i++)
            str[count++] = '0' + max_splited[i];
    }

    for (i = 0; i < 5; i++)
    {
        if ((i + count) < 5)
            sMsgString.buffer1[i] = ' ';
        else
            sMsgString.buffer1[i] = str[i + count - 5];
    }
    sMsgString.buffer1[5] = '\0';
    return (char *)sMsgString.buffer1;
}

/* LynJump */
char * GetItemDisplayRangeString(int item)
{
    if ((ITEM_INDEX(item) == 0))
        return GetStringFromIndex(0x52B); /* "--\0" */

    int min = GetItemMinRange(item);
    int max = GetItemMaxRange(item);

    return ItemRangeToString(min, max);
}

const char * GetUnitDisplayRangeString(struct Unit * unit)
{
    u16 item;
    int min, max;

    if (!IS_UNIT_PTR(unit))
    {
        unit = gStatScreen.unit;
        if (!UNIT_IS_VALID(unit))
            return GetStringFromIndex(0x52B); /* "--\0" */
    }

    item = GetUnitEquippedWeapon(unit);
    if ((ITEM_INDEX(item) == 0))
        return GetStringFromIndex(0x52B); /* "--\0" */

    min = GetItemMinRangeRework(item, unit);
    max = GetItemMaxRangeRework(item, unit);

    LogDebugf("%s: %d - %d, item %#x", __func__, min, max, item);

    return ItemRangeToString(min, max);
}
