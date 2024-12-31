#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "battle-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "strmag.h"

#if defined(SID_MendArms) && (COMMON_SKILL_VALID(SID_MendArms))
bool PostActionMendArms(ProcPtr proc)
{   
    struct Unit * survivor = gActiveUnit;

    int weaponUsesExpended = 0;

    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (BattleSkillTester(&gBattleActor, SID_MendArms) && UNIT_ALIVE(GetUnit(gBattleActor.unit.index)) && !UNIT_ALIVE(GetUnit(gBattleTarget.unit.index)))
    {
        weaponUsesExpended = GetItemUses(gBattleActor.weaponBefore) - GetItemUses(gBattleActor.weapon);
        survivor = GetUnit(gBattleActor.unit.index);
    }
    else if (BattleSkillTester(&gBattleTarget, SID_MendArms) && UNIT_ALIVE(GetUnit(gBattleTarget.unit.index)) && !UNIT_ALIVE(GetUnit(gBattleActor.unit.index)))
    {
        weaponUsesExpended = GetItemUses(gBattleTarget.weaponBefore) - GetItemUses(gBattleTarget.weapon);
        survivor = GetUnit(gBattleTarget.unit.index);
    }
    else
        return false;

    /* Skill holder needs to beat enemy in one hit. This might conflict with other skills that restore weapon durability */
    if (weaponUsesExpended != 1)
        return false;

    for (int i = 0; i < 5; i++) 
    {
        int item = GetItemIndex(survivor->items[i]);

        if (CanUnitUseWeapon(survivor, item))
            if (GetItemUses(item) < GetItemMaxUses(item))
				survivor->items[i] = (item += ((GetItemUses(survivor->items[i]) + 1) << 8));
    }

    return true;
}
#endif