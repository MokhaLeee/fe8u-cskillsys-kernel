#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitAid(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status;

    if (!(UNIT_CATTRIBUTES(unit) & CA_MOUNTEDAID))
        status = UNIT_CON(unit) - 1;
    else if (UNIT_CATTRIBUTES(unit) & CA_FEMALE)
        status = 20 - UNIT_CON(unit);
    else
        status = 25 - UNIT_CON(unit);

    for (it = gAidGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}
