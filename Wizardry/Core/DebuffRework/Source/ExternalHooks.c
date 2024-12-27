#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(ComputeBattleUnitStatusBonuses);
void ComputeBattleUnitStatusBonuses(struct BattleUnit *bu)
{
    /**
     * Did nothing since now we put it in pre-battle calc
     */
    return;
}

void PreBattleCalcDebuffs(struct BattleUnit *bu, struct BattleUnit *defender)
{
    struct Unit *unit = GetUnit(bu->unit.index);
    int debuff = GetUnitStatusIndex(unit);
    const struct DebuffInfo *info = &gpDebuffInfos[debuff];

    bu->battleAttack += info->battle_status.atk;
    bu->battleDefense += info->battle_status.def;
    bu->battleHitRate += info->battle_status.hit;
    bu->battleAvoidRate += info->battle_status.avo;
    bu->battleCritRate += info->battle_status.crit;
    bu->battleSilencerRate += info->battle_status.silencer;
    bu->battleDodgeRate += info->battle_status.dodge;

#if (defined(SID_PsychUp) && (COMMON_SKILL_VALID(SID_PsychUp)))
    if (BattleFastSkillTester(bu, SID_PsychUp))
    {
        struct Unit *unit_enemy = GetUnit(defender->unit.index);
        int debuff = GetUnitStatusIndex(unit_enemy);
        const struct DebuffInfo *info_enemy = &gpDebuffInfos[debuff];

        bu->battleAttack += info_enemy->battle_status.atk;
        bu->battleDefense += info_enemy->battle_status.def;
        bu->battleHitRate += info_enemy->battle_status.hit;
        bu->battleAvoidRate += info_enemy->battle_status.avo;
        bu->battleCritRate += info_enemy->battle_status.crit;
        bu->battleSilencerRate += info_enemy->battle_status.silencer;
        bu->battleDodgeRate += info_enemy->battle_status.dodge;
    }
#endif
}

/* Unit status getter */
int PowGetterDebuff(int status, struct Unit *unit)
{
#if (defined(SID_HyperCutter) && (COMMON_SKILL_VALID(SID_HyperCutter)))
    if (SkillTester(unit, SID_HyperCutter))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.pow < 0)
            return status;
#endif

#if (defined(SID_ClearBody) && (COMMON_SKILL_VALID(SID_ClearBody)))
    if (SkillTester(unit, SID_ClearBody))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.pow < 0)
            return status;
#endif

    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.pow;
}

int MagGetterDebuff(int status, struct Unit *unit)
{
#if (defined(SID_ClearBody) && (COMMON_SKILL_VALID(SID_ClearBody)))
    if (SkillTester(unit, SID_ClearBody))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.mag < 0)
            return status;
#endif

    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.mag;
}

int SklGetterDebuff(int status, struct Unit *unit)
{
#if (defined(SID_ClearBody) && (COMMON_SKILL_VALID(SID_ClearBody)))
    if (SkillTester(unit, SID_ClearBody))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.skl < 0)
            return status;
#endif
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.skl;
    ;
}

int SpdGetterDebuff(int status, struct Unit *unit)
{
#if (defined(SID_ClearBody) && (COMMON_SKILL_VALID(SID_ClearBody)))
    if (SkillTester(unit, SID_ClearBody))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.spd < 0)
            return status;
#endif
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.spd;
}

int DefGetterDebuff(int status, struct Unit *unit)
{
#if (defined(SID_ClearBody) && (COMMON_SKILL_VALID(SID_ClearBody)))
    if (SkillTester(unit, SID_ClearBody))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.def < 0)
            return status;
#endif
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.def;
}

int ResGetterDebuff(int status, struct Unit *unit)
{
#if (defined(SID_ClearBody) && (COMMON_SKILL_VALID(SID_ClearBody)))
    if (SkillTester(unit, SID_ClearBody))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.res < 0)
            return status;
#endif
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.res;
}

int LckGetterDebuff(int status, struct Unit *unit)
{
#if (defined(SID_ClearBody) && (COMMON_SKILL_VALID(SID_ClearBody)))
    if (SkillTester(unit, SID_ClearBody))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.lck < 0)
            return status;
#endif
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.lck;
}

int MovGetterDebuff(int status, struct Unit *unit)
{
#if (defined(SID_ClearBody) && (COMMON_SKILL_VALID(SID_ClearBody)))
    if (SkillTester(unit, SID_ClearBody))
        if (gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.mov < 0)
            return status;
#endif
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.mov;
}
