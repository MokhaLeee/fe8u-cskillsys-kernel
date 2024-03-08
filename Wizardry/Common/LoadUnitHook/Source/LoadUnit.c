#include "global.h"
#include "bmunit.h"

#include "common-chax.h"
#include "strmag.h"
#include "bwl.h"
#include "skill-system.h"

STATIC_DECLAR void UnitLoadStatsFromChracterVanilla(struct Unit * unit, const struct CharacterData * character)
{
    int i;

    unit->maxHP = character->baseHP + unit->pClassData->baseHP;
    unit->pow   = character->basePow + unit->pClassData->basePow;
    unit->skl   = character->baseSkl + unit->pClassData->baseSkl;
    unit->spd   = character->baseSpd + unit->pClassData->baseSpd;
    unit->def   = character->baseDef + unit->pClassData->baseDef;
    unit->res   = character->baseRes + unit->pClassData->baseRes;
    unit->lck   = character->baseLck;

    unit->conBonus = 0;

    for (i = 0; i < 8; ++i) {
        unit->ranks[i] = unit->pClassData->baseRanks[i];

        if (unit->pCharacterData->baseRanks[i])
            unit->ranks[i] = unit->pCharacterData->baseRanks[i];
    }

    if (UNIT_FACTION(unit) == FACTION_BLUE && (unit->level != UNIT_LEVEL_MAX))
        unit->exp = 0;
    else
        unit->exp = UNIT_EXP_DISABLED;
}

STATIC_DECLAR void UnitLoadStatsFromCharacterMagic(struct Unit * unit, const struct CharacterData * character)
{
    UNIT_MAG(unit) = GetUnitBaseMagic(unit);
}

/* LynJump */
void UnitLoadStatsFromChracter(struct Unit * unit, const struct CharacterData * character)
{
    UnitLoadStatsFromChracterVanilla(unit, character);
    UnitLoadStatsFromCharacterMagic(unit, character);
    UnitAutoLoadSkills(unit);
    UnitHiddenLevelPreLoad(unit);
}

/* LynJump */
// For now, no need to hack this funcion
// struct Unit * LoadUnit(const struct UnitDefinition * uDef)
