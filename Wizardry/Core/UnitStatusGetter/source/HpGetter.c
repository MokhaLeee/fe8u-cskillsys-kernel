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
    struct Unit * target = GetUnit(gBattleTarget.unit.index);
    struct Unit * actor = GetUnit(gBattleActor.unit.index);
/*
** This needs to be first, as it returns the status value before changes are applied
*/
#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
        if (unit == target)
        {
            if (SkillTester(actor, SID_Unaware))
                return status;
        }
        else if (unit == actor)
        {
            if (SkillTester(target, SID_Unaware))
                return status;
        }
#endif

    u16 weapon = GetUnitEquippedWeapon(unit);
    status += GetItemHpBonus(weapon);
    return status;
}

int HpGetterSkills(int status, struct Unit * unit)
{
    struct Unit * target = GetUnit(gBattleTarget.unit.index);
    struct Unit * actor = GetUnit(gBattleActor.unit.index);
/*
** This needs to be first, as it returns the status value before changes are applied
*/
#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
        if (unit == target)
        {
            if (SkillTester(actor, SID_Unaware))
                return status;
        }
        else if (unit == actor)
        {
            if (SkillTester(target, SID_Unaware))
                return status;
        }
#endif

#if defined(SID_HpBonus) && (COMMON_SKILL_VALID(SID_HpBonus))
    if (SkillTester(unit, SID_HpBonus))
        status += SKILL_EFF0(SID_HpBonus);
#endif

    return status;
}
