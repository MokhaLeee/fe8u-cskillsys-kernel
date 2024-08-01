#include "common-chax.h"
#include "battle-system.h"
#include "kernel-lib.h"

inline int GetUnitLeaderShip(struct Unit * unit)
{
    return gpLeaderShipPConf[UNIT_CHAR_ID(unit)] + gpLeaderShipJConf[UNIT_CLASS_ID(unit)];
}

STATIC_DECLAR int GetBmLeaderShip(struct Unit * unit)
{
    int i, ret = 0;
    for (i = UNIT_FACTION(unit) + 1; i < (UNIT_FACTION(unit) + GetFactionUnitAmount(UNIT_FACTION(unit))); i++)
    {
        struct Unit * _unit = GetUnit(i);
        if (!UNIT_ALIVE(unit))
            continue;

        ret += GetUnitLeaderShip(_unit);
    }
    return ret;
}

void PreBattleCalcLeadershipBonus(struct BattleUnit * actor, struct BattleUnit * target)
{
    int leadership;

    if (gpLeaderShipConf->en == false)
        return;

    if (gBattleStats.config & BATTLE_CONFIG_ARENA)
        return;

    leadership = GetBmLeaderShip(&actor->unit);
    if (gpLeaderShipConf->cancel_out_opposing == true)
        leadership -= GetBmLeaderShip(&target->unit);

    if (leadership < 0)
        leadership = 0;

    switch (UNIT_FACTION(&actor->unit)) {
    case FACTION_BLUE:
        actor->battleHitRate   += leadership * gpLeaderShipConf->ally_bonus.hit;
        actor->battleAvoidRate += leadership * gpLeaderShipConf->ally_bonus.avo;
        break;

    case FACTION_RED:
        actor->battleHitRate   += leadership * gpLeaderShipConf->enemy_bonus.hit;
        actor->battleAvoidRate += leadership * gpLeaderShipConf->enemy_bonus.avo;
        break;

    case FACTION_GREEN:
        actor->battleHitRate   += leadership * gpLeaderShipConf->npc_bonus.hit;
        actor->battleAvoidRate += leadership * gpLeaderShipConf->npc_bonus.avo;
        break;

    default:
        break;
    }
}
