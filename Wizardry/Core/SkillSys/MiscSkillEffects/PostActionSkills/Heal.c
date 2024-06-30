#include "common-chax.h"
#include "debuff.h"
#include "battle-system.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

struct ProcPostActionHeal {
    PROC_HEADER;
    int heal;
};

STATIC_DECLAR void PostActionHeal_MoveCamera(ProcPtr proc)
{
    EnsureCameraOntoPosition(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

STATIC_DECLAR void PostActionHeal_ExecAnim(struct ProcPostActionHeal * proc)
{
    HideUnitSprite(gActiveUnit);
    BeginUnitHealAnim(gActiveUnit, proc->heal);
}

STATIC_DECLAR void PostActionHeal_ExecBmHeal(struct ProcPostActionHeal * proc)
{
    AddUnitHp(gActiveUnit, proc->heal);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionHeal[] = {
    PROC_CALL(MapAnim_CommonInit),
    PROC_CALL(PostActionHeal_MoveCamera),
    PROC_YIELD,
    PROC_CALL(PostActionHeal_ExecAnim),
    PROC_YIELD,
    PROC_CALL(PostActionHeal_ExecBmHeal),
    PROC_CALL(MapAnim_CommonEnd),
    PROC_END
};

bool PostAction_BattleActorHeal(ProcPtr parent)
{
    int heal = 0;
    struct Unit * unit = gActiveUnit;
    struct ProcPostActionHeal * proc;

    int hp_cur = GetUnitCurrentHp(unit);
    int hp_max = GetUnitMaxHp(unit);

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

#if defined(SID_Lifetaker) && (COMMON_SKILL_VALID(SID_Lifetaker))
    if (SkillTester(unit, SID_Lifetaker) && gBattleActorGlobalFlag.enimy_defeated)
        heal += hp_max / 4;
#endif

    if (heal == 0)
        return false;

    if ((heal >= (hp_max - hp_cur)))
        heal = hp_max - hp_cur;

    proc = Proc_StartBlocking(ProcScr_PostActionHeal, parent);
    proc->heal = heal;

    return true;
}
