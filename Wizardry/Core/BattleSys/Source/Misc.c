#include "global.h"

#include "common-chax.h"
#include "battle-system.h"

void ClearBattleGlobalFlags(void)
{
    CpuFill16(0, &gBattleActorGlobalFlag, sizeof(gBattleActorGlobalFlag));
    CpuFill16(0, &gBattleTargetGlobalFlag, sizeof(gBattleTargetGlobalFlag));
}
