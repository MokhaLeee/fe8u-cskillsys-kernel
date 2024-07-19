#include "common-chax.h"
#include "skill-system.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "battle-system.h"
#include "constants/skills.h"

struct ProcPostActionHurt {
    PROC_HEADER;
    struct Unit * unit;
    int damage;
    u32 lock;
};

STATIC_DECLAR void PostActionHurt_Start(struct ProcPostActionHurt * proc)
{
    MapAnim_CommonInit();
    proc->lock = GetGameLock();
    BeginUnitCritDamageAnim(proc->unit, proc->damage);
}

STATIC_DECLAR void PostActionWaitHurtAnimDone(struct ProcPostActionHurt * proc)
{
    if (proc->lock == GetGameLock())
        Proc_Break(proc);
}

STATIC_DECLAR void PostActionHurt_End(struct ProcPostActionHurt * proc)
{
    AddUnitHp(proc->unit, -proc->damage);

    MapAnim_CommonEnd();
    RestoreBattleRoundInfo();
}

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionHurt[] = {
    PROC_NAME("PostActionHurt"),
    PROC_YIELD,
    PROC_CALL(PostActionHurt_Start),
    PROC_SLEEP(4),
    PROC_WHILE(PostActionWaitHurtAnimDone),
    PROC_SLEEP(4),
    PROC_CALL(PostActionHurt_End),
    PROC_YIELD,
    PROC_END
};

STATIC_DECLAR void PostActionHurtCommon(ProcPtr parent, struct Unit * unit, int damage)
{
    struct ProcPostActionHurt * proc;

    if (damage <= 0)
        return;

    EndAllMus();
    RefreshUnitSprites();
    HideUnitSprite(unit);

    proc = Proc_StartBlocking(ProcScr_PostActionHurt, parent);

    proc->unit = unit;
    proc->damage = damage;
}

bool PostActionBattleActorHurt(ProcPtr parent)
{
    struct Unit * unit = gActiveUnit;
    struct Unit * target = GetUnit(gActionData.targetIndex);
    int damage = 0;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
#if defined(SID_GrislyWound) && (COMMON_SKILL_VALID(SID_GrislyWound))
        if (SkillTester(target, SID_GrislyWound))
        {
            int GrislyWoundDamage = unit->maxHP / 5;

            if (GrislyWoundDamage > unit->curHP)
                GrislyWoundDamage = unit->curHP - 1;

            damage += GrislyWoundDamage;
        }
#endif

    /* fall through */

    case UNIT_ACTION_STAFF:
#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
        if (SkillTester(unit, SID_Fury))
            damage += 6;
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
        if (SkillTester(unit, SID_FuryPlus))
            damage += 8;
#endif

#if defined(SID_DoubleLion) && (COMMON_SKILL_VALID(SID_DoubleLion))
        if (SkillTester(unit, SID_DoubleLion) && (gBattleActorGlobalFlag.skill_activated_double_lion))
            damage += 1;
#endif

        break;
    };

    if (damage == 0)
        return false;

    if (damage >= GetUnitCurrentHp(unit))
        damage = GetUnitCurrentHp(unit) - 1;

    PostActionHurtCommon(parent, unit, damage);
    return true;
}

bool PostActionBattleTargetHurt(ProcPtr parent)
{
    struct Unit * unit = GetUnit(gActionData.targetIndex);
    struct Unit * actor = GetUnit(gActionData.subjectIndex);

    int damage = 0;

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
#if defined(SID_PoisonStrike) && (COMMON_SKILL_VALID(SID_PoisonStrike))
        if (SkillTester(actor, SID_PoisonStrike))
        {
            int poisonStrikeDamage = unit->maxHP / 5;

            if (poisonStrikeDamage > unit->curHP)
                poisonStrikeDamage = unit->curHP - 1;

            damage += poisonStrikeDamage;
        }
#endif

#if defined(SID_GrislyWound) && (COMMON_SKILL_VALID(SID_GrislyWound))
        if (SkillTester(actor, SID_GrislyWound))
        {
            int GrislyWoundDamage = unit->maxHP / 5;

            if (GrislyWoundDamage > unit->curHP)
                GrislyWoundDamage = unit->curHP - 1;

            damage += GrislyWoundDamage;
        }
#endif

    /* fall through */

    case UNIT_ACTION_STAFF:
#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
        if (SkillTester(unit, SID_Fury))
            damage += 6;
#endif

#if defined(SID_FuryPlus) && (COMMON_SKILL_VALID(SID_FuryPlus))
        if (SkillTester(unit, SID_FuryPlus))
            damage += 8;
#endif

        break;
    };

    if (damage == 0)
        return false;

    if (damage >= GetUnitCurrentHp(unit))
        damage = GetUnitCurrentHp(unit) - 1;

    PostActionHurtCommon(parent, unit, damage);
    return true;
}
