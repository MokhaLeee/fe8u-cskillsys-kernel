#include "global.h"
#include "common-chax.h"

u32 simple_div(u32 a, u32 b)
{
    u32 i = 0;
    u32 _b = b;

    if (b == 0)
        return 0;

    while (a > b)
    {
        b = b + _b;
        i = i + 1;
    }
    return i;
}

u32 simple_mod(u32 a, u32 b)
{
    u32 _b = b;

    if (b == 0)
        return 0;

    while (a > b)
        b = b + _b;

    return (a + _b) - b;
}
