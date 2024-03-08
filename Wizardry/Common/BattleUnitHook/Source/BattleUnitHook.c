#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "bmsave.h"

#include "common-chax.h"
#include "skill-system.h"
#include "strmag.h"
#include "battle-system.h"
#include "status-getter.h"
#include "combat-art.h"
#include "constants/skills.h"

STATIC_DECLAR void InitBattleUnitVanilla(struct BattleUnit * bu, struct Unit * unit)
{
    if (!unit)
        return;

    bu->unit = *unit;

    bu->unit.maxHP = GetUnitMaxHp(unit);
    bu->unit.pow = GetUnitPower(unit);
    bu->unit.skl = GetUnitSkill(unit);
    bu->unit.spd = GetUnitSpeed(unit);
    bu->unit.def = GetUnitDefense(unit);
    bu->unit.lck = GetUnitLuck(unit);
    bu->unit.res = GetUnitResistance(unit);
    bu->unit.conBonus = UNIT_CON(unit);
    bu->unit.movBonus = MovGetter(unit);

    bu->levelPrevious = bu->unit.level;
    bu->expPrevious = bu->unit.exp;

    bu->hpInitial = bu->unit.curHP;
    bu->statusOut = 0xFF;

    bu->changeHP = 0;
    bu->changePow = 0;
    bu->changeSkl = 0;
    bu->changeSpd = 0;
    bu->changeDef = 0;
    bu->changeRes = 0;
    bu->changeLck = 0;
    bu->changeCon = 0;

    gBattleActor.wexpMultiplier = 0;
    gBattleTarget.wexpMultiplier = 0;

    bu->wTriangleHitBonus = 0;
    bu->wTriangleDmgBonus = 0;

    bu->nonZeroDamage = FALSE;

    gBattleActor.weaponBroke = FALSE;
    gBattleTarget.weaponBroke = FALSE;

    gBattleActor.expGain = 0;
    gBattleTarget.expGain = 0;
}

STATIC_DECLAR void UpdateUnitFromBattleVanilla(struct Unit * unit, struct BattleUnit * bu)
{
    int tmp;

    unit->level = bu->unit.level;
    unit->exp   = bu->unit.exp;
    unit->curHP = bu->unit.curHP;
    unit->state = bu->unit.state;

    gUnknown_03003060 = UNIT_ARENA_LEVEL(unit);

    if (bu->statusOut >= 0)
        SetUnitStatus(unit, bu->statusOut);

    unit->maxHP += bu->changeHP;
    unit->pow   += bu->changePow;
    unit->skl   += bu->changeSkl;
    unit->spd   += bu->changeSpd;
    unit->def   += bu->changeDef;
    unit->res   += bu->changeRes;
    unit->lck   += bu->changeLck;

    UnitCheckStatCaps(unit);

    tmp = GetBattleUnitUpdatedWeaponExp(bu);

    if (tmp > 0)
        unit->ranks[bu->weaponType] = tmp;

    for (tmp = 0; tmp < UNIT_ITEM_COUNT; ++tmp)
        unit->items[tmp] = bu->unit.items[tmp];

    UnitRemoveInvalidItems(unit);

    if (bu->expGain)
        PidStatsAddExpGained(unit->pCharacterData->number, bu->expGain);
}

/* LynJump */
void InitBattleUnit(struct BattleUnit * bu, struct Unit * unit)
{
    InitBattleUnitVanilla(bu, unit);

    UNIT_MAG(&bu->unit) = MagGetter(unit);
    BU_CHG_MAG(bu) = 0;

    bu->unit._u3A = unit->_u3A;
    bu->unit._u3B = unit->_u3B;
}

/* LynJump */
void UpdateUnitFromBattle(struct Unit * unit, struct BattleUnit * bu)
{
    UpdateUnitFromBattleVanilla(unit, bu);

    UNIT_MAG(unit) += BU_CHG_MAG(bu);

    /* Unit expa sus */
    unit->_u3A = bu->unit._u3A;
    unit->_u3B = bu->unit._u3B;

    ResetSkillLists();
    ResetCombatArtStatus();
}
