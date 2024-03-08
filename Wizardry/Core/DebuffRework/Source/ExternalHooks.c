#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

#include "common-chax.h"
#include "debuff.h"

/* LynJump */
void ComputeBattleUnitStatusBonuses(struct BattleUnit * bu)
{
    /**
     * Did nothing since now we put it in pre-battle calc
     */
    return;
}

void PreBattleCalcDebuffs(struct BattleUnit * bu, struct BattleUnit * defender)
{
    struct Unit * unit = GetUnit(bu->unit.index);
    int debuff = GetUnitStatusIndex(unit);
    const struct DebuffInfo * info = &gpDebuffInfos[debuff];

    bu->battleAttack       += info->battle_status.atk;
    bu->battleDefense      += info->battle_status.def;
    bu->battleHitRate      += info->battle_status.hit;
    bu->battleAvoidRate    += info->battle_status.avo;
    bu->battleCritRate     += info->battle_status.crit;
    bu->battleSilencerRate += info->battle_status.silencer;
    bu->battleDodgeRate    += info->battle_status.dodge;
}

/* Unit status getter */
int PowGetterDebuff(int status, struct Unit * unit)
{
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.pow;
}

int MagGetterDebuff(int status, struct Unit * unit)
{
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.mag;
}

int SklGetterDebuff(int status, struct Unit * unit)
{
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.skl;
}

int SpdGetterDebuff(int status, struct Unit * unit)
{
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.spd;
}

int DefGetterDebuff(int status, struct Unit * unit)
{
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.def;
}

int ResGetterDebuff(int status, struct Unit * unit)
{
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.res;
}

int LckGetterDebuff(int status, struct Unit * unit)
{
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.lck;
}

int MovGetterDebuff(int status, struct Unit * unit)
{
    return status + gpDebuffInfos[GetUnitStatusIndex(unit)].unit_status.mov;
}
