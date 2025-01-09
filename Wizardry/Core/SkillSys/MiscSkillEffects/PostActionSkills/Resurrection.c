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

#if (defined(SID_Resurrection) && (COMMON_SKILL_VALID(SID_Resurrection)))
STATIC_DECLAR void PrepareResurrection(void)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    if (targetUnit->_u3A != UES_BIT_RESURRECTION_SKILL_USED)
    {
        AddUnitHp(targetUnit, (targetUnit->maxHP - targetUnit->curHP));
        targetUnit->_u3A = UES_BIT_RESURRECTION_SKILL_USED;
    }
}
#endif

bool PostActionResurrection(ProcPtr proc)
{

    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

#if (defined(SID_Resurrection) && (COMMON_SKILL_VALID(SID_Resurrection)))
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);
    
    if (SkillTester(targetUnit, SID_Resurrection) && gBattleTargetGlobalFlag.skill_activated_resurrection)
    {
        STATIC_DECLAR const EventScr EventScr_PostActionResurrection[] = {
            ASMC(PrepareResurrection)
            BREAKSTONE_TARGET_UNIT
            ASMC(MapAnim_CommonEnd)
            NOFADE
            ENDA
        };
        KernelCallEvent(EventScr_PostActionResurrection, EV_EXEC_CUTSCENE, proc);
        return true;
    }
#endif
    return false;
}
