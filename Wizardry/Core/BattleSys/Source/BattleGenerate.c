#include "common-chax.h"
#include "battle-system.h"

/* LynJump */
void BattleGenerate(struct Unit * actor, struct Unit * target)
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
}
