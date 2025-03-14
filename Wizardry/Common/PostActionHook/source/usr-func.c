#include "common-chax.h"
#include "battle-system.h"
#include "post-action.h"

bool PostAction_CommonStart(ProcPtr proc)
{
	int i;
	struct PostActionCommonBuffer *buf = &gPostActionCommonBuffer;

	/**
	 * Calculate total damage
	 */
	buf->total_damage_actor  = 0;
	buf->total_damage_target = 0;

	for (i = 0; i < NEW_BATTLE_HIT_MAX; i++) {
		struct BattleHit *hit = (prBattleHitArray + BattleHitArrayWidth * i);

		if (!(hit->info & BATTLE_HIT_INFO_RETALIATION))
			buf->total_damage_actor  += hit->hpChange;
		else
			buf->total_damage_target += hit->hpChange;

		if (hit->info & BATTLE_HIT_INFO_FINISHES)
			break;
	}

	return false;
}

bool CallExternalPostActionHook(ProcPtr proc)
{
	if (gpExternalPostActionHook)
		return gpExternalPostActionHook(proc);

	return false;
}
