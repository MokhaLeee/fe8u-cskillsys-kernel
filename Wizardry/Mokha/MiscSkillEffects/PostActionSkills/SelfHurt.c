#include "global.h"
#include "bmusailment.h"
#include "bmunit.h"
#include "mu.h"
#include "proc.h"
#include "bmudisp.h"
#include "bmmind.h"

#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

struct ProcPostActionSelfHurt {
    PROC_HEADER;
    struct Unit * unit;
    int damage;
    u32 lock;
};

STATIC_DECLAR void PostActionWaitSelfHurtAnimDone(struct ProcPostActionSelfHurt * proc)
{
    if (proc->lock == GetGameClock())
        Proc_Break(proc);
}

STATIC_DECLAR void PostActionWaitSelfHurtReal(struct ProcPostActionSelfHurt * proc)
{
    AddUnitHp(proc->unit, -proc->damage);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionSelfHurt[] = {
    PROC_NAME("PostActionSelfHurt"),
    PROC_SLEEP(4),
    PROC_WHILE(PostActionWaitSelfHurtAnimDone),
    PROC_SLEEP(4),
    PROC_CALL(PostActionWaitSelfHurtReal),
    PROC_END
};

STATIC_DECLAR void PostActionSelfHurtCommon(ProcPtr parent, struct Unit * unit, int damage)
{
    struct ProcPostActionSelfHurt * proc;

    if (damage <= 0)
        return;

    MU_EndAll();
	RefreshUnitSprites();
    HideUnitSprite(unit);

    proc = Proc_StartBlocking(ProcScr_PostActionSelfHurt, parent);
    proc->unit = unit;
    proc->damage = damage;
    proc->lock = GetGameClock();

    BeginUnitCritDamageAnim(unit, damage);
}

bool PostActionBattleActorSelfHurt(ProcPtr parent)
{
    struct Unit * unit = gActiveUnit;
    int damage = 0;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
    case UNIT_ACTION_STAFF:
        if (SkillTester(unit, SID_Fury))
            damage += 6;

        if (SkillTester(unit, SID_FuryPlus))
            damage += 8;

        if (SkillTester(unit, SID_DoubleLion) && (gBattleActorGlobalFlag.skill_activated_double_lion))
            damage += 1;

        break;
    };

    if (damage == 0)
        return false;

    if (damage >= GetUnitCurrentHp(unit))
        damage = GetUnitCurrentHp(unit) - 1;

    PostActionSelfHurtCommon(parent, unit, damage);
    return true;
}

bool PostActionBattleTargetSelfHurt(ProcPtr parent)
{
    struct Unit * unit = GetUnit(gActionData.targetIndex);

    int damage = 0;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
    case UNIT_ACTION_STAFF:
        if (SkillTester(unit, SID_Fury))
            damage += 6;

        if (SkillTester(unit, SID_FuryPlus))
            damage += 8;

        break;
    };

    if (damage == 0)
        return false;

    if (damage >= GetUnitCurrentHp(unit))
        damage = GetUnitCurrentHp(unit) - 1;

    PostActionSelfHurtCommon(parent, unit, damage);
    return true;
}
