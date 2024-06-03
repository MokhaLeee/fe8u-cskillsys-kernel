#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"
#include "class-types.h"

#define NEGLECT_RANGE_DEBUFF_CALC_NOT_REAL 0

typedef void (* PreBattleCalcFunc) (struct BattleUnit * buA, struct BattleUnit * buB);
extern PreBattleCalcFunc const * const gpPreBattleCalcFuncs;
void PreBattleCalcWeaponTriangle(struct BattleUnit * attacker, struct BattleUnit * defender);

/* LynJump */
void ComputeBattleUnitAttack(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int status;
    status = GetItemMight(attacker->weapon);

    if (IsUnitEffectiveAgainst(&attacker->unit, &defender->unit) || IsItemEffectiveAgainst(attacker->weapon, &defender->unit))
    {
        status = status * 2;

#if (defined(SID_Resourceful) && (SID_Resourceful < MAX_SKILL_NUM))
        if (SkillTester(&attacker->unit, SID_Resourceful))
            status = status * 2;
#endif
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

void PreBattleCalcInit(struct BattleUnit * attacker, struct BattleUnit * defender)
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

void PreBattleCalcEnd(struct BattleUnit * attacker, struct BattleUnit * defender)
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

#if (defined(SID_RuinedBladePlus) && (SID_RuinedBladePlus < MAX_SKILL_NUM))
    /* RuinedBladePlus cannot crit attack */
    if (SkillTester(&attacker->unit, SID_RuinedBladePlus))
    {
        attacker->battleCritRate = 0;
        attacker->battleSilencerRate = 0;
    }
#endif

    /* If defender cannot get silencer */
    if (UNIT_CATTRIBUTES(&defender->unit) & CA_NEGATE_LETHALITY)
        attacker->battleSilencerRate = 0;

    if (attacker->battleSilencerRate < 0)
        attacker->battleSilencerRate = 0;
}

void PreBattleCalcSkills(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    struct Unit * unit = GetUnit(attacker->unit.index);

    /* Defiant skills */
    if ((GetUnitCurrentHp(unit) * 4) < GetUnitMaxHp(unit))
    {
#if (defined(SID_DefiantCrit) && (SID_DefiantCrit < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_DefiantCrit))
            attacker->battleCritRate += 50;
#endif

#if (defined(SID_DefiantAvoid) && (SID_DefiantAvoid < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_DefiantAvoid))
            attacker->battleAvoidRate += 30;
#endif
    }

    /* Blow skills */
    if (attacker == &gBattleActor)
    {
#if (defined(SID_BlowDarting) && (SID_BlowDarting < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_BlowDarting))
            attacker->battleSpeed += 6;
#endif

#if (defined(SID_BlowDeath) && (SID_BlowDeath < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_BlowDeath))
        {
            if (!IsMagicAttack(attacker))
                attacker->battleAttack += 6;
        }
#endif

#if (defined(SID_BlowArmored) && (SID_BlowArmored < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_BlowArmored))
        {
            if (!IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
#endif

#if (defined(SID_BlowFiendish) && (SID_BlowFiendish < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_BlowFiendish))
        {
            if (IsMagicAttack(attacker))
                attacker->battleAttack += 6;
        }
#endif

#if (defined(SID_BlowWarding) && (SID_BlowWarding < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_BlowWarding))
        {
            if (IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
#endif

#if (defined(SID_BlowDuelist) && (SID_BlowDuelist < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_BlowDuelist))
           attacker->battleAvoidRate += 20;
#endif

#if (defined(SID_BlowUncanny) && (SID_BlowUncanny < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_BlowUncanny))
            attacker->battleHitRate += 30;
#endif
    }

    /* Stance skills */
    if (attacker == &gBattleTarget)
    {
#if (defined(SID_StanceBracing) && (SID_StanceBracing < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceBracing))
            attacker->battleDefense += 4;
#endif

#if (defined(SID_StanceDarting) && (SID_StanceDarting < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceDarting))
            attacker->battleSpeed += 6;
#endif

#if (defined(SID_StanceFierce) && (SID_StanceFierce < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceFierce))
            attacker->battleAttack += 6;
#endif

#if (defined(SID_StanceKestrel) && (SID_StanceKestrel < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceKestrel))
        {
            attacker->battleAttack += 4;
            attacker->battleSpeed += 4;
        }
#endif

#if (defined(SID_StanceMirror) && (SID_StanceMirror < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceMirror))
        {
            attacker->battleAttack += 4;

            if (IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }
#endif

#if (defined(SID_StanceReady) && (SID_StanceReady < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceReady))
        {
            attacker->battleSpeed += 4;

            if (!IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }
#endif

#if (defined(SID_StanceSteady) && (SID_StanceSteady < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceSteady))
        {
            if (!IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
#endif

#if (defined(SID_StanceSturdy) && (SID_StanceSturdy < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceSturdy))
        {
            attacker->battleAttack += 4;

            if (!IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }
#endif

#if (defined(SID_StanceSwift) && (SID_StanceSwift < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceSwift))
        {
            attacker->battleSpeed += 4;

            if (IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }
#endif

#if (defined(SID_StanceWarding) && (SID_StanceWarding < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StanceWarding))
        {
            if (IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
#endif

#if (defined(SID_StanceSpectrum) && (SID_StanceSpectrum < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_StanceSpectrum))
    {
            attacker->battleAttack += 2;
            attacker->battleSpeed += 2;
            attacker->battleDefense += 2;
    }
#endif
    }

    /* Misc */
#if (defined(SID_Lethality) && (SID_Lethality < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Lethality))
        attacker->battleSilencerRate += GetUnitSkill(unit);
#endif

#if (defined(SID_WatchfulEye) && (SID_WatchfulEye < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_WatchfulEye))
        attacker->battleHitRate += 20;
#endif

#if (defined(SID_Crit) && (SID_Crit < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Crit))
        attacker->battleCritRate += 15;
#endif

#if (defined(SID_Avoid) && (SID_Avoid < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Avoid))
        attacker->battleAvoidRate += 10;
#endif

    /* wType related */
    switch (attacker->weaponType)
    {
    case ITYPE_SWORD:
#if (defined(SID_CritSword) && (SID_CritSword < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_CritSword))
            attacker->battleCritRate += 10;
#endif

#if (defined(SID_FaireSword) && (SID_FaireSword < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FaireSword))
            attacker->battleAttack += 5;
#endif

#if (defined(SID_AvoidSword) && (SID_AvoidSword < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_AvoidSword))
            attacker->battleAvoidRate += 20;
#endif
        break;

    case ITYPE_LANCE:
#if (defined(SID_CritLance) && (SID_CritLance < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_CritLance))
            attacker->battleCritRate += 10;
#endif

#if (defined(SID_FaireLance) && (SID_FaireLance < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FaireLance))
            attacker->battleAttack += 5;
#endif
        break;

    case ITYPE_AXE:
#if (defined(SID_CritAxe) && (SID_CritAxe < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_CritAxe))
            attacker->battleCritRate += 10;
#endif

#if (defined(SID_FaireAxe) && (SID_FaireAxe < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FaireAxe))
            attacker->battleAttack += 5;
#endif
        break;

    case ITYPE_BOW:
#if (defined(SID_CritBow) && (SID_CritBow < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_CritBow))
            attacker->battleCritRate += 10;
#endif

#if (defined(SID_FaireBow) && (SID_FaireBow < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FaireBow))
            attacker->battleAttack += 5;
#endif
        break;

    case ITYPE_ANIMA:
    case ITYPE_LIGHT:
    case ITYPE_DARK:
#if (defined(SID_FaireBMag) && (SID_FaireBMag < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FaireBMag))
            attacker->battleAttack += 5;
#endif
        break;

    case ITYPE_STAFF:
    case ITYPE_BLLST:
    case ITYPE_ITEM:
    case ITYPE_DRAGN:
    default:
        break;
    }

#if (defined(SID_BlueFlame) && (SID_BlueFlame < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_BlueFlame))
        attacker->battleAttack += 2;
#endif

#if (defined(SID_Frenzy) && (SID_Frenzy < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Frenzy))
    {
        // Check if their current HP is at least 4 points less than their max HP
        if ((GetUnitMaxHp(unit) - GetUnitCurrentHp(unit)) >= 4)
        {
            /**
             * Find the difference between the two
             * Divide by 4
             * Add the resulting total to the unit's attack
             */
            attacker->battleAttack += (GetUnitMaxHp(unit) - GetUnitCurrentHp(unit)) / 4;
        }
    }
#endif

#if (defined(SID_WonderGuard) && (SID_WonderGuard < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_WonderGuard))
    {
        // check if the attacker and defender have the same weapon type
        if (defender->weaponType == attacker->weaponType) 
            /**
             * if so, then increase the attacker's defense by the
             * defender's attack to render the latter's attack harmless 
             */
            attacker->battleDefense = INT16_MAX;
    }
#endif

#if (defined(SID_Merciless) && (SID_Merciless < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Merciless))
    {
        // Check if the defending unit has the poison status
        if (GetUnitStatusIndex(&defender->unit) == UNIT_STATUS_POISON)
            // If so, then set an arbitrary high value for crit to 'gurantee' it.
            attacker->battleCritRate = 255;
    }
#endif

#if (defined(SID_CriticalPierce) && (SID_CriticalPierce < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_CriticalPierce))
        // Add the defender's critical avoid rate to the attacker's.
        // Thus effectively 'negating' it.
        attacker->battleCritRate += defender->battleDodgeRate;
#endif

#if (defined(SID_KillingMachine) && (SID_KillingMachine < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_KillingMachine))
        attacker->battleCritRate *= 2;
#endif

#if (defined(SID_HeavyStrikes) && (SID_HeavyStrikes < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_HeavyStrikes))
        attacker->battleCritRate += GetItemWeight(attacker->weapon);
#endif

#if (defined(SID_QuickBurn) && (SID_QuickBurn < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_QuickBurn))
    {
        int turnNumber;

        if(gPlaySt.chapterTurnNumber > 15)
        {
            turnNumber = 16;
        }
        else
        {
            turnNumber = gPlaySt.chapterTurnNumber;
        }

        attacker->battleHitRate += (15 - (turnNumber-1));
        attacker->battleAvoidRate += (15 - (turnNumber-1));
    }
#endif

#if (defined(SID_SlowBurn) && (SID_SlowBurn < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_SlowBurn))
    {
        int turnNumber;

        if (gPlaySt.chapterTurnNumber > 15) 
        {
            turnNumber = 15;
        }
        else 
        {
            turnNumber = gPlaySt.chapterTurnNumber;
        }

        attacker->battleHitRate += turnNumber;
        attacker->battleAvoidRate += turnNumber;
    }
#endif

#if (defined(SID_Technician) && (SID_Technician < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Technician))
    {
        if (GetItemRequiredExp(attacker->weapon) < WPN_EXP_D)
        {
            int itemMight = GetItemMight(attacker->weapon);
            attacker->battleAttack += itemMight / 2;
        }
    }
#endif

}

void PreBattle_CalcSkillsOnEnd(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    /**
     * Here we need to put some calculation at the end of the pre-battle calc.
     * Thus the main part of calc should be positioned at berfore.
     */
    struct Unit * unit = GetUnit(attacker->unit.index);

#if (defined(SID_CatchingUp) && (SID_CatchingUp < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_CatchingUp))
        {
            /**
             * Check if the enemy unit doubles the skill holder
             * if there's any additional speed above the doubling threshold
             * add that to the skillholder's attack
             */
            if((defender->battleSpeed - attacker->battleSpeed) > BATTLE_FOLLOWUP_SPEED_THRESHOLD)
                attacker->battleAttack += (defender->battleSpeed - attacker->battleSpeed);
        }
#endif

    if (attacker->battleAttack > defender->battleAttack)
    {
#if (defined(SID_HeavyBlade) && (SID_HeavyBlade < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_HeavyBlade))
            attacker->battleCritRate += 15;
#endif

#if (defined(SID_HeavyBladePlus) && (SID_HeavyBladePlus < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_HeavyBladePlus))
            attacker->battleCritRate += 25;
#endif
    }

    if (attacker->battleSpeed > defender->battleSpeed)
    {
#if (defined(SID_FlashingBlade) && (SID_FlashingBlade < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FlashingBlade))
            attacker->battleCritRate += 15;
#endif

#if (defined(SID_FlashingBladePlus) && (SID_FlashingBladePlus < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FlashingBladePlus))
            attacker->battleCritRate += 25;
#endif
    }
#if (defined(SID_Skyguard) && (SID_Skyguard < MAX_SKILL_NUM))
         if (SkillTester(unit, SID_Skyguard) && CheckClassFlier(UNIT_CLASS_ID(&defender->unit)))
         {
             const struct Vec2 vec_range[24] = {
                                      { 0, -3},
                            {-1, -2}, { 0, -2}, { 1, -1},
                  {-2, -1}, {-1, -1}, { 0, -1}, { 1, -1}, { 2, -1},
        {-3,  0}, {-2,  0}, {-1,  0},           { 1,  0}, { 2,  0}, { 3,  0},
                  {-2,  1}, {-1,  1}, { 0,  1}, { 1,  1}, { 2,  1},
                            {-1,  2}, { 0,  2}, { 1,  2},
                                      { 0,  3}};
            /*Search the presence of allied Sky unit.
              Could probably be optimised with a Binary search */
              u32 i, x, y;
              for (i = 0; i < 24; i++)
              {
                  struct Unit * unit_to_check;
                  x = attacker->unit.xPos + vec_range[i].x;
                  y = attacker->unit.yPos + vec_range[i].y;
                  unit_to_check = GetUnitAtPosition(x, y);
                  if (AreUnitsAllied(attacker->unit.index, unit_to_check->index) && CheckClassFlier(UNIT_CLASS_ID(unit_to_check)))
                  {
                      attacker->battleAttack += 4;
                      break;
                  }
              }
        }
#endif

#if (defined(SID_Horseguard) && (SID_Horseguard < MAX_SKILL_NUM))
         if (SkillTester(unit, SID_Horseguard) && CheckClassCavalry(UNIT_CLASS_ID(&defender->unit)))
         {
             const struct Vec2 vec_range[24] = {
                                      { 0, -3},
                            {-1, -2}, { 0, -2}, { 1, -1},
                  {-2, -1}, {-1, -1}, { 0, -1}, { 1, -1}, { 2, -1},
        {-3,  0}, {-2,  0}, {-1,  0},           { 1,  0}, { 2,  0}, { 3,  0},
                  {-2,  1}, {-1,  1}, { 0,  1}, { 1,  1}, { 2,  1},
                            {-1,  2}, { 0,  2}, { 1,  2},
                                      { 0,  3}};
            /*Search the presence of allied Cavalry unit.
              Could probably be optimised with a Binary search */
              u32 i, x, y;
              for (i = 0; i < 24; i++)
              {
                  struct Unit * unit_to_check;
                  x = attacker->unit.xPos + vec_range[i].x;
                  y = attacker->unit.yPos + vec_range[i].y;
                  unit_to_check = GetUnitAtPosition(x, y);
                  if (AreUnitsAllied(attacker->unit.index, unit_to_check->index) && CheckClassCavalry(UNIT_CLASS_ID(unit_to_check)))
                  {
                      attacker->battleAttack += 4;
                      break;
                  }
              }
        }
#endif

#if (defined(SID_Armorboost) && (SID_Armorboost < MAX_SKILL_NUM))
         if (SkillTester(unit, SID_Armorboost) && CheckClassArmor(UNIT_CLASS_ID(&defender->unit)))
         {
             const struct Vec2 vec_range[24] = {
                                      { 0, -3},
                            {-1, -2}, { 0, -2}, { 1, -1},
                  {-2, -1}, {-1, -1}, { 0, -1}, { 1, -1}, { 2, -1},
        {-3,  0}, {-2,  0}, {-1,  0},           { 1,  0}, { 2,  0}, { 3,  0},
                  {-2,  1}, {-1,  1}, { 0,  1}, { 1,  1}, { 2,  1},
                            {-1,  2}, { 0,  2}, { 1,  2},
                                      { 0,  3}};
            /*Search the presence of allied Armor unit.
              Could probably be optimised with a Binary search */
              u32 i, x, y;
              for (i = 0; i < 24; i++)
              {
                  struct Unit * unit_to_check;
                  x = attacker->unit.xPos + vec_range[i].x;
                  y = attacker->unit.yPos + vec_range[i].y;
                  unit_to_check = GetUnitAtPosition(x, y);
                  if (AreUnitsAllied(attacker->unit.index, unit_to_check->index) && CheckClassArmor(UNIT_CLASS_ID(unit_to_check)))
                  {
                      attacker->battleAttack += 4;
                      attacker->battleDefense += 4;
                      break;
                  }
              }
        }
#endif
}

void PreBattleCalcAuraEffect(struct BattleUnit * attacker, struct BattleUnit * defender)
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
    int allies_range2 = 0;
    int allies_range1 = 0;

    int enmies_range3 = 0;
    int enmies_range2 = 0;
    int enmies_range1 = 0;

    for (i = 0; i < 24; i++)
    {
        _x = attacker->unit.xPos + vec_range[i].x;
        _y = attacker->unit.yPos + vec_range[i].y;

        unit = GetUnitAtPosition(_x, _y);
        if (!unit)
            continue;

        if (AreUnitsAllied(attacker->unit.index, unit->index))
        {
#if NEGLECT_RANGE_DEBUFF_CALC_NOT_REAL
            /* Fasten pre-battle calc */
            if (gBattleStats.config & BATTLE_CONFIG_REAL)
#else
            if (1)
#endif
            {
                /* Buffs */
#if (defined(SID_Bond) && (SID_Bond < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Bond) && range3[i] == 1)
                {
                    attacker->battleHitRate += 10;
                    attacker->battleAttack  += 2;
                }
#endif
#if (defined(SID_Charm) && (SID_Charm < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Charm) && range2[i] == 1)
                    attacker->battleAttack  += 3;
#endif

#if (defined(SID_Inspiration) && (SID_Inspiration < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Inspiration) && range2[i] == 1)
                {
                    attacker->battleAttack += 2;
                    attacker->battleDefense += 2;
                }
#endif

#if (defined(SID_DivinelyInspiring) && (SID_DivinelyInspiring < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_DivinelyInspiring) && range1[i] == 1)
                {
                    attacker->battleAttack += 3;
                    attacker->battleDefense += 1;
                }
#endif


#if (defined(SID_BloodTide) && (SID_BloodTide < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_BloodTide) && range1[i] == 1)
                {
                    attacker->battleAttack += 5;
                    attacker->battleHitRate += 5;
                }
#endif

#if (defined(SID_WhitePool) && (SID_WhitePool < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_WhitePool) && range1[i] == 1)
                {
                    attacker->battleAttack += 5;
                    attacker->battleSpeed += 5;
                }
#endif

#if (defined(SID_NightTide) && (SID_NightTide < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_NightTide) && range1[i] == 1)
                {
                    attacker->battleDefense += 5;

                }
#endif

#if (defined(SID_Peacebringer) && (SID_Peacebringer < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Peacebringer) && range2[i] == 1)
                    attacker->battleAttack -= 2;

#endif
            }

            if (range3[i])
                allies_range3++;

            if (range2[i])
                allies_range2++;

            if (range1[i])
                allies_range1++;
        }
        else
        {
#if NEGLECT_RANGE_DEBUFF_CALC_NOT_REAL
            /* Fasten pre-battle calc */
            if (gBattleStats.config & BATTLE_CONFIG_REAL)
#else
            if (1)
#endif
            {
                /* Debuff */
#if (defined(SID_Anathema) && (SID_Anathema < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Anathema) && range3[i] == 1)
                {
                    attacker->battleAvoidRate -= 10;
                    attacker->battleDodgeRate -= 10;
                }
#endif

#if (defined(SID_Daunt) && (SID_Daunt < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Daunt) && range3[i] == 1)
                {
                    attacker->battleHitRate -= 5;
                    attacker->battleCritRate -= 5;
                }
#endif

#if (defined(SID_Intimidate) && (SID_Intimidate < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Intimidate) && range2[i] == 1)
                    attacker->battleAvoidRate -= 10;
#endif

#if (defined(SID_Hex) && (SID_Hex < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Hex) && range1[i] == 1)
                    attacker->battleAvoidRate -= 10;
#endif

#if (defined(SID_VoiceOfPeace) && (SID_VoiceOfPeace < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_VoiceOfPeace) && range2[i] == 1)
                    attacker->battleAttack -= 2;
#endif

#if (defined(SID_Peacebringer) && (SID_Peacebringer < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Peacebringer) && range2[i] == 1)
                    attacker->battleAttack -= 2;
#endif
            }

            if (range3[i])
                enmies_range3++;

            if (range2[i])
                enmies_range2++;

            if (range1[i])
                enmies_range1++;
        }
    }

    unit = GetUnit(attacker->unit.index);

    if (allies_range3 != 0)
    {
        /* Todo */
    }
    else
    {
#if (defined(SID_Tantivy) && (SID_Tantivy < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Tantivy))
        {
            attacker->battleHitRate += 10;
            attacker->battleAvoidRate += 10;
        }
#endif

#if (defined(SID_Focus) && (SID_Focus < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Focus))
        {
            attacker->battleCritRate += 10;
        }
#endif
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
#if (defined(SID_Infiltrator) && (SID_Infiltrator < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Infiltrator))
        {
            attacker->battleAttack += 3;
            attacker->battleHitRate += 15;
        }
#endif
    }

    if (allies_range1 > 0)
    {
#if (defined(SID_BlueFlame) && (SID_BlueFlame < MAX_SKILL_NUM))
        if (SkillTester(&attacker->unit, SID_BlueFlame))
            attacker->battleAttack += 2;
#endif
    }

    /* AND skills */
    if (allies_range3 == 0)
    {
#if (defined(SID_BattleRange_Todo1) && (SID_BattleRange_Todo1 < MAX_SKILL_NUM))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo1))
            attacker->battleAttack += 10;
#endif
    }
    else if (allies_range2 == 0)
    {
#if (defined(SID_BattleRange_Todo2) && (SID_BattleRange_Todo2 < MAX_SKILL_NUM))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo2))
            attacker->battleAttack += 7;
#endif
    }
    else if (allies_range1 == 0)
    {
#if (defined(SID_BattleRange_Todo3) && (SID_BattleRange_Todo3 < MAX_SKILL_NUM))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo3))
            attacker->battleAttack += 5;
#endif
    }
    else
    {}

#ifdef CONFIG_BATTLE_SURROUND
    if (attacker == &gBattleTarget && (gBattleStats.config & BATTLE_CONFIG_REAL))
    {
        /* Flyer in outdoor environments are not affected by this effect (todo) */
        if (!(UNIT_CATTRIBUTES(&attacker->unit) & CA_FLYER) || (0))
        {
            int surround_enemies = enmies_range1 - 1;

            /**
             * When a unit is attacked and adjacent to the enemy,
             * each side with enemy may cause unit avoid -10%
             */
            if (surround_enemies > 0)
            {
                TriggerKtutorial(KTUTORIAL_BATTLE_SURROUNDER);
                attacker->battleAvoidRate -= 10 * surround_enemies;
            }

            /**
             * If unit have been completely surrounded,
             * unit may cause def-5 additionally.
             */
            if (surround_enemies == 3)
                attacker->battleDefense -= 5;
        }
    }
#endif /* CONFIG_BATTLE_SURROUND */
}

void PreBattleCalcSilencerRate(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    if (UNIT_CATTRIBUTES(&defender->unit) & CA_BOSS)
        attacker->battleSilencerRate -= 25;
}

void PreBattleCalcPad(struct BattleUnit * attacker, struct BattleUnit * defender) {}

/* LynJump */
void ComputeBattleUnitStats(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    const PreBattleCalcFunc * it;
    for (it = gpPreBattleCalcFuncs; *it; it++)
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
