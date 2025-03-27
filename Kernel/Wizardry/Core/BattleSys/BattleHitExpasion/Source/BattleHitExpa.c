#include "common-chax.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "combat-art.h"
#include "battle-system.h"
#include "combo-attack.h"

bool CheckBattleHitOverflow(void)
{
	return (gBattleHitIterator - gBattleHitArrayRe) >= NEW_BATTLE_HIT_MAX;
}

LYN_REPLACE_CHECK(ClearBattleHits);
void ClearBattleHits(void)
{
	CpuFill16(0, gBattleHitArrayRe, sizeof(gBattleHitArrayRe));
	CpuFill16(0, gExtBattleHitArray, sizeof(gExtBattleHitArray));
	InitEfxCombatArtRoundData();
	InitEfxSkillRoundData();
	gBattleHitIterator = gBattleHitArrayRe;
}
