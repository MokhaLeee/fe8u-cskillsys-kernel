#include "common-chax.h"
#include "battle-system.h"

void ClearBattleGlobalFlags(void)
{
    CpuFill16(0, &gBattleActorGlobalFlag, sizeof(gBattleActorGlobalFlag));
    CpuFill16(0, &gBattleTargetGlobalFlag, sizeof(gBattleTargetGlobalFlag));
}

void RegisterHitCnt(struct BattleUnit * bu, bool miss)
{
    struct BattleGlobalFlags * flags = GetBattleGlobalFlags(bu);

    flags->round_cnt++;

    if (!miss)
        flags->round_cnt_hit++;
    else
        flags->round_cnt_avo++;
}
