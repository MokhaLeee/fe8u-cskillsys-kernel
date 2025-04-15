#include "common-chax.h"
#include "battle-system.h"

STATIC_DECLAR void BanimRoundUpdateEfxStatus(struct Anim *anim)
{
#ifdef CONFIG_EFX_STATUS_DISPLAY_ON_EXTHIT
	struct Anim *act_anim = GetAnimAnotherSide(anim);
	struct Anim *tar_anim = GetAnimAnotherSide(act_anim);
	struct ExtBattleHit *exthit = GetExtBattleHit(anim->nextRoundId - 1);

	if (exthit->act_debuff >= 0)
		SetUnitEfxDebuff(act_anim, exthit->act_debuff);

	if (exthit->tar_debuff >= 0)
		SetUnitEfxDebuff(tar_anim, exthit->tar_debuff);
#endif
}

LYN_REPLACE_CHECK(StartBattleAnimHitEffectsDefault);
void StartBattleAnimHitEffectsDefault(struct Anim *anim, int type)
{
	int round = anim->nextRoundId - 1;
	struct BattleHit *hit = (prBattleHitArray + BattleHitArrayWidth * round);

	if (hit->attributes & BATTLE_HIT_ATTR_HPSTEAL) {
		/**
		 * 0x1E is the efx magic for ITEM_DARK_NOSFERATU
		 * which may directly call the hpbar-resire and cause
		 * the hpbar offset get wrong.
		 */
		if (CheckEfxMagicIsResire(gEkrSpellAnimIndex[GetAnimPosition(anim)])) {
			StartBattleAnimHitEffects(anim, type, 3, 4);
			BanimRoundUpdateEfxStatus(anim);
			return;
		}

		StartBattleAnimResireHitEffects(anim, type);
		gEfxHpBarResireFlag = 3;
		return;
	}

	StartBattleAnimHitEffects(anim, type, 3, 4);
	BanimRoundUpdateEfxStatus(anim);
}
