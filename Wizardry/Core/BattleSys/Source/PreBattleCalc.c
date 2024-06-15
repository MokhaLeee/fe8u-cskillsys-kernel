#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "combat-art.h"
#include "kernel-tutorial.h"
#include "constants/skills.h"

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
    struct Unit * unit_def = GetUnit(defender->unit.index);

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
        /* Non-blow attacker skill*/

#if (defined(SID_BlowKilling) && (SID_BlowKilling < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_BlowKilling))
            attacker->battleCritRate += 20;
#endif

#if (defined(SID_QuickDraw) && (SID_QuickDraw < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_QuickDraw))
           attacker->battleAttack += 4;
#endif

#if (defined(SID_ArcaneBlade) && (SID_ArcaneBlade < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_ArcaneBlade))
           if (gBattleStats.range == 1)
           {
                attacker->battleCritRate += 3 + UNIT_MAG(unit) / 2;
                attacker->battleHitRate += 3 + UNIT_MAG(unit) / 2;
           }
#endif

#if (defined(SID_Prescience) && (SID_Prescience < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Prescience))
           if (gBattleStats.range == 1)
           {
                attacker->battleHitRate += 15;
                attacker->battleAvoidRate += 15;
           }
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

// Non-Stance defender skill
#if (defined(SID_StrongRiposte) && (SID_StrongRiposte < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_StrongRiposte))
                attacker->battleAttack += 3;
#endif

#if (defined(SID_Patience) && (SID_Patience < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Patience))
                attacker->battleAvoidRate += 10;
#endif

#if (defined(SID_Pursuit) && (SID_Pursuit < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Pursuit))
                attacker->battleSpeed += 2;
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

#if (defined(SID_CriticalPierce) && (SID_CriticalPierce < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_CriticalPierce))
    {
        /**
         * Add the defender's critical avoid rate to the attacker's.
         * Thus effectively 'negating' it.
         */
        attacker->battleCritRate += defender->battleDodgeRate;
    }
#endif

#if (defined(SID_KillingMachine) && (SID_KillingMachine < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_KillingMachine))
        attacker->battleCritRate *= 2;
#endif

#if (defined(SID_HeavyStrikes) && (SID_HeavyStrikes < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_HeavyStrikes))
        attacker->battleCritRate += GetItemWeight(attacker->weapon);
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

#if (defined(SID_BattleVeteran) && (SID_BattleVeteran < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_BattleVeteran))
    {
       u32 n_level = unit->level/10 + 2 * (UNIT_CATTRIBUTES(unit) && CA_PROMOTED);
       attacker->battleHitRate += 5*n_level;
       attacker->battleAttack += n_level;
    }
#endif

#if (defined(SID_StoneBody) && (SID_StoneBody < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_StoneBody))
    {
        int attackerCon = ConGetter(unit);
        int defenderCon = ConGetter(unit_def);

        if (attackerCon > defenderCon)
            attacker->battleAttack += (attackerCon - defenderCon);
    }
#endif

#if (defined(SID_CriticalForce) && (SID_CriticalForce < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_CriticalForce))
       attacker->battleCritRate += unit->skl;
#endif

    if (GetUnitCurrentHp(unit_def) == GetUnitMaxHp(unit_def))
    {
#if (defined(SID_Chivalry) && (SID_Chivalry < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Chivalry))
        {
            attacker->battleDefense += 2;
            attacker->battleAttack += 2;
        }
#endif
    }
    else
    {
#if (defined(SID_Pragmatic) && (SID_Pragmatic < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Pragmatic))
        {
            attacker->battleDefense += 1;
            attacker->battleAttack += 3;
        }
#endif
    }

    if (GetUnitCurrentHp(unit) == GetUnitMaxHp(unit))
    {
#if (defined(SID_Perfectionist) && (SID_Perfectionist < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_Perfectionist))
        {
            attacker->battleHitRate += 15;
            attacker->battleAvoidRate += 15;
        }
#endif
    }
    else
    {
#if (defined(SID_WindDisciple) && (SID_WindDisciple < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_WindDisciple))
        {
            attacker->battleHitRate += 10;
            attacker->battleAvoidRate += 10;
        }
#endif
    }

    if (attacker->hpInitial - defender->hpInitial >= 3)
    {
#if (defined(SID_FireBoost) && (SID_FireBoost < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_FireBoost))
            attacker->battleAttack += 6;
#endif

#if (defined(SID_WindBoost) && (SID_WindBoost < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_WindBoost))
            attacker->battleSpeed += 6;
#endif

#if (defined(SID_EarthBoost) && (SID_EarthBoost < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_EarthBoost))
            if (!IsMagicAttack(defender))
                attacker->battleDefense += 6;
#endif

#if (defined(SID_WaterBoost) && (SID_WaterBoost < MAX_SKILL_NUM))
        if (SkillTester(unit, SID_WaterBoost))
            if (IsMagicAttack(defender))
                attacker->battleDefense += 6;
#endif
    }

#if (defined(SID_ChaosStyle) && (SID_ChaosStyle < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_ChaosStyle))
    {
        if ((IsMagicAttack(attacker) && !IsMagicAttack(defender)) || (!IsMagicAttack(attacker) && IsMagicAttack(defender)))
        {
            attacker->battleSpeed += 3;
        }
    }
#endif

#if defined(SID_Charge) && (SID_Charge < MAX_SKILL_NUM)
        if (SkillTester(unit, SID_Charge))
            attacker->battleAttack += gActionData.moveCount / 2;
#endif

#if (defined(SID_FieryBlood) && (SID_FieryBlood < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_FieryBlood))
    {
       if (GetUnitCurrentHp(unit) < GetUnitMaxHp(unit))
       {
            attacker->battleAttack += 4;
       }
    }
#endif

#if (defined(SID_Wrath) && (SID_Wrath < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Wrath))
    {
       if (GetUnitCurrentHp(unit) < GetUnitMaxHp(unit)/2)
       {
            attacker->battleCritRate += 20;
       }
    }
#endif

    if (attacker->terrainDefense || attacker->terrainAvoid || attacker->terrainResistance)
    {
#if (defined(SID_NaturalCover) && (SID_NaturalCover < MAX_SKILL_NUM)) 
        if (SkillTester(unit, SID_NaturalCover))
            attacker->battleDefense += 3;
#endif
    }
    else
    {
#if (defined(SID_ElbowRoom) && (SID_ElbowRoom < MAX_SKILL_NUM)) 
        if (SkillTester(unit, SID_ElbowRoom))
            attacker->battleAttack += 3;
#endif
    }

#if (defined(SID_Vigilance) && (SID_Vigilance < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Vigilance))
    {
       attacker->battleAvoidRate += 20;
    }
#endif

#if (defined(SID_OutdoorFighter) && (SID_OutdoorFighter < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_OutdoorFighter))
    {
        switch (gBmMapTerrain[unit->yPos][unit->xPos]) {
        case TERRAIN_PLAINS:
        case TERRAIN_ROAD:
        case TERRAIN_VILLAGE_03:
        case TERRAIN_VILLAGE_04:
        case TERRIAN_HOUSE:
        case TERRAIN_ARMORY:
        case TERRAIN_VENDOR:
        case TERRAIN_ARENA_08:
        case TERRAIN_C_ROOM_09:
        case TERRAIN_GATE_0B:
        case TERRAIN_FOREST:
        case TERRAIN_THICKET:
        case TERRAIN_SAND:
        case TERRAIN_DESERT:
        case TERRAIN_RIVER:
        case TERRAIN_MOUNTAIN:
        case TERRAIN_PEAK:
        case TERRAIN_BRIDGE_13:
        case TERRAIN_BRIDGE_14:
        case TERRAIN_SEA:
        case TERRAIN_LAKE:
        case TERRAIN_GATE_23:
        case TERRAIN_CHURCH:
        case TERRAIN_RUINS_25:
        case TERRAIN_CLIFF:
        case TERRAIN_BALLISTA_REGULAR:
        case TERRAIN_BALLISTA_LONG:
        case TERRAIN_BALLISTA_KILLER:
        case TERRAIN_SHIP_FLAT:
        case TERRAIN_SHIP_WRECK:
        case TERRAIN_TILE_2C:
        case TERRAIN_ARENA_30:
        case TERRAIN_VALLEY:
        case TERRAIN_FENCE_32:
        case TERRAIN_SNAG:
        case TERRAIN_BRIDGE_34:
        case TERRAIN_SKY:
        case TERRAIN_DEEPS:
        case TERRAIN_RUINS_37:
        case TERRAIN_INN:
        case TERRAIN_BARREL:
        case TERRAIN_BONE:
        case TERRAIN_DARK:
        case TERRAIN_WATER:
        case TERRAIN_DECK:
        case TERRAIN_BRACE:
        case TERRAIN_MAST:
            attacker->battleHitRate += 10;
            attacker->battleAvoidRate += 10;
            break;

        default:
            break;
        }
    }
#endif

#if (defined(SID_KnightAspirant) && (SID_KnightAspirant < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_KnightAspirant))
    {
        if ((GetUnitMaxHp(unit) * 75) > (GetUnitCurrentHp(unit) * 100))
        {
            attacker->battleAttack += 2;
            attacker->battleAvoidRate += 15;
        }
    }
#endif

#if (defined(SID_Outrider) && (SID_Outrider < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_Outrider))
    {
        attacker->battleDefense += gActionData.moveCount;
        attacker->battleCritRate += (gActionData.moveCount * 3);
    }
#endif

#if (defined(SID_EvenRhythm) && (SID_EvenRhythm < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_EvenRhythm) && (gPlaySt.chapterTurnNumber % 2) == 0)
    {
        attacker->battleAvoidRate += 10;
        attacker->battleHitRate += 10;
    }
#endif

#if (defined(SID_OddRhythm) && (SID_OddRhythm < MAX_SKILL_NUM))
    if (SkillTester(unit, SID_OddRhythm) && (gPlaySt.chapterTurnNumber % 2) == 1)
    {
        attacker->battleAvoidRate += 10;
        attacker->battleHitRate += 10;
    }
#endif
}

void PreBattleCalcSkillsPhaseTurn(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int trun_num = gPlaySt.chapterTurnNumber;
    int trun_num_suppl = trun_num > 15 ? 15 : trun_num;

#if (defined(SID_QuickBurn) && (SID_QuickBurn < MAX_SKILL_NUM))
    if (SkillTester(&attacker->unit, SID_QuickBurn))
    {
        attacker->battleHitRate += (15 - (trun_num_suppl - 1));
        attacker->battleAvoidRate += (15 - (trun_num_suppl - 1));
    }
#endif

#if (defined(SID_SlowBurn) && (SID_SlowBurn < MAX_SKILL_NUM))
    if (SkillTester(&attacker->unit, SID_SlowBurn))
    {
        attacker->battleHitRate += trun_num_suppl;
        attacker->battleAvoidRate += trun_num_suppl;
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
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
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
                if (range1[i] == 1)
                {
#if (defined(SID_DivinelyInspiring) && (SID_DivinelyInspiring < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_DivinelyInspiring))
                    {
                        attacker->battleAttack += 3;
                        attacker->battleDefense += 1;
                    }
#endif

#if (defined(SID_LilysPoise) && (SID_LilysPoise < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_LilysPoise))
                    {
                        attacker->battleAttack += 1;
                        attacker->battleDefense += 3;
                    }
#endif

#if (defined(SID_BloodTide) && (SID_BloodTide < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_BloodTide))
                    {
                        attacker->battleAttack += 5;
                        attacker->battleHitRate += 5;
                    }
#endif

#if (defined(SID_WhitePool) && (SID_WhitePool < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_WhitePool))
                    {
                        attacker->battleAttack += 5;
                        attacker->battleSpeed += 5;
                    }
#endif

#if (defined(SID_NightTide) && (SID_NightTide < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_NightTide))
                        attacker->battleDefense += 5;
#endif

#if (defined(SID_SpurStr) && (SID_SpurStr < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_SpurStr))
                    {
                        if (!IsMagicAttack(attacker))
                            attacker->battleAttack  += 4;
                    }
#endif

#if (defined(SID_SpurMag) && (SID_SpurMag < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_SpurMag))
                    {
                        if (IsMagicAttack(attacker))
                            attacker->battleAttack  += 4;
                    }
#endif

#if (defined(SID_SpurDef) && (SID_SpurDef < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_SpurDef))
                    {
                        if (!IsMagicAttack(defender))
                            attacker->battleDefense+= 4;
                    }
#endif

#if (defined(SID_SpurRes) && (SID_SpurRes < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_SpurRes))
                    {
                        if (IsMagicAttack(defender))
                            attacker->battleDefense+= 4;
                    }
#endif

#if (defined(SID_SpurSpd) && (SID_SpurSpd < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_SpurSpd))
                        attacker->battleSpeed += 4;
#endif

#if (defined(SID_Solidarity) && (SID_Solidarity < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Solidarity))
                    {
                        attacker->battleCritRate += 10;
                        attacker->battleDodgeRate += 10;
                    }
#endif
                }
                if (range2[i] == 1)
                {
#if (defined(SID_Charm) && (SID_Charm < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Charm) )
                        attacker->battleAttack  += 3;
#endif

#if (defined(SID_DriveStr) && (SID_DriveStr < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_DriveStr))
                    {
                        if (!IsMagicAttack(attacker))
                            attacker->battleAttack  += 4;
                    }
#endif

#if (defined(SID_DriveMag) && (SID_DriveMag < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_DriveMag))
                    {
                        if (IsMagicAttack(attacker))
                            attacker->battleAttack  += 4;
                    }
#endif

#if (defined(SID_DriveDef) && (SID_DriveDef < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_DriveDef))
                    {
                        if (!IsMagicAttack(defender))
                            attacker->battleDefense+= 4;
                    }
#endif

#if (defined(SID_DriveRes) && (SID_DriveRes < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_DriveRes))
                    {
                        if (IsMagicAttack(defender))
                            attacker->battleDefense+= 4;
                    }
#endif

#if (defined(SID_DriveSpd) && (SID_DriveSpd < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_DriveSpd))
                        attacker->battleSpeed += 4;
#endif

#if (defined(SID_Inspiration) && (SID_Inspiration < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Inspiration))
                    {
                        attacker->battleAttack += 2;
                        attacker->battleDefense += 2;
                    }
#endif

#if (defined(SID_Peacebringer) && (SID_Peacebringer < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Peacebringer) )
                        attacker->battleAttack -= 2;
#endif

#if (defined(SID_Gentilhomme) && (SID_Gentilhomme < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Gentilhomme)  && (UNIT_CATTRIBUTES(GetUnit(attacker->unit.index)) && CA_FEMALE))
                        attacker->battleDefense += 2;
#endif

#if (defined(SID_Demoiselle) && (SID_Demoiselle < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Demoiselle)  && !(UNIT_CATTRIBUTES(GetUnit(attacker->unit.index)) && CA_FEMALE))
                        attacker->battleDefense += 2;
#endif
                }
                if (range3[i] == 1)
                {                
#if (defined(SID_Bond) && (SID_Bond < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Bond))
                    {
                        attacker->battleHitRate += 10;
                        attacker->battleAttack  += 2;
                    }
#endif

#if (defined(SID_Charisma) && (SID_Charisma < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Charisma))
                    {
                        attacker->battleHitRate += 10;
                        attacker->battleAvoidRate += 10;
                    }
#endif
                }
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
                if (range1[i] == 1)
                {
#if (defined(SID_Hex) && (SID_Hex < MAX_SKILL_NUM))
                if (SkillTester(unit, SID_Hex))
                    attacker->battleAvoidRate -= 10;
#endif
            }
                if (range2[i] == 1)
                {
/* Debuff */

#if (defined(SID_Intimidate) && (SID_Intimidate < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Intimidate) )
                        attacker->battleAvoidRate -= 10;
#endif

#if (defined(SID_VoiceOfPeace) && (SID_VoiceOfPeace < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_VoiceOfPeace) )
                        attacker->battleAttack -= 2;
#endif

#if (defined(SID_Peacebringer) && (SID_Peacebringer < MAX_SKILL_NUM))
                    if (SkillTester(unit, SID_Peacebringer) )
                        attacker->battleAttack -= 2;
#endif
                }
                if (range3[i] == 1)
                {
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
                }
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
            attacker->battleCritRate += 10;
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
