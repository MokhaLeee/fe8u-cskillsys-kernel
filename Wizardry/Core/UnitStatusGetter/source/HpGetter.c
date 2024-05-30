#include "global.h"
#include "bmunit.h"
#include "bmitem.h"

#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int _GetUnitMaxHp(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->maxHP;

    for (it = gpHpGetters; *it; it++)
        status = (*it)(status, unit);

    return status;
}

/* Hooks */
int HpGetterWeaponBonus(int status, struct Unit * unit)
{
    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemHpBonus(weapon);
    return status;
}

int HpGetterSkills(int status, struct Unit * unit)
{
#if defined(SID_HpBonus) && (SID_HpBonus < MAX_SKILL_NUM)
    if (SkillTester(unit, SID_HpBonus))
        status += 5;
#endif

    return status;
}
