#include "common-chax.h"
#include "status-getter.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(ExecBarrierStaff);
void ExecBarrierStaff(ProcPtr proc) {

    struct Unit *unit_act = GetUnit(gActionData.subjectIndex);
    struct Unit *unit_tar = GetUnit(gActionData.targetIndex);

    BattleInitItemEffect(unit_act, gActionData.itemSlotIndex);

    BattleInitItemEffectTarget(unit_tar);

    unit_tar->barrierDuration = 7;

#if defined(SID_ExplosiveBuff) && (COMMON_SKILL_VALID(SID_ExplosiveBuff))
    if (SkillTester(unit_act, SID_ExplosiveBuff))
    {
        for (int i = 0; i < ARRAY_COUNT_RANGE1x1; i++)
        {
            int _x = gActiveUnit->xPos + gVecs_1x1[i].x;
            int _y = gActiveUnit->yPos + gVecs_1x1[i].y;
    
            struct Unit * unit_adjacent = GetUnitAtPosition(_x, _y);
            if (!UNIT_IS_VALID(unit_adjacent))
                continue;
    
            if (unit_adjacent->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
                continue;
    
            if (AreUnitsAllied(gActiveUnit->index, unit_adjacent->index) && GetUnit(unit_adjacent->index) != unit_tar)
                unit_adjacent->barrierDuration = 7;
        }
    }
#endif

    BattleApplyItemEffect(proc);
    BeginBattleAnimations();

    return;
}