#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"
#include "bmunit.h"

int _GetUnitMaxHp(struct Unit * unit)
{
    const StatusGetterFunc_t * it;
    int status = unit->maxHP;

#if defined(SID_Unaware) && (COMMON_SKILL_VALID(SID_Unaware))
    if (unit == GetUnit(gBattleActor.unit.index) && GetUnit(gBattleTarget.unit.index) && SkillTester(GetUnit(gBattleTarget.unit.index), SID_Unaware))
        return status;
    else if (unit == GetUnit(gBattleTarget.unit.index) && GetUnit(gBattleActor.unit.index) && SkillTester(GetUnit(gBattleActor.unit.index), SID_Unaware))
        return status;
#endif

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
#if defined(SID_HpBonus) && (COMMON_SKILL_VALID(SID_HpBonus))
    if (SkillTester(unit, SID_HpBonus))
        status += SKILL_EFF0(SID_HpBonus);
#endif

#if defined(SID_Lifefont) && (COMMON_SKILL_VALID(SID_Lifefont))
    if (SkillTester(unit, SID_Lifefont))
    {
        switch (gBmMapTerrain[unit->yPos][unit->xPos]) {
            case TERRAIN_FORT:
            case TERRAIN_THRONE:
                int boost = (GetTerrainHealAmount(gBmMapTerrain[unit->yPos][unit->xPos]) * unit->maxHP / 100);
                status += boost;
                break;
    
            default:
                break;
        }
    }
#endif

    return status;
}
