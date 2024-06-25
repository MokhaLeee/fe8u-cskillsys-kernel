#include "common-chax.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"

void ForEachUnitInRange(void(* func)(struct Unit * unit));
void AddUnitToTargetListIfAllied(struct Unit * unit);

STATIC_DECLAR void ExecSkillBreathOfLifeEffectAnim(ProcPtr proc)
{
    struct Unit * unit = gActiveUnit;

    BmMapFill(gBmMapMovement, -1);
    BmMapFill(gBmMapRange, 0);
    GenerateUnitStandingReachRange(unit, 0b11);

    InitTargets(unit->xPos, unit->yPos);
    ForEachUnitInRange(AddUnitToTargetListIfAllied);

#if 0
    int i;
    for (i = 0; i < GetSelectTargetCount(); i++)
    {
        struct SelectTarget * starget = GetTarget(i);
        CallMapAnim_HeavyGravity(proc, starget->x, starget->y);
    }
#endif
}

STATIC_DECLAR void SkillBreathOfLifePostAnimEffect(ProcPtr proc)
{
    int i;

    for (i = 0; i < GetSelectTargetCount(); i++)
    {
        struct SelectTarget * starget = GetTarget(i);
        struct Unit * tunit = GetUnit(starget->uid);

        int max_hp = GetUnitMaxHp(tunit);
        int heal_amt = Div(max_hp * 2, 10);

        if ((tunit->curHP + heal_amt) <= max_hp)
            tunit->curHP = tunit->curHP + heal_amt;
        else
            tunit->curHP = max_hp;
    }
}

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionSkillBreathOfLife[] = {
    PROC_NAME("PostActionSkillBreathOfLife"),
    PROC_YIELD,
    PROC_CALL(ExecSkillBreathOfLifeEffectAnim),
    // PROC_WHILE(MapAnimHeavyGravityExists),
    PROC_CALL(SkillBreathOfLifePostAnimEffect),
    PROC_END
};

bool PostActionSkillBreathOfLife(ProcPtr parent)
{
    struct Unit * unit = gActiveUnit;

#if defined(SID_Canto) && (COMMON_SKILL_VALID(SID_Canto))
    if (!SkillTester(unit, SID_BreathOfLife))
#else
    if (1)
#endif
        return false;

    Proc_StartBlocking(ProcScr_PostActionSkillBreathOfLife, parent);
    return true;
}
