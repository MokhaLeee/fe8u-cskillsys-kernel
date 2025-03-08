#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "combat-art.h"
#include "class-types.h"
#include "constants/skills.h"

void ClearBattleGlobalFlags(void)
{
    Assert(sizeof(gBattleActorGlobalFlag) <= 0x10);
    Assert(sizeof(gBattleTargetGlobalFlag) <= 0x10);

    CpuFill16(0, &gBattleActorGlobalFlag, sizeof(gBattleActorGlobalFlag));
    CpuFill16(0, &gBattleTargetGlobalFlag, sizeof(gBattleTargetGlobalFlag));
}

void RegisterHitCnt(struct BattleUnit * bu, bool miss)
{
    struct BattleGlobalFlags * flags;

    if (!(gBattleStats.config & BATTLE_CONFIG_REAL))
        return;

    flags = GetBattleGlobalFlags(bu);
    flags->round_cnt++;
    if (!miss)
    {
        flags->hitted = true;
        flags->round_cnt_hit++;
    }
    else
        flags->round_cnt_avo++;
}

void ResetRoundEfxSkills(void)
{
    memset(&sEfxSkillQueue, 0, sizeof(sEfxSkillQueue));
}

void EnqueueRoundEfxSkill(u16 sid)
{
    if (sEfxSkillQueue.max < (sizeof(sEfxSkillQueue.skill_pool) - 1))
        sEfxSkillQueue.skill_pool[sEfxSkillQueue.max++] = sid;
}

u16 DequeueRoundEfxSkill(void)
{
    if (sEfxSkillQueue.cur < sEfxSkillQueue.max)
        return sEfxSkillQueue.skill_pool[sEfxSkillQueue.cur++];

    return 0;
}

LYN_REPLACE_CHECK(SetBattleUnitTerrainBonuses);
void SetBattleUnitTerrainBonuses(struct BattleUnit* bu, int terrain) {
    bu->terrainId = terrain;

#if (defined(SID_Perch) && (COMMON_SKILL_VALID(SID_Perch)))
    if (BattleSkillTester(bu, SID_Perch))
    {
        bu->terrainAvoid      = TerrainTable_Avo_Common[bu->terrainId];
        bu->terrainDefense    = TerrainTable_Def_Common[bu->terrainId];
        bu->terrainResistance = TerrainTable_Res_Common[bu->terrainId];
    }
    else
    {
        bu->terrainAvoid      = bu->unit.pClassData->pTerrainAvoidLookup[bu->terrainId];
        bu->terrainDefense    = bu->unit.pClassData->pTerrainDefenseLookup[bu->terrainId];
        bu->terrainResistance = bu->unit.pClassData->pTerrainResistanceLookup[bu->terrainId];
    }

#else
    bu->terrainAvoid      = bu->unit.pClassData->pTerrainAvoidLookup[bu->terrainId];
    bu->terrainDefense    = bu->unit.pClassData->pTerrainDefenseLookup[bu->terrainId];
    bu->terrainResistance = bu->unit.pClassData->pTerrainResistanceLookup[bu->terrainId];
#endif


#if (defined(SID_Camouflage) && (COMMON_SKILL_VALID(SID_Camouflage)))
    if (BattleSkillTester(bu, SID_Camouflage))
        bu->terrainAvoid = (bu->terrainAvoid * 3)/2;
#endif
}

LYN_REPLACE_CHECK(SetBattleUnitTerrainBonusesAuto);
void SetBattleUnitTerrainBonusesAuto(struct BattleUnit* bu) {
    bu->terrainId = gBmMapTerrain[bu->unit.yPos][bu->unit.xPos];

#if (defined(SID_Perch) && (COMMON_SKILL_VALID(SID_Perch)))
    if (BattleSkillTester(bu, SID_Perch))
    {
        bu->terrainAvoid      = TerrainTable_Avo_Common[bu->terrainId];
        bu->terrainDefense    = TerrainTable_Def_Common[bu->terrainId];
        bu->terrainResistance = TerrainTable_Res_Common[bu->terrainId];
    }
    else
    {
        bu->terrainAvoid      = bu->unit.pClassData->pTerrainAvoidLookup[bu->terrainId];
        bu->terrainDefense    = bu->unit.pClassData->pTerrainDefenseLookup[bu->terrainId];
        bu->terrainResistance = bu->unit.pClassData->pTerrainResistanceLookup[bu->terrainId];
    }

#else
    bu->terrainAvoid      = bu->unit.pClassData->pTerrainAvoidLookup[bu->terrainId];
    bu->terrainDefense    = bu->unit.pClassData->pTerrainDefenseLookup[bu->terrainId];
    bu->terrainResistance = bu->unit.pClassData->pTerrainResistanceLookup[bu->terrainId];
#endif

#if (defined(SID_Camouflage) && (COMMON_SKILL_VALID(SID_Camouflage)))
    if (BattleSkillTester(bu, SID_Camouflage))
        bu->terrainAvoid = (bu->terrainAvoid * 3)/2;
#endif
}
