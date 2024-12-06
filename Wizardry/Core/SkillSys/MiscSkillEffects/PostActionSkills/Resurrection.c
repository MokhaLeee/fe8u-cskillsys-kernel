#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "battle-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "strmag.h"
#include "jester_headers/macros.h"
#include "unit-expa.h"

STATIC_DECLAR void PrepareResurrection(void)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    if (targetUnit->_u3A != UES_BIT_RESURRECTION_SKILL_USED)
    {
        AddUnitHp(targetUnit, (targetUnit->maxHP - targetUnit->curHP));
        targetUnit->_u3A = UES_BIT_RESURRECTION_SKILL_USED;
    }
}

STATIC_DECLAR const EventScr EventScr_PostActionResurrection[] = {
    ASMC(PrepareResurrection)
    BREAKSTONE_TARGET_UNIT
    ASMC(MapAnim_CommonEnd)
    NoFade
    ENDA
};

bool PostActionResurrection(ProcPtr proc)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

#if defined(SID_Resurrection) && (COMMON_SKILL_VALID(SID_Resurrection))
    if (SkillTester(targetUnit, SID_Resurrection) && gBattleTargetGlobalFlag.skill_activated_resurrection)
    {
        KernelCallEvent(EventScr_PostActionResurrection, EV_EXEC_CUTSCENE, proc);
        return true;
    }
#endif
    return false;
}
