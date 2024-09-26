#include "common-chax.h"
#include "battle-system.h"
#include "kernel-lib.h"

typedef void (*PreBattleGenerateFunc) (void);
extern PreBattleGenerateFunc const *const gpPreBattleGenerateFuncs;

/**
 * This is set an addition routine on start of function: `BattleGenerate()`
 * The goal of introducing this function is to make potential modification on battle unit status.
 * This function will be called once per battle calc.
 * As a comparison, pre-battle calc and battle-calc real routine will exec twice for both foe.
 */
void PreBattleGenerateHook(void)
{
	const PreBattleGenerateFunc *it;

	for (it = gpPreBattleGenerateFuncs; *it; it++)
		(*it)();
}

LYN_REPLACE_CHECK(BattleGenerate);
void BattleGenerate(struct Unit *actor, struct Unit *target)
{
#if CHAX
	PreBattleGenerateHook();
#endif

	ComputeBattleUnitStats(&gBattleActor, &gBattleTarget);
	ComputeBattleUnitStats(&gBattleTarget, &gBattleActor);

	ComputeBattleUnitEffectiveStats(&gBattleActor, &gBattleTarget);
	ComputeBattleUnitEffectiveStats(&gBattleTarget, &gBattleActor);

	if (target == NULL)
		ComputeBattleObstacleStats();

	if ((gBattleStats.config & BATTLE_CONFIG_REAL) && (gActionData.scriptedBattleHits))
		BattleUnwindScripted();
	else
		BattleUnwind();

	/* Finally fix on UI */
	ModifyBattleStatusForUI();
}
