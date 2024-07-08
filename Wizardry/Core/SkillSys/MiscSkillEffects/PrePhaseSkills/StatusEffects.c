#include "common-chax.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "constants/skills.h"
#include "debuff.h"

struct ProcPrePhaseBoon {
    PROC_HEADER;

    u8 uid;
};

STATIC_DECLAR void PrePhaseBoon_FindNextCharacter(struct ProcPrePhaseBoon * proc)
{
    while (++proc->uid < (gPlaySt.faction + 0x40))
    {
        struct Unit * unit = GetUnit(proc->uid);
        if (!UNIT_ALIVE(unit))
            continue;

#if defined(SID_Boon) && (COMMON_SKILL_VALID(SID_Boon))
        if (!SkillTester(unit, SID_Boon))
        {
            int i;
            struct StatDebuffStatus * sdebuff = GetUnitStatDebuffStatus(unit);

            bool negative_status = IsDebuff(GetUnitStatusIndex(unit));
            bool negative_statdebuff = false;

            for (i = UNIT_STAT_DEBUFF_IDX_START; i < UNIT_STAT_DEBUFF_MAX; i++)
            {
                if (!_BIT_CHK(sdebuff->st.bitmask, i))
                    continue;

                if (gpStatDebuffInfos[i].positive_type == STATUS_DEBUFF_NEGATIVE)
                {
                    negative_statdebuff = true;
                    break;
                }
            }

            if (negative_status || negative_statdebuff)
            {
                EnsureCameraOntoPosition(proc, unit->xPos, unit->yPos);
                return;
            }
        }
#endif
    }
    Proc_Goto(proc, 99);
}

STATIC_DECLAR void PrePhaseBoon_ExecAnim(struct ProcPrePhaseBoon * proc)
{
    MU_EndAll();
    StartStatusHealEffect(GetUnit(proc->uid), proc);
}

STATIC_DECLAR void PrePhaseBoon_ClearStatus(struct ProcPrePhaseBoon * proc)
{
    int i;
    struct Unit * unit = GetUnit(proc->uid);
    struct StatDebuffStatus * sdebuff = GetUnitStatDebuffStatus(unit);

    if (IsDebuff(GetUnitStatusIndex(unit)))
        SetUnitStatus(unit, UNIT_STATUS_NONE);

    for (i = UNIT_STAT_DEBUFF_IDX_START; i < UNIT_STAT_DEBUFF_MAX; i++)
    {
        if (!_BIT_CHK(sdebuff->st.bitmask, i))
            continue;

        if (gpStatDebuffInfos[i].positive_type == STATUS_DEBUFF_NEGATIVE)
            _BIT_CLR(sdebuff->st.bitmask, i);
    }
}

STATIC_DECLAR const struct ProcCmd ProcScr_PrePhaseBoon[] = {
    PROC_YIELD,

PROC_LABEL(1),
    PROC_CALL(PrePhaseBoon_FindNextCharacter),
    PROC_YIELD,
    PROC_CALL(PrePhaseBoon_ExecAnim),
    PROC_YIELD,
    PROC_CALL(PrePhaseBoon_ClearStatus),
    PROC_GOTO(1),

PROC_LABEL(99),
    PROC_END
};

bool PrePhase_UnitStatusEffectSkills(ProcPtr parent)
{
    FORCE_DECLARE struct ProcPrePhaseBoon * proc;

#if defined(SID_Boon) && (COMMON_SKILL_VALID(SID_Boon))
    proc = Proc_StartBlocking(ProcScr_PrePhaseBoon, parent);
    proc->uid = gPlaySt.faction;
    return true;
#endif

    return false;
}
