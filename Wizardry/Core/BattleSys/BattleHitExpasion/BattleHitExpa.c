#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"

#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "combat-art.h"
#include "battle-system.h"

bool CheckBattleHitOverflow(void)
{
    return (gBattleHitIterator - gBattleHitArrayRe) >= NEW_BATTLE_HIT_MAX;
}

/* LynJump */
void ClearBattleHits(void)
{
    CpuFill16(0, gBattleHitArrayRe, sizeof(gBattleHitArrayRe));
    InitEfxCombatArtRoundData();
    InitEfxSkillRoundData();
    ClearBattleGlobalFlags();
    gBattleHitIterator = gBattleHitArrayRe;
}
