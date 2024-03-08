#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "uiselecttarget.h"
#include "bmtarget.h"
#include "bmmap.h"
#include "proc.h"
#include "bmidoten.h"
#include "bmmind.h"

#include "common-chax.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"

void ForEachUnitInRange(void(* func)(struct Unit * unit));
void AddUnitToTargetListIfNotAllied(struct Unit * unit);

STATIC_DECLAR void ExecSkillSavageBlowEffectAnim(ProcPtr proc)
{
    int i;
    struct Unit * unit = gActiveUnit;
    struct Unit * target = GetUnit(gActionData.targetIndex);

    BmMapFill(gBmMapMovement, -1);
    BmMapFill(gBmMapRange, 0);
    GenerateUnitStandingReachRange(target, 0b11);

    InitTargets(unit->xPos, unit->yPos);
    ForEachUnitInRange(AddUnitToTargetListIfNotAllied);

    for (i = 0; i < GetSelectTargetCount(); i++)
    {
        struct SelectTarget * starget = GetTarget(i);
        CallMapAnim_HeavyGravity(proc, starget->x, starget->y);
    }
}

STATIC_DECLAR void SkillSavageBlowPostAnimEffect(ProcPtr proc)
{
    int i;
    int damage = Div(gBattleStats.damage * 2, 10);

    for (i = 0; i < GetSelectTargetCount(); i++)
    {
        struct SelectTarget * starget = GetTarget(i);
        struct Unit * tunit = GetUnit(starget->uid);

        if (tunit->curHP <= damage)
            tunit->curHP = 1;
        else
            tunit->curHP = tunit->curHP - damage;
    }
}

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionSkillSavageBlow[] = {
    PROC_NAME("PostActionSkillSavageBlow"),
    PROC_YIELD,
    PROC_CALL(ExecSkillSavageBlowEffectAnim),
    PROC_WHILE(MapAnimHeavyGravityExists),
    PROC_CALL(SkillSavageBlowPostAnimEffect),
    PROC_END
};

bool PostActionSkillSavageBlow(ProcPtr parent)
{
    struct Unit * unit = gActiveUnit;
    struct Unit * target = GetUnit(gActionData.targetIndex);

    if (!SkillTester(unit, SID_SavageBlow) || !UNIT_IS_VALID(target))
        return false;

    Proc_StartBlocking(ProcScr_PostActionSkillSavageBlow, parent);
    return true;
}
