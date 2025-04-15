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

STATIC_DECLAR void ResetExtBattleHits(void)
{
	int i;

	CpuFill16(0, gExtBattleHitArray, sizeof(gExtBattleHitArray));

	for (i = 0; i < NEW_BATTLE_HIT_MAX; i++) {
		gExtBattleHitArray[i].act_debuff = -1;
		gExtBattleHitArray[i].tar_debuff = -1;
	}
}

LYN_REPLACE_CHECK(ClearBattleHits);
void ClearBattleHits(void)
{
	CpuFill16(0, gBattleHitArrayRe, sizeof(gBattleHitArrayRe));
	ResetExtBattleHits();
	InitEfxCombatArtRoundData();
	InitEfxSkillRoundData();
	gBattleHitIterator = gBattleHitArrayRe;
}
