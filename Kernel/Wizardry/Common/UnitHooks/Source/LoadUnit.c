#include "common-chax.h"
#include "strmag.h"
#include "bwl.h"
#include "debuff.h"
#include "skill-system.h"

typedef int (*LoadUnitFunc_t)(struct Unit *unit, const struct CharacterData *character);
// extern const LoadUnitFunc_t gLoadUnitHooks[];
extern LoadUnitFunc_t const *const gpLoadUnitHooks;

void UnitLoadStatsFromChracterVanilla(struct Unit *unit, const struct CharacterData *character)
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

LYN_REPLACE_CHECK(UnitLoadStatsFromChracter);
void UnitLoadStatsFromChracter(struct Unit *unit, const struct CharacterData *character)
{
	const LoadUnitFunc_t *it;

	for (it = gpLoadUnitHooks; *it; it++)
		(*it)(unit, character);
}

// For now, no need to hack this funcion
// struct Unit *LoadUnit(const struct UnitDefinition *uDef)
