#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "constants/characters.h"
#include "constants/items.h"

#include "common-chax.h"
#include "debug-kit.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

/* LynJump! */
int StatusGetterTest(int status, struct Unit * unit)
{
    LogPrintf("%s: Unit got +1 bonus!", __func__);
    status += 1;

    if (UNIT_CHAR_ID(unit) == CHARACTER_EIRIKA)
    {
        LogPrintf("%s: Eirika got +1 bonus!", __func__);
        status += 1;
    }

    return status;
}
