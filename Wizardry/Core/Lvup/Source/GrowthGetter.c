#include "common-chax.h"
#include "lvup.h"
#include "skill-system.h"
#include "strmag.h"
#include "constants/skills.h"
#include "bwl.h"

STATIC_DECLAR int GetUnitCommonGrowthBonus(int status, struct Unit *unit)
{
    int new = status;

#if defined(SID_Blossom) && (COMMON_SKILL_VALID(SID_Blossom))
    if (SkillTester(unit, SID_Blossom))
        new = new + status * 2;
#endif

#if defined(SID_Aptitude) && (COMMON_SKILL_VALID(SID_Aptitude))
    if (SkillTester(unit, SID_Aptitude))
        new = new + SKILL_EFF0(SID_Aptitude);
#endif

#if defined(SID_PeoplesKnight) && (COMMON_SKILL_VALID(SID_PeoplesKnight))
    if (SkillTester(unit, SID_PeoplesKnight))
    {
        struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(unit));

        for (int i = 0; i < 7; i++)
        {
            if (bwl->supports[i] >= 240)
                new += SKILL_EFF0(SID_PeoplesKnight) * 3;
            else if (bwl->supports[i] >= 160)
                new += SKILL_EFF0(SID_PeoplesKnight) * 2;
            else if (bwl->supports[i] >= 80)
                new += SKILL_EFF0(SID_PeoplesKnight) * 1;
        }
    }
#endif

/* This must come last */
#if defined(SID_NecroCopy) && (COMMON_SKILL_VALID(SID_NecroCopy))
    if (SkillTester(unit, SID_NecroCopy))
        new = 0;
#endif

    return new;
}

int GetUnitHpGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitHpGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->maxHP / 2;
#endif

#if defined(SID_KnackHealth) && (COMMON_SKILL_VALID(SID_KnackHealth))
    if (SkillTester(unit, SID_KnackHealth))
        status = status + SKILL_EFF0(SID_KnackHealth);
#endif

    return status;
}

int GetUnitPowGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitPowGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->pow;
#endif

#if defined(SID_KnackStrength) && (COMMON_SKILL_VALID(SID_KnackStrength))
    if (SkillTester(unit, SID_KnackStrength))
        status = status + SKILL_EFF0(SID_KnackStrength);
#endif

    return status;
}

int GetUnitMagGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitMagGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + GetUnitMagic(unit);
#endif

#if defined(SID_KnackMagic) && (COMMON_SKILL_VALID(SID_KnackMagic))
    if (SkillTester(unit, SID_KnackMagic))
        status = status + SKILL_EFF0(SID_KnackMagic);
#endif

    return status;
}

int GetUnitSklGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitSklGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->skl;
#endif

#if defined(SID_KnackSkill) && (COMMON_SKILL_VALID(SID_KnackSkill))
    if (SkillTester(unit, SID_KnackSkill))
        status = status + SKILL_EFF0(SID_KnackSkill);
#endif

    return status;
}

int GetUnitSpdGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitSpdGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->spd;
#endif

#if defined(SID_KnackSpeed) && (COMMON_SKILL_VALID(SID_KnackSpeed))
    if (SkillTester(unit, SID_KnackSpeed))
        status = status + SKILL_EFF0(SID_KnackSpeed);
#endif

    return status;
}

int GetUnitLckGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitLckGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->lck;
#endif

#if defined(SID_KnackLuck) && (COMMON_SKILL_VALID(SID_KnackLuck))
    if (SkillTester(unit, SID_KnackLuck))
        status = status + SKILL_EFF0(SID_KnackLuck);
#endif

    return status;
}

int GetUnitDefGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitDefGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->def;
#endif

#if defined(SID_KnackDefense) && (COMMON_SKILL_VALID(SID_KnackDefense))
    if (SkillTester(unit, SID_KnackDefense))
        status = status + SKILL_EFF0(SID_KnackDefense);
#endif

    return status;
}

int GetUnitResGrowthBonus(int status, struct Unit *unit)
{
	status = GetUnitCommonGrowthBonus(status, unit);
	status = GetUnitResGrowthJobBonus(status, unit);

#if defined(SID_DarkHorse) && (COMMON_SKILL_VALID(SID_DarkHorse))
    if (SkillTester(unit, SID_DarkHorse))
        status = status + unit->def;
#endif

#if defined(SID_KnackResistance) && (COMMON_SKILL_VALID(SID_KnackResistance))
    if (SkillTester(unit, SID_KnackResistance))
        status = status + SKILL_EFF0(SID_KnackResistance);
#endif

    return status;
}

/* Person based growth */
int GetUnitHpGrowth(struct Unit *unit)  { return GetUnitHpGrowthBonus(unit->pCharacterData->growthHP, unit); }
int GetUnitPowGrowth(struct Unit *unit) { return GetUnitPowGrowthBonus(unit->pCharacterData->growthPow, unit); }
int GetUnitMagGrowth(struct Unit *unit) { return GetUnitMagGrowthBonus(GetUnitBasicMagGrowth(unit), unit); }
int GetUnitSklGrowth(struct Unit *unit) { return GetUnitSklGrowthBonus(unit->pCharacterData->growthSkl, unit); }
int GetUnitSpdGrowth(struct Unit *unit) { return GetUnitSpdGrowthBonus(unit->pCharacterData->growthSpd, unit); }
int GetUnitLckGrowth(struct Unit *unit) { return GetUnitLckGrowthBonus(unit->pCharacterData->growthLck, unit); }
int GetUnitDefGrowth(struct Unit *unit) { return GetUnitDefGrowthBonus(unit->pCharacterData->growthDef, unit); }
int GetUnitResGrowth(struct Unit *unit) { return GetUnitResGrowthBonus(unit->pCharacterData->growthRes, unit); }

/* Job based growth */
int GetUnitJobBasedHpGrowth(struct Unit *unit)  { return GetUnitHpGrowthBonus(unit->pClassData->growthHP, unit); }
int GetUnitJobBasedPowGrowth(struct Unit *unit) { return GetUnitPowGrowthBonus(unit->pClassData->growthPow, unit); }
int GetUnitJobBasedMagGrowth(struct Unit *unit) { return GetUnitMagGrowthBonus(GetUnitJobBasedBasicMagGrowth(unit), unit); }
int GetUnitJobBasedSklGrowth(struct Unit *unit) { return GetUnitSklGrowthBonus(unit->pClassData->growthSkl, unit); }
int GetUnitJobBasedSpdGrowth(struct Unit *unit) { return GetUnitSpdGrowthBonus(unit->pClassData->growthSpd, unit); }
int GetUnitJobBasedLckGrowth(struct Unit *unit) { return GetUnitLckGrowthBonus(unit->pClassData->growthLck, unit); }
int GetUnitJobBasedDefGrowth(struct Unit *unit) { return GetUnitDefGrowthBonus(unit->pClassData->growthDef, unit); }
int GetUnitJobBasedResGrowth(struct Unit *unit) { return GetUnitResGrowthBonus(unit->pClassData->growthRes, unit); }