#include "common-chax.h"
#include "debuff.h"
#include "battle-system.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"

STATIC_DECLAR void PostActionSynchronize_StartActor(ProcPtr proc)
{
    struct Unit * unit_act = GetUnit(gActionData.subjectIndex);

    MU_StartActionAnim(MU_Create(unit_act));
}

STATIC_DECLAR void PostActionSynchronize_StartTargetAnim(ProcPtr proc)
{
    struct Unit * unit_tar = GetUnit(gActionData.targetIndex);

    if (IsDebuff(GetUnitStatusIndex(GetUnit(gActionData.subjectIndex))))
        CallMapAnim_HeavyGravity(proc, unit_tar->xPos, unit_tar->yPos);
    else
        StartStatusHealEffect(unit_tar, proc);
}

STATIC_DECLAR void PostActionSynchronize_End(ProcPtr proc)
{
    SetUnitStatus(
        GetUnit(gActionData.targetIndex),
        GetUnitStatusIndex(GetUnit(gActionData.subjectIndex)));

    MU_EndAll();

    /**
     * Since the StartStatusHealEffect() may change the active-unit,
     * so here we need to recover it
     */
    gActiveUnit = GetUnit(gActionData.subjectIndex);

    ShowUnitSprite(gActiveUnit);
    RefreshUnitSprites();
}

STATIC_DECLAR const struct ProcCmd ProcScr_PostActionSynchronize[] = {
    PROC_YIELD,
    PROC_CALL(PostActionSynchronize_StartActor),
    PROC_SLEEP(30),
    PROC_CALL(PostActionSynchronize_StartTargetAnim),
    PROC_YIELD,
    PROC_CALL(PostActionSynchronize_End),
    PROC_END
};

bool PostActionSynchronize(ProcPtr parent)
{
    struct Unit * unit_act, * unit_tar;
    int debuff_act;

#if (defined(SID_Synchronize) && (COMMON_SKILL_VALID(SID_Synchronize)))
    if (!SkillTester(gActiveUnit, SID_Synchronize))
#else
    if (1)
#endif
        return false;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
        /**
         * If battle is not hitted, return
         */
        if (gBattleActorGlobalFlag.hitted == false)
            return false;

        break;

    case UNIT_ACTION_STAFF:
        if (GetSpellAssocCharCount(gBattleActor.weaponBefore) != 2)
            return false;

        break;

    case UNIT_ACTION_DANCE:
        break;

    default:
        return false;
    }

    unit_act = GetUnit(gActionData.subjectIndex);
    if (!UNIT_IS_VALID(unit_act) || (unit_act->state & (US_DEAD | US_BIT16)))
        return false;

    unit_tar = GetUnit(gActionData.targetIndex);
    if (!UNIT_IS_VALID(unit_tar) || (unit_tar->state & (US_DEAD | US_BIT16)))
        return false;

    /* actor should already hold a special status */
    debuff_act = GetUnitStatusIndex(unit_act);
    if (debuff_act == UNIT_STATUS_NONE)
        return false;

    /* Give ally positive status, give enemy debuff */
    if (AreUnitsAllied(unit_act->index, unit_tar->index) == IsDebuff(debuff_act))
        return false;

    Proc_StartBlocking(ProcScr_PostActionSynchronize, parent);

    MU_EndAll();
    HideUnitSprite(unit_act);
    ShowUnitSprite(unit_tar);
    return true;
}
