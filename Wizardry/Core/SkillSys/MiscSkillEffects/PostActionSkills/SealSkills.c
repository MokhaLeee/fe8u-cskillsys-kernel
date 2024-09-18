#include "common-chax.h"
#include "debuff.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "combat-art.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

void PostActionSealSkills(ProcPtr parent)
{
    FORCE_DECLARE struct Unit *unit = gActiveUnit;
    STATIC_DECLAR bool fullMetalBodyProc = false;

#if defined(SID_FullMetalBody) && (COMMON_SKILL_VALID(SID_FullMetalBody))
    if (SkillTester(GetUnit(gActionData.targetIndex), SID_FullMetalBody))
        fullMetalBodyProc = true;
#endif

    if (!fullMetalBodyProc)
    {
        switch (gActionData.unitActionType)
        {
        case UNIT_ACTION_COMBAT:
#if defined(SID_SealDefense) && (COMMON_SKILL_VALID(SID_SealDefense))
            if (SkillTester(unit, SID_SealDefense) && unit == GetUnit(gActionData.subjectIndex))
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_DEBUFF_SEAL_DEF);
            else if (SkillTester(unit, SID_SealDefense) && unit == GetUnit(gActionData.targetIndex))
                SetUnitStatDebuff(GetUnit(gActionData.subjectIndex), UNIT_STAT_DEBUFF_SEAL_DEF);
#endif

#if defined(SID_SealLuck) && (COMMON_SKILL_VALID(SID_SealLuck))
            if (SkillTester(unit, SID_SealLuck) && unit == GetUnit(gActionData.subjectIndex))
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_DEBUFF_SEAL_LCK);
            else if (SkillTester(unit, SID_SealLuck) && unit == GetUnit(gActionData.targetIndex))
                SetUnitStatDebuff(GetUnit(gActionData.subjectIndex), UNIT_STAT_DEBUFF_SEAL_LCK);
#endif

#if defined(SID_SealMagic) && (COMMON_SKILL_VALID(SID_SealMagic))
            if (SkillTester(unit, SID_SealMagic) && unit == GetUnit(gActionData.subjectIndex))
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_DEBUFF_SEAL_MAG);
            else if (SkillTester(unit, SID_SealMagic) && unit == GetUnit(gActionData.targetIndex))
                SetUnitStatDebuff(GetUnit(gActionData.subjectIndex), UNIT_STAT_DEBUFF_SEAL_MAG);
#endif

#if defined(SID_SealResistance) && (COMMON_SKILL_VALID(SID_SealResistance))
            if (SkillTester(unit, SID_SealResistance) && unit == GetUnit(gActionData.subjectIndex))
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_DEBUFF_SEAL_RES);
            else if (SkillTester(unit, SID_SealResistance) && unit == GetUnit(gActionData.targetIndex))
                SetUnitStatDebuff(GetUnit(gActionData.subjectIndex), UNIT_STAT_DEBUFF_SEAL_RES);
#endif

#if defined(SID_SealSkill) && (COMMON_SKILL_VALID(SID_SealSkill))
            if (SkillTester(unit, SID_SealSkill) && unit == GetUnit(gActionData.subjectIndex))
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_DEBUFF_SEAL_SKL);
            else if (SkillTester(unit, SID_SealSkill) && unit == GetUnit(gActionData.targetIndex))
                SetUnitStatDebuff(GetUnit(gActionData.subjectIndex), UNIT_STAT_DEBUFF_SEAL_SKL);
#endif

#if defined(SID_SealSpeed) && (COMMON_SKILL_VALID(SID_SealSpeed))
            if (SkillTester(unit, SID_SealSpeed) && unit == GetUnit(gActionData.subjectIndex))
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_DEBUFF_SEAL_SPD);
            else if (SkillTester(unit, SID_SealSpeed) && unit == GetUnit(gActionData.targetIndex))
                SetUnitStatDebuff(GetUnit(gActionData.subjectIndex), UNIT_STAT_DEBUFF_SEAL_SPD);
#endif

#if defined(SID_SealStrength) && (COMMON_SKILL_VALID(SID_SealStrength))
            if (SkillTester(unit, SID_SealStrength) && unit == GetUnit(gActionData.subjectIndex))
                SetUnitStatDebuff(GetUnit(gActionData.targetIndex), UNIT_STAT_DEBUFF_SEAL_POW);
            else if (SkillTester(unit, SID_SealStrength) && unit == GetUnit(gActionData.targetIndex))
                SetUnitStatDebuff(GetUnit(gActionData.subjectIndex), UNIT_STAT_DEBUFF_SEAL_POW);
#endif

        default:
            break;
        }
    }
}
