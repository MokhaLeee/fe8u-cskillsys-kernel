#include "common-chax.h"
#include "debuff.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "combat-art.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

void PostActionDebilitator(ProcPtr parent)
{
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    switch (gActionData.unitActionType)
    {
        case UNIT_ACTION_COMBAT:
#if defined(SID_Debilitator) && (COMMON_SKILL_VALID(SID_Debilitator))
            if (SkillTester(unit, SID_Debilitator) &&
                gBattleActorGlobalFlag.skill_activated_debilitator)
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_DEBUFF_DEBILITATOR);
            else if (
                SkillTester(GetUnit(gActionData.targetIndex), SID_Debilitator) &&
                gBattleActorGlobalFlag.skill_activated_debilitator)
                SetUnitStatDebuff(unit, UNIT_STAT_DEBUFF_DEBILITATOR);
#endif
        default:
            break;
    }
}
