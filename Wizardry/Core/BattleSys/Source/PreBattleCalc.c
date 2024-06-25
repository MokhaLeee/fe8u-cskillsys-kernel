#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "debuff.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "class-types.h"
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

#if (defined(SID_Resourceful) && (COMMON_SKILL_VALID(SID_Resourceful)))
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

    def = attacker->unit.def + attacker->terrainDefense;
    res = attacker->unit.res + attacker->terrainResistance;

    if (IsMagicAttack(defender))
        status = res;
    else
        status = def;


#if (defined(SID_SorceryBlade) && (COMMON_SKILL_VALID(SID_SorceryBlade)))
    if (SkillTester(&attacker->unit, SID_SorceryBlade))
        status = def < res ? def : res;
#endif

    attacker->battleDefense = status;
}

/* LynJump */
void ComputeBattleUnitCritRate(struct BattleUnit * bu)
{
    int status;
    status = bu->unit.skl / 2;

#if defined(SID_SuperLuck) && (COMMON_SKILL_VALID(SID_SuperLuck))
    if (SkillTester(&bu->unit, SID_SuperLuck))
        status = bu->unit.lck;
#endif

    status += GetItemCrit(bu->weapon);

    if (UNIT_CATTRIBUTES(&bu->unit) & CA_CRITBONUS)
        bu->battleCritRate += 15;
    
    bu->battleCritRate = status;
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

#if (defined(SID_RuinedBladePlus) && (COMMON_SKILL_VALID(SID_RuinedBladePlus)))
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
    /* Defiant skills */
    if ((attacker->hpInitial * 4) < attacker->unit.maxHP)
    {
#if (defined(SID_DefiantCrit) && (COMMON_SKILL_VALID(SID_DefiantCrit)))
        if (SkillTester(&attacker->unit, SID_DefiantCrit))
            attacker->battleCritRate += 50;
#endif

#if (defined(SID_DefiantAvoid) && (COMMON_SKILL_VALID(SID_DefiantAvoid)))
        if (SkillTester(&attacker->unit, SID_DefiantAvoid))
            attacker->battleAvoidRate += 30;
#endif
    }

    /* Blow skills */
    if (attacker == &gBattleActor && !(gBattleStats.config & BATTLE_CONFIG_ARENA))
    {
#if (defined(SID_BlowDarting) && (COMMON_SKILL_VALID(SID_BlowDarting)))
        if (SkillTester(&attacker->unit, SID_BlowDarting))
            attacker->battleSpeed += 6;
#endif

#if (defined(SID_BlowDeath) && (COMMON_SKILL_VALID(SID_BlowDeath)))
        if (SkillTester(&attacker->unit, SID_BlowDeath))
        {
            if (!IsMagicAttack(attacker))
                attacker->battleAttack += 6;
        }
#endif

#if (defined(SID_BlowArmored) && (COMMON_SKILL_VALID(SID_BlowArmored)))
        if (SkillTester(&attacker->unit, SID_BlowArmored))
        {
            if (!IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
#endif

#if (defined(SID_BlowFiendish) && (COMMON_SKILL_VALID(SID_BlowFiendish)))
        if (SkillTester(&attacker->unit, SID_BlowFiendish))
        {
            if (IsMagicAttack(attacker))
                attacker->battleAttack += 6;
        }
#endif

#if (defined(SID_BlowWarding) && (COMMON_SKILL_VALID(SID_BlowWarding)))
        if (SkillTester(&attacker->unit, SID_BlowWarding))
        {
            if (IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
#endif

#if (defined(SID_BlowDuelist) && (COMMON_SKILL_VALID(SID_BlowDuelist)))
        if (SkillTester(&attacker->unit, SID_BlowDuelist))
           attacker->battleAvoidRate += 20;
#endif

#if (defined(SID_BlowUncanny) && (COMMON_SKILL_VALID(SID_BlowUncanny)))
        if (SkillTester(&attacker->unit, SID_BlowUncanny))
            attacker->battleHitRate += 30;
#endif
        /* Non-blow attacker skill*/

#if (defined(SID_BlowKilling) && (COMMON_SKILL_VALID(SID_BlowKilling)))
        if (SkillTester(&attacker->unit, SID_BlowKilling))
            attacker->battleCritRate += 20;
#endif

#if (defined(SID_QuickDraw) && (COMMON_SKILL_VALID(SID_QuickDraw)))
        if (SkillTester(&attacker->unit, SID_QuickDraw))
           attacker->battleAttack += 4;
#endif

#if (defined(SID_ArcaneBlade) && (COMMON_SKILL_VALID(SID_ArcaneBlade)))
        if (SkillTester(&attacker->unit, SID_ArcaneBlade))
           if (gBattleStats.range == 1)
           {
                attacker->battleCritRate += 3 + UNIT_MAG(&attacker->unit) / 2;
                attacker->battleHitRate += 3 + UNIT_MAG(&attacker->unit) / 2;
           }
#endif

#if (defined(SID_Prescience) && (COMMON_SKILL_VALID(SID_Prescience)))
        if (SkillTester(&attacker->unit, SID_Prescience))
           if (gBattleStats.range == 1)
           {
                attacker->battleHitRate += 15;
                attacker->battleAvoidRate += 15;
           }
#endif
    }

    /* Stance skills */
    if (attacker == &gBattleTarget && !(gBattleStats.config & BATTLE_CONFIG_ARENA))
    {
#if (defined(SID_StanceBracing) && (COMMON_SKILL_VALID(SID_StanceBracing)))
        if (SkillTester(&attacker->unit, SID_StanceBracing))
            attacker->battleDefense += 4;
#endif

#if (defined(SID_StanceDarting) && (COMMON_SKILL_VALID(SID_StanceDarting)))
        if (SkillTester(&attacker->unit, SID_StanceDarting))
            attacker->battleSpeed += 6;
#endif

#if (defined(SID_StanceFierce) && (COMMON_SKILL_VALID(SID_StanceFierce)))
        if (SkillTester(&attacker->unit, SID_StanceFierce))
            attacker->battleAttack += 6;
#endif

#if (defined(SID_StanceKestrel) && (COMMON_SKILL_VALID(SID_StanceKestrel)))
        if (SkillTester(&attacker->unit, SID_StanceKestrel))
        {
            attacker->battleAttack += 4;
            attacker->battleSpeed += 4;
        }
#endif

#if (defined(SID_StanceMirror) && (COMMON_SKILL_VALID(SID_StanceMirror)))
        if (SkillTester(&attacker->unit, SID_StanceMirror))
        {
            attacker->battleAttack += 4;

            if (IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }
#endif

#if (defined(SID_StanceReady) && (COMMON_SKILL_VALID(SID_StanceReady)))
        if (SkillTester(&attacker->unit, SID_StanceReady))
        {
            attacker->battleSpeed += 4;

            if (!IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }
#endif

#if (defined(SID_StanceSteady) && (COMMON_SKILL_VALID(SID_StanceSteady)))
        if (SkillTester(&attacker->unit, SID_StanceSteady))
        {
            if (!IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
#endif

#if (defined(SID_StanceSturdy) && (COMMON_SKILL_VALID(SID_StanceSturdy)))
        if (SkillTester(&attacker->unit, SID_StanceSturdy))
        {
            attacker->battleAttack += 4;

            if (!IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }
#endif

#if (defined(SID_StanceSwift) && (COMMON_SKILL_VALID(SID_StanceSwift)))
        if (SkillTester(&attacker->unit, SID_StanceSwift))
        {
            attacker->battleSpeed += 4;

            if (IsMagicAttack(defender))
                attacker->battleDefense += 4;
        }
#endif

#if (defined(SID_StanceWarding) && (COMMON_SKILL_VALID(SID_StanceWarding)))
        if (SkillTester(&attacker->unit, SID_StanceWarding))
        {
            if (IsMagicAttack(defender))
                attacker->battleDefense += 6;
        }
#endif

#if (defined(SID_StanceSpectrum) && (COMMON_SKILL_VALID(SID_StanceSpectrum)))
        if (SkillTester(&attacker->unit, SID_StanceSpectrum))
        {
                attacker->battleAttack += 2;
                attacker->battleSpeed += 2;
                attacker->battleDefense += 2;
        }
#endif

        // Non-Stance defender skill
#if (defined(SID_StrongRiposte) && (COMMON_SKILL_VALID(SID_StrongRiposte)))
        if (SkillTester(&attacker->unit, SID_StrongRiposte))
                attacker->battleAttack += 3;
#endif

#if (defined(SID_Patience) && (COMMON_SKILL_VALID(SID_Patience)))
        if (SkillTester(&attacker->unit, SID_Patience))
                attacker->battleAvoidRate += 10;
#endif

#if (defined(SID_Pursuit) && (COMMON_SKILL_VALID(SID_Pursuit)))
        if (SkillTester(&attacker->unit, SID_Pursuit))
                attacker->battleSpeed += 2;
#endif
    }

    /* Misc */
#if (defined(SID_Lethality) && (COMMON_SKILL_VALID(SID_Lethality)))
    if (SkillTester(&attacker->unit, SID_Lethality))
        attacker->battleSilencerRate += attacker->unit.skl;
#endif

#if (defined(SID_WatchfulEye) && (COMMON_SKILL_VALID(SID_WatchfulEye)))
    if (SkillTester(&attacker->unit, SID_WatchfulEye))
        attacker->battleHitRate += 20;
#endif

#if (defined(SID_Crit) && (COMMON_SKILL_VALID(SID_Crit)))
    if (SkillTester(&attacker->unit, SID_Crit))
        attacker->battleCritRate += 15;
#endif

#if (defined(SID_Avoid) && (COMMON_SKILL_VALID(SID_Avoid)))
    if (SkillTester(&attacker->unit, SID_Avoid))
        attacker->battleAvoidRate += 10;
#endif

    /* wType related */
    switch (attacker->weaponType)
    {
    case ITYPE_SWORD:
#if (defined(SID_CritSword) && (COMMON_SKILL_VALID(SID_CritSword)))
        if (SkillTester(&attacker->unit, SID_CritSword))
            attacker->battleCritRate += 10;
#endif

#if (defined(SID_FaireSword) && (COMMON_SKILL_VALID(SID_FaireSword)))
        if (SkillTester(&attacker->unit, SID_FaireSword))
            attacker->battleAttack += 5;
#endif

#if (defined(SID_AvoidSword) && (COMMON_SKILL_VALID(SID_AvoidSword)))
        if (SkillTester(&attacker->unit, SID_AvoidSword))
            attacker->battleAvoidRate += 20;
#endif
        break;

    case ITYPE_LANCE:
#if (defined(SID_CritLance) && (COMMON_SKILL_VALID(SID_CritLance)))
        if (SkillTester(&attacker->unit, SID_CritLance))
            attacker->battleCritRate += 10;
#endif

#if (defined(SID_FaireLance) && (COMMON_SKILL_VALID(SID_FaireLance)))
        if (SkillTester(&attacker->unit, SID_FaireLance))
            attacker->battleAttack += 5;
#endif
        break;

    case ITYPE_AXE:
#if (defined(SID_CritAxe) && (COMMON_SKILL_VALID(SID_CritAxe)))
        if (SkillTester(&attacker->unit, SID_CritAxe))
            attacker->battleCritRate += 10;
#endif

#if (defined(SID_FaireAxe) && (COMMON_SKILL_VALID(SID_FaireAxe)))
        if (SkillTester(&attacker->unit, SID_FaireAxe))
            attacker->battleAttack += 5;
#endif
        break;

    case ITYPE_BOW:
#if (defined(SID_CritBow) && (COMMON_SKILL_VALID(SID_CritBow)))
        if (SkillTester(&attacker->unit, SID_CritBow))
            attacker->battleCritRate += 10;
#endif

#if (defined(SID_FaireBow) && (COMMON_SKILL_VALID(SID_FaireBow)))
        if (SkillTester(&attacker->unit, SID_FaireBow))
            attacker->battleAttack += 5;
#endif
        break;

    case ITYPE_ANIMA:
    case ITYPE_LIGHT:
    case ITYPE_DARK:
#if (defined(SID_FaireBMag) && (COMMON_SKILL_VALID(SID_FaireBMag)))
        if (SkillTester(&attacker->unit, SID_FaireBMag))
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

#if (defined(SID_BlueFlame) && (COMMON_SKILL_VALID(SID_BlueFlame)))
    if (SkillTester(&attacker->unit, SID_BlueFlame))
        attacker->battleAttack += 2;
#endif

#if (defined(SID_Frenzy) && (COMMON_SKILL_VALID(SID_Frenzy)))
    if (SkillTester(&attacker->unit, SID_Frenzy))
        if ((attacker->unit.maxHP - attacker->hpInitial) >= 4)
            attacker->battleAttack += (attacker->unit.maxHP - attacker->hpInitial) / 4;
#endif

#if (defined(SID_CriticalPierce) && (COMMON_SKILL_VALID(SID_CriticalPierce)))
    if (SkillTester(&attacker->unit, SID_CriticalPierce))
        attacker->battleCritRate += defender->battleDodgeRate;
#endif

#if (defined(SID_KillingMachine) && (COMMON_SKILL_VALID(SID_KillingMachine)))
    if (SkillTester(&attacker->unit, SID_KillingMachine))
        attacker->battleCritRate *= 2;
#endif

#if (defined(SID_HeavyStrikes) && (COMMON_SKILL_VALID(SID_HeavyStrikes)))
    if (SkillTester(&attacker->unit, SID_HeavyStrikes))
        attacker->battleCritRate += GetItemWeight(attacker->weapon);
#endif

#if (defined(SID_Technician) && (COMMON_SKILL_VALID(SID_Technician)))
    if (SkillTester(&attacker->unit, SID_Technician))
        if (GetItemRequiredExp(attacker->weapon) < WPN_EXP_D)
            attacker->battleAttack += GetItemMight(attacker->weapon) / 2;
#endif

#if (defined(SID_BattleVeteran) && (COMMON_SKILL_VALID(SID_BattleVeteran)))
    if (SkillTester(&attacker->unit, SID_BattleVeteran))
    {
       int n_level = simple_div(attacker->levelPrevious + GetUnitHiddenLevel(&attacker->unit), 10);
       attacker->battleHitRate += 5 * n_level;
       attacker->battleAttack += n_level;
    }
#endif

#if (defined(SID_StoneBody) && (COMMON_SKILL_VALID(SID_StoneBody)))
    if (SkillTester(&attacker->unit, SID_StoneBody))
    {
        if (attacker->unit.conBonus > defender->unit.conBonus)
            attacker->battleAttack += (attacker->unit.conBonus - defender->unit.conBonus);
    }
#endif

#if (defined(SID_CriticalForce) && (COMMON_SKILL_VALID(SID_CriticalForce)))
    if (SkillTester(&attacker->unit, SID_CriticalForce))
       attacker->battleCritRate += attacker->unit.skl;
#endif

    if (defender->hpInitial == defender->unit.maxHP)
    {
#if (defined(SID_Chivalry) && (COMMON_SKILL_VALID(SID_Chivalry)))
        if (SkillTester(&attacker->unit, SID_Chivalry))
        {
            attacker->battleDefense += 2;
            attacker->battleAttack += 2;
        }
#endif
    }
    else
    {
#if (defined(SID_Pragmatic) && (COMMON_SKILL_VALID(SID_Pragmatic)))
        if (SkillTester(&attacker->unit, SID_Pragmatic))
        {
            attacker->battleDefense += 1;
            attacker->battleAttack += 3;
        }
#endif
    }

    if (attacker->hpInitial == attacker->unit.maxHP)
    {
#if (defined(SID_Perfectionist) && (COMMON_SKILL_VALID(SID_Perfectionist)))
        if (SkillTester(&attacker->unit, SID_Perfectionist))
        {
            attacker->battleHitRate += 15;
            attacker->battleAvoidRate += 15;
        }
#endif
    }
    else
    {
#if (defined(SID_WindDisciple) && (COMMON_SKILL_VALID(SID_WindDisciple)))
        if (SkillTester(&attacker->unit, SID_WindDisciple))
        {
            attacker->battleHitRate += 10;
            attacker->battleAvoidRate += 10;
        }
#endif
    }

    if (attacker->hpInitial - defender->hpInitial >= 3)
    {
#if (defined(SID_FireBoost) && (COMMON_SKILL_VALID(SID_FireBoost)))
        if (SkillTester(&attacker->unit, SID_FireBoost))
            attacker->battleAttack += 6;
#endif

#if (defined(SID_WindBoost) && (COMMON_SKILL_VALID(SID_WindBoost)))
        if (SkillTester(&attacker->unit, SID_WindBoost))
            attacker->battleSpeed += 6;
#endif

#if (defined(SID_EarthBoost) && (COMMON_SKILL_VALID(SID_EarthBoost)))
        if (SkillTester(&attacker->unit, SID_EarthBoost))
            if (!IsMagicAttack(defender))
                attacker->battleDefense += 6;
#endif

#if (defined(SID_WaterBoost) && (COMMON_SKILL_VALID(SID_WaterBoost)))
        if (SkillTester(&attacker->unit, SID_WaterBoost))
            if (IsMagicAttack(defender))
                attacker->battleDefense += 6;
#endif
    }

#if (defined(SID_ChaosStyle) && (COMMON_SKILL_VALID(SID_ChaosStyle)))
    if (SkillTester(&attacker->unit, SID_ChaosStyle))
        if ((IsMagicAttack(attacker) && !IsMagicAttack(defender)) || (!IsMagicAttack(attacker) && IsMagicAttack(defender)))
            attacker->battleSpeed += 3;
#endif

#if defined(SID_Charge) && (COMMON_SKILL_VALID(SID_Charge))
        if (SkillTester(&attacker->unit, SID_Charge))
            attacker->battleAttack += gActionData.moveCount / 2;
#endif

#if (defined(SID_FieryBlood) && (COMMON_SKILL_VALID(SID_FieryBlood)))
    if (SkillTester(&attacker->unit, SID_FieryBlood))
       if (attacker->hpInitial < attacker->unit.maxHP)
            attacker->battleAttack += 4;
#endif

#if (defined(SID_Wrath) && (COMMON_SKILL_VALID(SID_Wrath)))
    if (SkillTester(&attacker->unit, SID_Wrath))
       if ((attacker->hpInitial * 2) < attacker->unit.maxHP)
            attacker->battleCritRate += 20;
#endif

    if (attacker->terrainDefense || attacker->terrainAvoid || attacker->terrainResistance)
    {
#if (defined(SID_NaturalCover) && (COMMON_SKILL_VALID(SID_NaturalCover))) 
        if (SkillTester(&attacker->unit, SID_NaturalCover))
            attacker->battleDefense += 3;
#endif
    }
    else
    {
#if (defined(SID_ElbowRoom) && (COMMON_SKILL_VALID(SID_ElbowRoom))) 
        if (SkillTester(&attacker->unit, SID_ElbowRoom))
            attacker->battleAttack += 3;
#endif
    }

#if (defined(SID_Vigilance) && (COMMON_SKILL_VALID(SID_Vigilance)))
    if (SkillTester(&attacker->unit, SID_Vigilance))
       attacker->battleAvoidRate += 20;
#endif

#if (defined(SID_OutdoorFighter) && (COMMON_SKILL_VALID(SID_OutdoorFighter)))
    if (SkillTester(&attacker->unit, SID_OutdoorFighter))
    {
        switch (gBmMapTerrain[attacker->unit.yPos][attacker->unit.xPos]) {
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

#if (defined(SID_KnightAspirant) && (COMMON_SKILL_VALID(SID_KnightAspirant)))
    if (SkillTester(&attacker->unit, SID_KnightAspirant))
    {
        if ((attacker->unit.maxHP * 75) > (attacker->hpInitial * 100))
        {
            attacker->battleAttack += 2;
            attacker->battleAvoidRate += 15;
        }
    }
#endif

#if (defined(SID_Outrider) && (COMMON_SKILL_VALID(SID_Outrider)))
    if (SkillTester(&attacker->unit, SID_Outrider))
    {
        attacker->battleDefense += gActionData.moveCount;
        attacker->battleCritRate += (gActionData.moveCount * 3);
    }
#endif

#if (defined(SID_EvenRhythm) && (COMMON_SKILL_VALID(SID_EvenRhythm)))
    if (SkillTester(&attacker->unit, SID_EvenRhythm) && (gPlaySt.chapterTurnNumber % 2) == 0)
    {
        attacker->battleAvoidRate += 10;
        attacker->battleHitRate += 10;
    }
#endif

#if (defined(SID_OddRhythm) && (COMMON_SKILL_VALID(SID_OddRhythm)))
    if (SkillTester(&attacker->unit, SID_OddRhythm) && (gPlaySt.chapterTurnNumber % 2) == 1)
    {
        attacker->battleAvoidRate += 10;
        attacker->battleHitRate += 10;
    }
#endif

#if (defined(SID_SilentPride) && (COMMON_SKILL_VALID(SID_SilentPride)))
    if (SkillTester(&attacker->unit, SID_SilentPride))
    {
        int mult = simple_div(attacker->unit.maxHP * 4, attacker->hpInitial);
        attacker->battleAttack += 2 * mult;
        attacker->battleDefense += 2 * mult;
    }
#endif

#if defined(SID_Guts) && (COMMON_SKILL_VALID(SID_Guts))
    if (SkillTester(&attacker->unit, SID_Guts) && (GetUnitStatusIndex(&attacker->unit) != UNIT_STATUS_NONE))
        attacker->battleAttack += 5;
#endif


#if defined(SID_HolyAura) && (COMMON_SKILL_VALID(SID_HolyAura))
    if (SkillTester(&attacker->unit, SID_HolyAura) && attacker->weaponType == ITYPE_LIGHT)
    {
        attacker->battleAttack += 1;
        attacker->battleCritRate += 5;
        attacker->battleHitRate += 5;
        attacker->battleAvoidRate += 5;
    }
#endif

#if (defined(SID_TowerShield) && (COMMON_SKILL_VALID(SID_TowerShield)))
    if (SkillTester(&attacker->unit, SID_TowerShield) && gBattleStats.range >= 2)
        attacker->battleDefense += 6;
#endif

#if (defined(SID_ShortShield) && (COMMON_SKILL_VALID(SID_ShortShield)))
    if (SkillTester(&attacker->unit, SID_ShortShield) && gBattleStats.range == 1)
        attacker->battleDefense += 6;
#endif

#if (defined(SID_StunningSmile) && (COMMON_SKILL_VALID(SID_StunningSmile)))
    if (SkillTester(&defender->unit, SID_StunningSmile) && !(UNIT_CATTRIBUTES(&attacker->unit) && CA_FEMALE))
        attacker->battleAvoidRate -= 20;
#endif

#if defined(SID_Trample) && (COMMON_SKILL_VALID(SID_Trample))
    if (SkillTester(&attacker->unit, SID_Trample) && !(UNIT_CATTRIBUTES(&defender->unit) & CA_MOUNTEDAID))
        attacker->battleAttack += 5;
#endif

#if defined(SID_Opportunist) && (COMMON_SKILL_VALID(SID_Opportunist))
    if (SkillTester(&attacker->unit, SID_Opportunist) && !defender->canCounter)
        attacker->battleAttack += 4;
#endif

#if (defined(SID_Vanity) && (COMMON_SKILL_VALID(SID_Vanity)))
    if (SkillTester(&attacker->unit, SID_Vanity) && gBattleStats.range == 2)
    {
        attacker->battleAttack += 2;
        attacker->battleHitRate += 10;
    }
#endif
}

void PreBattleCalcSkillsPhaseTurn(struct BattleUnit * attacker, struct BattleUnit * defender)
{
    int trun_num = gPlaySt.chapterTurnNumber;
    int trun_num_suppl = trun_num > 15 ? 15 : trun_num;

#if (defined(SID_QuickBurn) && (COMMON_SKILL_VALID(SID_QuickBurn)))
    if (SkillTester(&attacker->unit, SID_QuickBurn))
    {
        attacker->battleHitRate += (15 - (trun_num_suppl - 1));
        attacker->battleAvoidRate += (15 - (trun_num_suppl - 1));
    }
#endif

#if (defined(SID_SlowBurn) && (COMMON_SKILL_VALID(SID_SlowBurn)))
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

    bool lord_range2 = false;

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
#if (defined(SID_DivinelyInspiring) && (COMMON_SKILL_VALID(SID_DivinelyInspiring)))
                    if (SkillTester(unit, SID_DivinelyInspiring))
                    {
                        attacker->battleAttack += 3;
                        attacker->battleDefense += 1;
                    }
#endif

#if (defined(SID_LilysPoise) && (COMMON_SKILL_VALID(SID_LilysPoise)))
                    if (SkillTester(unit, SID_LilysPoise))
                    {
                        attacker->battleAttack += 1;
                        attacker->battleDefense += 3;
                    }
#endif

#if (defined(SID_BloodTide) && (COMMON_SKILL_VALID(SID_BloodTide)))
                    if (SkillTester(unit, SID_BloodTide))
                    {
                        attacker->battleAttack += 5;
                        attacker->battleHitRate += 5;
                    }
#endif

#if (defined(SID_WhitePool) && (COMMON_SKILL_VALID(SID_WhitePool)))
                    if (SkillTester(unit, SID_WhitePool))
                    {
                        attacker->battleAttack += 5;
                        attacker->battleSpeed += 5;
                    }
#endif

#if (defined(SID_NightTide) && (COMMON_SKILL_VALID(SID_NightTide)))
                    if (SkillTester(unit, SID_NightTide))
                        attacker->battleDefense += 5;
#endif

#if (defined(SID_SpurStr) && (COMMON_SKILL_VALID(SID_SpurStr)))
                    if (SkillTester(unit, SID_SpurStr))
                    {
                        if (!IsMagicAttack(attacker))
                            attacker->battleAttack  += 4;
                    }
#endif

#if (defined(SID_SpurMag) && (COMMON_SKILL_VALID(SID_SpurMag)))
                    if (SkillTester(unit, SID_SpurMag))
                    {
                        if (IsMagicAttack(attacker))
                            attacker->battleAttack  += 4;
                    }
#endif

#if (defined(SID_SpurDef) && (COMMON_SKILL_VALID(SID_SpurDef)))
                    if (SkillTester(unit, SID_SpurDef))
                    {
                        if (!IsMagicAttack(defender))
                            attacker->battleDefense+= 4;
                    }
#endif

#if (defined(SID_SpurRes) && (COMMON_SKILL_VALID(SID_SpurRes)))
                    if (SkillTester(unit, SID_SpurRes))
                    {
                        if (IsMagicAttack(defender))
                            attacker->battleDefense+= 4;
                    }
#endif

#if (defined(SID_SpurSpd) && (COMMON_SKILL_VALID(SID_SpurSpd)))
                    if (SkillTester(unit, SID_SpurSpd))
                        attacker->battleSpeed += 4;
#endif

#if (defined(SID_Solidarity) && (COMMON_SKILL_VALID(SID_Solidarity)))
                    if (SkillTester(unit, SID_Solidarity))
                    {
                        attacker->battleCritRate += 10;
                        attacker->battleDodgeRate += 10;
                    }
#endif
                }
                if (range2[i] == 1)
                {
#if (defined(SID_Charm) && (COMMON_SKILL_VALID(SID_Charm)))
                    if (SkillTester(unit, SID_Charm) )
                        attacker->battleAttack  += 3;
#endif

#if (defined(SID_DriveStr) && (COMMON_SKILL_VALID(SID_DriveStr)))
                    if (SkillTester(unit, SID_DriveStr))
                    {
                        if (!IsMagicAttack(attacker))
                            attacker->battleAttack  += 4;
                    }
#endif

#if (defined(SID_DriveMag) && (COMMON_SKILL_VALID(SID_DriveMag)))
                    if (SkillTester(unit, SID_DriveMag))
                    {
                        if (IsMagicAttack(attacker))
                            attacker->battleAttack  += 4;
                    }
#endif

#if (defined(SID_DriveDef) && (COMMON_SKILL_VALID(SID_DriveDef)))
                    if (SkillTester(unit, SID_DriveDef))
                    {
                        if (!IsMagicAttack(defender))
                            attacker->battleDefense+= 4;
                    }
#endif

#if (defined(SID_DriveRes) && (COMMON_SKILL_VALID(SID_DriveRes)))
                    if (SkillTester(unit, SID_DriveRes))
                    {
                        if (IsMagicAttack(defender))
                            attacker->battleDefense+= 4;
                    }
#endif

#if (defined(SID_DriveSpd) && (COMMON_SKILL_VALID(SID_DriveSpd)))
                    if (SkillTester(unit, SID_DriveSpd))
                        attacker->battleSpeed += 4;
#endif

#if (defined(SID_Inspiration) && (COMMON_SKILL_VALID(SID_Inspiration)))
                    if (SkillTester(unit, SID_Inspiration))
                    {
                        attacker->battleAttack += 2;
                        attacker->battleDefense += 2;
                    }
#endif

#if (defined(SID_Peacebringer) && (COMMON_SKILL_VALID(SID_Peacebringer)))
                    if (SkillTester(unit, SID_Peacebringer) )
                        attacker->battleAttack -= 2;
#endif

#if (defined(SID_Gentilhomme) && (COMMON_SKILL_VALID(SID_Gentilhomme)))
                    if (SkillTester(unit, SID_Gentilhomme)  && (UNIT_CATTRIBUTES(&attacker->unit) && CA_FEMALE))
                        attacker->battleDefense += 2;
#endif

#if (defined(SID_Demoiselle) && (COMMON_SKILL_VALID(SID_Demoiselle)))
                    if (SkillTester(unit, SID_Demoiselle)  && !(UNIT_CATTRIBUTES(&attacker->unit) && CA_FEMALE))
                        attacker->battleDefense += 2;
#endif
                }
                if (range3[i] == 1)
                {                
#if (defined(SID_Bond) && (COMMON_SKILL_VALID(SID_Bond)))
                    if (SkillTester(unit, SID_Bond))
                    {
                        attacker->battleHitRate += 10;
                        attacker->battleAttack  += 2;
                    }
#endif

#if (defined(SID_Charisma) && (COMMON_SKILL_VALID(SID_Charisma)))
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
            {
                if (UNIT_CATTRIBUTES(unit) && CA_LORD)
                    lord_range2 = true;
                allies_range2++;
            }
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
#if (defined(SID_Hex) && (COMMON_SKILL_VALID(SID_Hex)))
                if (SkillTester(unit, SID_Hex))
                    attacker->battleAvoidRate -= 10;
#endif
            }
                if (range2[i] == 1)
                {
/* Debuff */

#if (defined(SID_Intimidate) && (COMMON_SKILL_VALID(SID_Intimidate)))
                    if (SkillTester(unit, SID_Intimidate) )
                        attacker->battleAvoidRate -= 10;
#endif

#if (defined(SID_VoiceOfPeace) && (COMMON_SKILL_VALID(SID_VoiceOfPeace)))
                    if (SkillTester(unit, SID_VoiceOfPeace) )
                        attacker->battleAttack -= 2;
#endif

#if (defined(SID_Peacebringer) && (COMMON_SKILL_VALID(SID_Peacebringer)))
                    if (SkillTester(unit, SID_Peacebringer) )
                        attacker->battleAttack -= 2;
#endif
                }
                if (range3[i] == 1)
                {
#if (defined(SID_Anathema) && (COMMON_SKILL_VALID(SID_Anathema)))
                    if (SkillTester(unit, SID_Anathema) && range3[i] == 1)
                    {
                        attacker->battleAvoidRate -= 10;
                        attacker->battleDodgeRate -= 10;
                    }
#endif

#if (defined(SID_Daunt) && (COMMON_SKILL_VALID(SID_Daunt)))
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

    if (allies_range3 != 0)
    {
        /* Todo */
    }
    else
    {
#if (defined(SID_Tantivy) && (COMMON_SKILL_VALID(SID_Tantivy)))
        if (SkillTester(&attacker->unit, SID_Tantivy))
        {
            attacker->battleHitRate += 10;
            attacker->battleAvoidRate += 10;
        }
#endif

#if (defined(SID_Focus) && (COMMON_SKILL_VALID(SID_Focus)))
        if (SkillTester(&attacker->unit, SID_Focus))
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
#if (defined(SID_Infiltrator) && (COMMON_SKILL_VALID(SID_Infiltrator)))
        if (SkillTester(&attacker->unit, SID_Infiltrator))
        {
            attacker->battleAttack += 3;
            attacker->battleHitRate += 15;
        }
#endif
    }

    if (allies_range1 > 0)
    {
#if (defined(SID_BlueFlame) && (COMMON_SKILL_VALID(SID_BlueFlame)))
        if (SkillTester(&attacker->unit, SID_BlueFlame))
            attacker->battleAttack += 2;
#endif
    }

    /* AND skills */
    if (allies_range3 == 0)
    {
#if (defined(SID_BattleRange_Todo1) && (COMMON_SKILL_VALID(SID_BattleRange_Todo1)))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo1))
            attacker->battleAttack += 10;
#endif
    }
    else if (allies_range2 == 0)
    {
#if (defined(SID_BattleRange_Todo2) && (COMMON_SKILL_VALID(SID_BattleRange_Todo2)))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo2))
            attacker->battleAttack += 7;
#endif
    }
    else if (allies_range1 == 0)
    {
#if (defined(SID_BattleRange_Todo3) && (COMMON_SKILL_VALID(SID_BattleRange_Todo3)))
        if (SkillTester(&attacker->unit, SID_BattleRange_Todo3))
            attacker->battleAttack += 5;
#endif
    }
    else
    {}

    if (lord_range2)
    {
#if (defined(SID_Loyalty) && (COMMON_SKILL_VALID(SID_Loyalty)))
        if (SkillTester(&attacker->unit, SID_Loyalty))
        {   attacker->battleDefense += 3;
            attacker->battleHitRate += 15;
        }
#endif   
    }
    
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
