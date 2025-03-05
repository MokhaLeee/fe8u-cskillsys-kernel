#include "common-chax.h"
#include "battle-system.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

int GetUnitLeaderShip(struct Unit *unit)
{
	int ret = 0;

	ret += gpLeaderShipPConf[UNIT_CHAR_ID(unit)];
	ret += gpLeaderShipJConf[UNIT_CLASS_ID(unit)];

#if defined(SID_Leader) && (COMMON_SKILL_VALID(SID_Leader))
	if (SkillTester(unit, SID_Leader))
		ret += SKILL_EFF0(SID_Leader);
#endif

	return ret;
}

STATIC_DECLAR int GetBmLeaderShip(struct Unit *unit)
{
	int ret = 0;

	FOR_UNITS_ONMAP_FACTION(UNIT_FACTION(unit), _unit,
	{
		ret += GetUnitLeaderShip(_unit);
	})
	return ret;
}

void PreBattleCalcLeadershipBonus(struct BattleUnit *actor, struct BattleUnit *target)
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
