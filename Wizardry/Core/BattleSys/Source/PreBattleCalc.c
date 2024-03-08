#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "bmphase.h"

#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "debuff.h"
#include "chax-glb.h"
#include "combat-art.h"
#include "constants/skills.h"

typedef void (* PreBattleCalcFunc) (struct BattleUnit * buA, struct BattleUnit * buB);
void PreBattleCalcWeaponTriangle(struct BattleUnit * attacker, struct BattleUnit * defender);

/* LynJump */
void ComputeBattleUnitAttack(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int status;
    status = GetItemMight(attacker->weapon);

    if (IsUnitEffectiveAgainst(&attacker->unit, &defender->unit) || IsItemEffectiveAgainst(attacker->weapon, &defender->unit))
    {
        status = status * 2;

        if (SkillTester(&attacker->unit, SID_Resourceful))
            status = status * 2;
    }

    if (IsMagicAttack(attacker))
        status = status + UNIT_MAG(&attacker->unit);
    else
        status = status + attacker->unit.pow;

    attacker->battleAttack = status;
}

/* LynJump */
void ComputeBattleUnitDefense(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int status, def, res;
    struct Unit * unit = GetUnit(attacker->unit.index);

    def = attacker->unit.def + attacker->terrainDefense;
    res = attacker->unit.res + attacker->terrainResistance;

    if (IsMagicAttack(defender))
        status = res;
    else
        status = def;

    if (SkillTester(unit, SID_SorceryBlade))
        status = def < res ? def : res;

    attacker->battleDefense = status;
}

STATIC_DECLAR void PreBattleCalcInit(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    ComputeBattleUnitDefense(attacker, defender);
    ComputeBattleUnitAttack(attacker, defender);
    ComputeBattleUnitSpeed(attacker);
    ComputeBattleUnitHitRate(attacker);
    ComputeBattleUnitAvoidRate(attacker);
    ComputeBattleUnitCritRate(attacker);
    ComputeBattleUnitDodgeRate(attacker);
    ComputeBattleUnitSupportBonuses(attacker, defender);
    ComputeBattleUnitWeaponRankBonuses(attacker);
    ComputeBattleUnitStatusBonuses(attacker);

    /* Calc silencer rate at pre-battle-calc rather than battle-calc-real */
    attacker->battleSilencerRate = 0;
}

STATIC_DECLAR void PreBattleCalcEnd(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (attacker->battleAttack < 0)
        attacker->battleAttack = 0;

    if (attacker->battleDefense < 0)
        attacker->battleDefense = 0;

    if (attacker->battleSpeed < 0)
        attacker->battleSpeed = 0;

    if (attacker->battleHitRate < 0)
        attacker->battleHitRate = 0;

    if (attacker->battleAvoidRate < 0)
        attacker->battleAvoidRate = 0;

    if (attacker->battleCritRate < 0)
        attacker->battleCritRate = 0;

    if (attacker->battleDodgeRate < 0)
        attacker->battleDodgeRate = 0;

    /* Some special effects */

    /* RuinedBladePlus cannot crit attack */
    if (SkillTester(&attacker->unit, SID_RuinedBladePlus))
    {
        attacker->battleCritRate = 0;
        attacker->battleSilencerRate = 0;
    }

    /* If defender cannot get silencer */
    if (UNIT_CATTRIBUTES(&defender->unit) & CA_NEGATE_LETHALITY)
        attacker->battleSilencerRate = 0;

    if (attacker->battleSilencerRate < 0)
        attacker->battleSilencerRate = 0;
}

STATIC_DECLAR void PreBattleCalcSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    struct Unit * unit = GetUnit(attacker->unit.index);

    /* Defiant skills */
    if ((GetUnitCurrentHp(unit) * 4) < GetUnitMaxHp(unit))
    {
        if (SkillTester(unit, SID_DefiantCrit))
            attacker->battleCritRate += 50;

        if (SkillTester(unit, SID_DefiantAvoid))
            attacker->battleAvoidRate += 30;
    }

    /* Blow skills */
    if (attacker == &gBattleActor)
    {
        if (SkillTester(unit, SID_BlowDarting))
            attacker->battleSpeed += 6;

        if (SkillTester(unit, SID_BlowDeath))
        {
            if (!IsMagicAttack(attacker))
                attacker->battleAttack += 6;
        }

        if (SkillTester(unit, SID_BlowArmored))
        {
            if (!IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }

        if (SkillTester(unit, SID_BlowFiendish))
        {
            if (IsMagicAttack(attacker))
                attacker->battleAttack += 6;
        }

        if (SkillTester(unit, SID_BlowWarding))
        {
            if (IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }

        if (SkillTester(unit, SID_BlowDuelist))
           attacker->battleAvoidRate += 20;

        if (SkillTester(unit, SID_BlowUncanny))
            attacker->battleHitRate += 30;
    }

    /* Stance skills */
    if (attacker == &gBattleTarget)
    {
        if (SkillTester(unit, SID_StanceBracing))
            attacker->battleDefense += 4;

        if (SkillTester(unit, SID_StanceDarting))
            attacker->battleSpeed += 6;

        if (SkillTester(unit, SID_StanceFierce))
            attacker->battleAttack += 6;

        if (SkillTester(unit, SID_StanceKestrel))
        {
            attacker->battleAttack += 4;
            attacker->battleSpeed += 4;
        }

        if (SkillTester(unit, SID_StanceMirror))
        {
            attacker->battleAttack += 4;

            if (IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }

        if (SkillTester(unit, SID_StanceReady))
        {
            attacker->battleSpeed += 4;

            if (!IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }

        if (SkillTester(unit, SID_StanceSteady))
        {
            if (!IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }

        if (SkillTester(unit, SID_StanceSturdy))
        {
            attacker->battleAttack += 4;

            if (!IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }

        if (SkillTester(unit, SID_StanceSwift))
        {
            attacker->battleSpeed += 4;

            if (IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }

        if (SkillTester(unit, SID_StanceWarding))
        {
            if (IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
    }

    /* Misc */
    if (SkillTester(unit, SID_Lethality))
        attacker->battleSilencerRate += GetUnitSkill(unit);

    if (SkillTester(unit, SID_WatchfulEye))
        attacker->battleHitRate += 20;

    if (SkillTester(unit, SID_Crit))
        attacker->battleCritRate += 15;

    if (SkillTester(unit, SID_Avoid))
        attacker->battleAvoidRate += 10;

    /* wType related */
    switch (attacker->weaponType)
    {
    case ITYPE_SWORD:
        if (SkillTester(unit, SID_CritSword))
            attacker->battleCritRate += 10;

        if (SkillTester(unit, SID_FaireSword))
            attacker->battleAttack += 5;

        if (SkillTester(unit, SID_AvoidSword))
            attacker->battleAvoidRate += 20;
        break;

    case ITYPE_LANCE:
        if (SkillTester(unit, SID_CritLance))
            attacker->battleCritRate += 10;

        if (SkillTester(unit, SID_FaireLance))
            attacker->battleAttack += 5;
        break;

    case ITYPE_AXE:
        if (SkillTester(unit, SID_CritAxe))
            attacker->battleCritRate += 10;

        if (SkillTester(unit, SID_FaireAxe))
            attacker->battleAttack += 5;
        break;

    case ITYPE_BOW:
        if (SkillTester(unit, SID_CritBow))
            attacker->battleCritRate += 10;

        if (SkillTester(unit, SID_FaireBow))
            attacker->battleAttack += 5;
        break;

    case ITYPE_ANIMA:
    case ITYPE_LIGHT:
    case ITYPE_DARK:
        if (SkillTester(unit, SID_FaireBMag))
            attacker->battleAttack += 5;
        break;

    case ITYPE_STAFF:
    case ITYPE_BLLST:
    case ITYPE_ITEM:
    case ITYPE_DRAGN:
    default:
        break;
    }
}

STATIC_DECLAR void PreBattlePostCalcSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    struct Unit * unit = GetUnit(attacker->unit.index);

    if (attacker->battleAttack > defender->battleAttack)
    {
        if (SkillTester(unit, SID_HeavyBlade))
            attacker->battleCritRate += 15;

        if (SkillTester(unit, SID_HeavyBladePlus))
            attacker->battleCritRate += 25;
    }

    if (attacker->battleSpeed > defender->battleSpeed)
    {
        if (SkillTester(unit, SID_FlashingBlade))
            attacker->battleCritRate += 15;

        if (SkillTester(unit, SID_FlashingBladePlus))
            attacker->battleCritRate += 25;
    }

    if (SkillTester(unit, SID_BlueFlame))
        attacker->battleAttack += 2;
}

STATIC_DECLAR void PreBattlePostCalcRangeDebuffs(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    const struct Vec2 vec_range[24] = {
                                      { 0, -3},
                            {-1, -2}, { 0, -2}, { 1, -1},
                  {-2, -1}, {-1, -1}, { 0, -1}, { 1, -1}, { 2, -1},
        {-3,  0}, {-2,  0}, {-1,  0},           { 1,  0}, { 2,  0}, { 3,  0},
                  {-2,  1}, {-1,  1}, { 0,  1}, { 1,  1}, { 2,  1},
                            {-1,  2}, { 0,  2}, { 1,  2},
                                      { 0,  3}
    };

    const u8 range1[24] = {
                 0,
              0, 0, 0,
           0, 0, 1, 0, 0,
        0, 0, 1,    1, 0, 0,
           0, 0, 1, 0, 0,
              0, 0, 0,
                 0
    };

    const u8 range2[24] = {
                 0,
              0, 1, 0,
           0, 1, 1, 1, 0,
        0, 1, 1,    1, 1, 0,
           0, 1, 1, 1, 0,
              0, 1, 0,
                 0
    };

    const u8 range3[24] = {
                 1,
              1, 1, 1,
           1, 1, 1, 1, 1,
        1, 1, 1,    1, 1, 1,
           1, 1, 1, 1, 1,
              1, 1, 1,
                 1
    };

    u32 i, _x, _y;
    struct Unit * unit;

    int allies_range3 = 0;
    int enmies_range3 = 0;

    int enmies_range2 = 0;
    int allies_range1 = 0;

    for (i = 0; i < 24; i++)
    {
        _x = attacker->unit.xPos + vec_range[i].x;
        _y = attacker->unit.yPos + vec_range[i].y;

        unit = GetUnitAtPosition(_x, _y);
        if (!unit)
            continue;

        if (AreUnitsAllied(attacker->unit.index, unit->index))
        {
            /* Buffs */
            if (SkillTester(unit, SID_Bond) && range3[i] == 1)
            {
                attacker->battleHitRate += 10;
                attacker->battleAttack  += 2;
            }

            if (SkillTester(unit, SID_Charm) && range2[i] == 1)
                attacker->battleAttack  += 3;

            if (SkillTester(unit, SID_Inspiration) && range2[i] == 1)
            {
                attacker->battleAttack += 2;
                attacker->battleDefense += 2;
            }

            if (SkillTester(unit, SID_DivinelyInspiring) && range1[i] == 1)
            {
                attacker->battleAttack += 3;
                attacker->battleDefense += 1;
            }

            if (range3[i])
                allies_range3++;

            if (range1[i])
                allies_range1++;
        }
        else
        {
            /* Debuff */
            if (SkillTester(unit, SID_Anathema) && range3[i] == 1)
            {
                attacker->battleAvoidRate -= 10;
                attacker->battleDodgeRate -= 10;
            }

            if (SkillTester(unit, SID_Intimidate) && range2[i] == 1)
                attacker->battleAvoidRate -= 10;

            if (SkillTester(unit, SID_Hex) && range1[i] == 1)
                attacker->battleAvoidRate -= 10;

            if (SkillTester(unit, SID_VoiceOfPeace) && range2[i] == 1)
                attacker->battleAttack -= 2;

            if (range2[i])
                enmies_range2++;

            if (range3[i])
                enmies_range3++;
        }
    }

    unit = GetUnit(attacker->unit.index);

    if (allies_range3 != 0)
    {
        /* Todo */
    }
    else
    {
        if (SkillTester(unit, SID_Tantivy))
        {
            attacker->battleHitRate += 10;
            attacker->battleAvoidRate += 10;
        }

        if (SkillTester(unit, SID_Focus))
        {
            attacker->battleCritRate += 10;
        }
    }

    if (enmies_range3 != 0)
    {
        /* Todo */
    }
    else
    {
        /* Todo */
    }

    if (enmies_range2 >= 2)
    {
        if (SkillTester(unit, SID_Infiltrator))
        {
            attacker->battleAttack += 3;
            attacker->battleHitRate += 15;
        }
    }

    if (allies_range1 > 0)
    {
        if (SkillTester(&attacker->unit, SID_BlueFlame))
            attacker->battleAttack += 2;
    }
}

STATIC_DECLAR void PreBattleCalcSilencerRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (UNIT_CATTRIBUTES(&defender->unit) & CA_BOSS)
        attacker->battleSilencerRate -= 25;
}

STATIC_DECLAR const PreBattleCalcFunc PreBattleCalcFuncs[] = {
    PreBattleCalcInit,

    /* Usr defined funcs */
    PreBattleCalcSilencerRate,
    PreBattleCalcWeaponTriangle,
    PreBattleCalcSkills,
    PreBattleCalcLegendSkills,
    PreBattleCalcDebuffs,
    PreBattleCalcCombatArt,
    PreBattlePostCalcRangeDebuffs,

    PreBattlePostCalcSkills,
    PreBattleCalcEnd,
    NULL,
};

/* LynJump */
void ComputeBattleUnitStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    const PreBattleCalcFunc * it;
    for (it = PreBattleCalcFuncs; *it; it++)
        (*it)(attacker, defender);
}

/* LynJump */
void ComputeBattleUnitSilencerRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    return;
}

/* LynJump */
void ComputeBattleUnitEffectiveHitRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    attacker->battleEffectiveHitRate = attacker->battleHitRate - defender->battleAvoidRate;

    /* Distance +2, hit rate -20% */
    attacker->battleEffectiveHitRate -= Div(gBattleStats.range, 2) * 20;

    if (attacker->battleEffectiveHitRate > 100)
        attacker->battleEffectiveHitRate = 100;

    if (attacker->battleEffectiveHitRate < 0)
        attacker->battleEffectiveHitRate = 0;
}
