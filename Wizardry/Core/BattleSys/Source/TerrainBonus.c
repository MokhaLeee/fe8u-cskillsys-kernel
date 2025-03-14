#include "common-chax.h"
#include "skill-system.h"
#include "class-types.h"
#include "constants/skills.h"

STATIC_DECLAR const struct ClassData *GetJInfoForTerrainBonus(struct Unit *unit)
{
	/**
	 * As it is just used for gActiveUnit, so here we can use SkillListTester to replace SkillTester
	 */
	if (CheckClassFlier(UNIT_CLASS_ID(unit))) {
#if (defined(SID_WingedShield) && (COMMON_SKILL_VALID(SID_WingedShield)))
		if (SkillListTester(unit, SID_WingedShield))
			return GetClassData(gpKernelClassList_Cavalry[0]);
#endif

#if (defined(SID_Perch) && (COMMON_SKILL_VALID(SID_Perch)))
		if (SkillListTester(unit, SID_Perch))
			return GetClassData(gpKernelClassList_Cavalry[0]);
#endif
	}

	return unit->pClassData;
}

LYN_REPLACE_CHECK(SetBattleUnitTerrainBonuses);
void SetBattleUnitTerrainBonuses(struct BattleUnit *bu, int terrain)
{
	const struct ClassData *jinfo = GetJInfoForTerrainBonus(&bu->unit);

	bu->terrainId = terrain;

	bu->terrainAvoid      = jinfo->pTerrainAvoidLookup[bu->terrainId];
	bu->terrainDefense    = jinfo->pTerrainDefenseLookup[bu->terrainId];
	bu->terrainResistance = jinfo->pTerrainResistanceLookup[bu->terrainId];
}

LYN_REPLACE_CHECK(SetBattleUnitTerrainBonusesAuto);
void SetBattleUnitTerrainBonusesAuto(struct BattleUnit *bu)
{
	SetBattleUnitTerrainBonuses(bu, gBmMapTerrain[bu->unit.yPos][bu->unit.xPos]);
}

LYN_REPLACE_CHECK(AiGetTerrainCombatPositionScoreComponent);
int AiGetTerrainCombatPositionScoreComponent(int x, int y)
{
	const struct ClassData *jinfo = GetJInfoForTerrainBonus(gActiveUnit);
	int terrainId = gBmMapTerrain[y][x];

	return jinfo->pTerrainAvoidLookup[terrainId]
		 + jinfo->pTerrainDefenseLookup[terrainId]
		 + jinfo->pTerrainResistanceLookup[terrainId];
}
