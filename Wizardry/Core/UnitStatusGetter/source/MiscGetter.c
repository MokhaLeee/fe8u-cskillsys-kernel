#include "common-chax.h"
#include "skill-system.h"
#include "status-getter.h"
#include "constants/skills.h"

int StatusGetterCheckCpas(int status, struct Unit *unit)
{
    LIMIT_AREA(status, 0, 127);
    return status;
}

LYN_REPLACE_CHECK(GetUnitMagBy2Range);
int GetUnitMagBy2Range(struct Unit *unit)
{
    int range;

    if (unit->pCharacterData->number == CHARACTER_FOMORTIIS)
        return GetItemMaxRange(ITEM_NIGHTMARE);

    range = MagGetter(unit) / 2;

    if (range < 5)
        range = 5;

    return range;
}

LYN_REPLACE_CHECK(GetUnitCurrentHp);
int GetUnitCurrentHp(struct Unit *unit)
{
    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);

    return unit->curHP;
}

LYN_REPLACE_CHECK(IsPositionMagicSealed);
s8 IsPositionMagicSealed(int x, int y)
{
    int i;

    /**
     * Decomp set i to 0x81 initially, but we're now making
     * magic seal a skill, so we have to check player units too
     */
    for (i = 0x1; i < 0xC0; ++i)
    {
        struct Unit *unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (!(UNIT_CATTRIBUTES(unit) & CA_MAGICSEAL))
        {
#if defined(SID_MagicSeal) && (COMMON_SKILL_VALID(SID_MagicSeal))
            if (!SkillTester(unit, SID_MagicSeal))
                continue;
#else 
            continue;
#endif
        }

        if (RECT_DISTANCE(unit->xPos, unit->yPos, x, y) <= 10)
            return TRUE;
    }

    return FALSE;
}

LYN_REPLACE_CHECK(IsUnitMagicSealed);
s8 IsUnitMagicSealed(struct Unit *unit)
{
    if (unit->statusIndex == UNIT_STATUS_SILENCED)
        return TRUE;

    if (IsPositionMagicSealed(unit->xPos, unit->yPos))
        return TRUE;

    return FALSE;
}

LYN_REPLACE_CHECK(GetUnitPortraitId);
inline int GetUnitPortraitId(struct Unit *unit)
{

#if (defined(SID_IdentityProblems) && (COMMON_SKILL_VALID(SID_IdentityProblems)))
    if (SkillTester(unit, SID_IdentityProblems))
        return NextRN_N(100);
#endif

    if (unit->pCharacterData->portraitId)
    {
        // TODO: PORTRAIT_LYON?, CHAPTER definitions
        if (gPlaySt.chapterIndex == 0x22 && unit->pCharacterData->portraitId == 0x4A)
            return 0x46;

        return unit->pCharacterData->portraitId;
    }

    if (unit->pClassData->defaultPortraitId)
        return unit->pClassData->defaultPortraitId;

    return 0;
}

LYN_REPLACE_CHECK(GetUnitMiniPortraitId);
inline int GetUnitMiniPortraitId(struct Unit *unit)
{
    FORCE_DECLARE int miniMugId = 0;

#if (defined(SID_IdentityProblems) && (COMMON_SKILL_VALID(SID_IdentityProblems)))
    if (SkillTester(unit, SID_IdentityProblems))
        do
        {
            miniMugId = NextRN_N(100);
        } while (miniMugId == 0);
#endif

    if (unit->pCharacterData->miniPortrait)
        return 0x7F00 + unit->pCharacterData->miniPortrait;

    return GetUnitPortraitId(unit);
}