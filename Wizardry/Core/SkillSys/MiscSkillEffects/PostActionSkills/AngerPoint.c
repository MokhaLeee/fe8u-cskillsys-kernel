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

void PostActionAngerPoint(ProcPtr parent)
{
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    switch (gActionData.unitActionType)
    {
        case UNIT_ACTION_COMBAT:
#if defined(SID_AngerPoint) && (COMMON_SKILL_VALID(SID_AngerPoint))
            if (SkillTester(unit, SID_AngerPoint) &&
                gBattleActorGlobalFlag.skill_activated_angerpoint)
                SetUnitStatDebuff(unit, UNIT_STAT_BUFF_ANGERPOINT);
            else if (
                SkillTester(GetUnit(gActionData.targetIndex), SID_AngerPoint) &&
                gBattleActorGlobalFlag.skill_activated_angerpoint)
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_BUFF_ANGERPOINT);
#endif
        default:
            break;
    }
}
