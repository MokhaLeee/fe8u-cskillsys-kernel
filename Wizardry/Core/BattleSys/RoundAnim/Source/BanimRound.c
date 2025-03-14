#include <common-chax.h>
#include <strmag.h>
#include <battle-system.h>

#define LOCAL_TRACE 0

STATIC_DECLAR const u16 round_types_normal_phy[EKR_DISTANCE_MAX] = {
	[EKR_DISTANCE_CLOSE]       = ANIM_ROUND_HIT_CLOSE,
	[EKR_DISTANCE_FAR]         = ANIM_ROUND_NONCRIT_FAR,
	[EKR_DISTANCE_FARFAR]      = ANIM_ROUND_NONCRIT_FAR,
	[EKR_DISTANCE_MONOCOMBAT]  = ANIM_ROUND_HIT_CLOSE,
	[EKR_DISTANCE_PROMOTION]   = ANIM_ROUND_HIT_CLOSE
};

STATIC_DECLAR const u16 round_types_normal_mag[EKR_DISTANCE_MAX] = {
	[EKR_DISTANCE_CLOSE]       = ANIM_ROUND_NONCRIT_FAR,
	[EKR_DISTANCE_FAR]         = ANIM_ROUND_NONCRIT_FAR,
	[EKR_DISTANCE_FARFAR]      = ANIM_ROUND_NONCRIT_FAR,
	[EKR_DISTANCE_MONOCOMBAT]  = ANIM_ROUND_HIT_CLOSE,
	[EKR_DISTANCE_PROMOTION]   = ANIM_ROUND_HIT_CLOSE
};

STATIC_DECLAR const u16 round_types_crital_phy[EKR_DISTANCE_MAX] = {
	[EKR_DISTANCE_CLOSE]       = ANIM_ROUND_CRIT_CLOSE,
	[EKR_DISTANCE_FAR]         = ANIM_ROUND_CRIT_FAR,
	[EKR_DISTANCE_FARFAR]      = ANIM_ROUND_CRIT_FAR,
	[EKR_DISTANCE_MONOCOMBAT]  = ANIM_ROUND_CRIT_CLOSE,
	[EKR_DISTANCE_PROMOTION]   = ANIM_ROUND_CRIT_CLOSE
};

STATIC_DECLAR const u16 round_types_crital_mag[EKR_DISTANCE_MAX] = {
	[EKR_DISTANCE_CLOSE]       = ANIM_ROUND_CRIT_FAR,
	[EKR_DISTANCE_FAR]         = ANIM_ROUND_CRIT_FAR,
	[EKR_DISTANCE_FARFAR]      = ANIM_ROUND_CRIT_FAR,
	[EKR_DISTANCE_MONOCOMBAT]  = ANIM_ROUND_CRIT_CLOSE,
	[EKR_DISTANCE_PROMOTION]   = ANIM_ROUND_CRIT_CLOSE
};

STATIC_DECLAR const u16 round_types_miss_phy[EKR_DISTANCE_MAX] = {
	[EKR_DISTANCE_CLOSE]       = ANIM_ROUND_MISS_CLOSE,
	[EKR_DISTANCE_FAR]         = ANIM_ROUND_NONCRIT_FAR,
	[EKR_DISTANCE_FARFAR]      = ANIM_ROUND_NONCRIT_FAR,
	[EKR_DISTANCE_MONOCOMBAT]  = ANIM_ROUND_MISS_CLOSE,
	[EKR_DISTANCE_PROMOTION]   = ANIM_ROUND_MISS_CLOSE
};

STATIC_DECLAR const u16 defender_round_types_dodge[EKR_DISTANCE_MAX] = {
	[EKR_DISTANCE_CLOSE]       = ANIM_ROUND_TAKING_MISS_CLOSE,
	[EKR_DISTANCE_FAR]         = ANIM_ROUND_TAKING_MISS_FAR,
	[EKR_DISTANCE_FARFAR]      = ANIM_ROUND_TAKING_MISS_FAR,
	[EKR_DISTANCE_MONOCOMBAT]  = ANIM_ROUND_TAKING_MISS_CLOSE,
	[EKR_DISTANCE_PROMOTION]   = ANIM_ROUND_TAKING_MISS_CLOSE
};

STATIC_DECLAR const u16 defender_round_types_hitted[EKR_DISTANCE_MAX] = {
	[EKR_DISTANCE_CLOSE]       = ANIM_ROUND_TAKING_HIT_CLOSE,
	[EKR_DISTANCE_FAR]         = ANIM_ROUND_TAKING_HIT_FAR,
	[EKR_DISTANCE_FARFAR]      = ANIM_ROUND_TAKING_HIT_FAR,
	[EKR_DISTANCE_MONOCOMBAT]  = ANIM_ROUND_TAKING_HIT_CLOSE,
	[EKR_DISTANCE_PROMOTION]   = ANIM_ROUND_TAKING_HIT_CLOSE
};

LYN_REPLACE_CHECK(ParseBattleHitToBanimCmd);
void ParseBattleHitToBanimCmd(void)
{
	u32 i;
	int new_hp;
	int distance_modes[2];
	int hplut_offs[2];

	#define HpLutAdvance(pos, chg) \
	do { \
		new_hp = gEfxHpLutRe[hplut_offs[pos] * 2 + pos] + (chg); \
		if (new_hp < 0) \
			new_hp = 0; \
		hplut_offs[pos]++; \
		gEfxHpLutRe[hplut_offs[pos] * 2 + pos] = new_hp; \
	} while (0)

	for (i = 0; i < ARRAY_COUNT(gAnimRoundDataRe); i++)
		gAnimRoundDataRe[i] = -1;

	for (i = 2; i < ARRAY_COUNT(gEfxHpLutRe); i++)
		gEfxHpLutRe[i] = -1;

	hplut_offs[POS_L] = hplut_offs[POS_R] = 0;
	gpEkrTriangleUnits[POS_L] = gpEkrTriangleUnits[POS_R] = NULL;

	if (gEkrDistanceType == EKR_DISTANCE_PROMOTION) {
		/* Promotion */
		gAnimRoundDataRe[POS_L] = gAnimRoundDataRe[POS_R] = ANIM_ROUND_TAKING_MISS_CLOSE;
		return;
	}

	if (gBattleStats.config & BATTLE_CONFIG_REFRESH) {
		gAnimRoundDataRe[POS_L] = ANIM_ROUND_TAKING_HIT_CLOSE;
		gAnimRoundDataRe[POS_R] = ANIM_ROUND_HIT_CLOSE;
		return;
	}

	distance_modes[POS_L] = distance_modes[POS_R] = gEkrDistanceType;
	if (gEkrDistanceType == EKR_DISTANCE_CLOSE) {
		switch (ITEM_INDEX(gpEkrBattleUnitLeft->weaponBefore)) {
		case ITEM_SWORD_RUNESWORD:
		case ITEM_AXE_HANDAXE:
		case ITEM_AXE_TOMAHAWK:
		case ITEM_AXE_HATCHET:
			distance_modes[POS_L] = EKR_DISTANCE_FAR;
			break;
		}

		switch (ITEM_INDEX(gpEkrBattleUnitRight->weaponBefore)) {
		case ITEM_SWORD_RUNESWORD:
		case ITEM_AXE_HANDAXE:
		case ITEM_AXE_TOMAHAWK:
		case ITEM_AXE_HATCHET:
			distance_modes[POS_R] = EKR_DISTANCE_FAR;
			break;
		}
	}

	gEfxHpLutRe[POS_L + 2 * 0] = gEkrGaugeHp[POS_L];
	gEfxHpLutRe[POS_R + 2 * 0] = gEkrGaugeHp[POS_R];

	for (i = 0; i < NEW_BATTLE_HIT_MAX; i++) {
		int attacker_pos, defender_pos;
		struct BattleUnit *actor;
		struct BattleHit *hit = prBattleHitArray + i * BattleHitArrayWidth;
		struct ExtBattleHit *exthit = gExtBattleHitArray + i;
		u16 *anim_round = gAnimRoundDataRe + i * 2;

		if (hit->info & BATTLE_HIT_INFO_END)
			break;

		/**
		 * Check the current round attacker position.
		 *
		 * It is a wrong name on gBanimPositionIsEnemy,
		 * which is not an identifier for enemy, but for whether is target or not.
		 * So it is better to be called as "gBanimPosIsTarget"
		 *
		 * The vanilla function GetBanimAllyPosition() is also wrong named, which should be "GetBanimBattleActorPosition"
		 */
		if (!!gBanimPositionIsEnemy[POS_L] == !!(hit->info & BATTLE_HIT_INFO_RETALIATION)) {
			attacker_pos = POS_L;
			defender_pos = POS_R;
			actor = gpEkrBattleUnitLeft;
		} else {
			attacker_pos = POS_R;
			defender_pos = POS_L;
			actor = gpEkrBattleUnitRight;
		}

		if (i == 0)
			gEkrInitialHitSide = attacker_pos;

		if (hit->attributes & BATTLE_HIT_ATTR_TATTACK) {
			gpEkrTriangleUnits[0] = gBattleStats.taUnitA;
			gpEkrTriangleUnits[1] = gBattleStats.taUnitB;
		}

		/**
		 * Attacker round base types
		 */
		if (hit->attributes & BATTLE_HIT_ATTR_CRIT) {
			if (!IsMagicAttack(actor))
				anim_round[attacker_pos] = round_types_crital_phy[distance_modes[attacker_pos]];
			else
				anim_round[attacker_pos] = round_types_crital_mag[distance_modes[attacker_pos]];
		} else {
			if (!IsMagicAttack(actor))
				anim_round[attacker_pos] = round_types_normal_phy[distance_modes[attacker_pos]];
			else
				anim_round[attacker_pos] = round_types_normal_mag[distance_modes[attacker_pos]];
		}

		/**
		 * Defender round base types
		 */
		if (hit->attributes & BATTLE_HIT_ATTR_MISS) {
			if (!IsMagicAttack(actor))
				anim_round[attacker_pos] = round_types_miss_phy[distance_modes[attacker_pos]];

			anim_round[defender_pos] = defender_round_types_dodge[distance_modes[defender_pos]];
		} else
			anim_round[defender_pos] = defender_round_types_hitted[distance_modes[defender_pos]];

#if CHAX
		/**
		 * Hp cost
		 */
		if (exthit->hp_cost > 0)
			HpLutAdvance(attacker_pos, -exthit->hp_cost);
#endif

		if (!(hit->attributes & BATTLE_HIT_ATTR_MISS)) {
			/**
			 * Devil
			 */
			if (hit->attributes & BATTLE_HIT_ATTR_DEVIL) {
				HpLutAdvance(attacker_pos, -hit->hpChange);

				/**
				 * If devil triggered, then you will lose everything :(
				 */
				LTRACEF("Round %d: pos=%d, DEVIL!", i, attacker_pos);
				anim_round[attacker_pos] |= ANIM_ROUND_DEVIL;
				continue;
			}

			/**
			 * Normal damage
			 */
			HpLutAdvance(defender_pos, -hit->hpChange);

			/**
			 * Hp drain
			 */
			if (exthit->hp_drain > 0 || hit->attributes & BATTLE_HIT_ATTR_HPSTEAL) {
				/**
				 * Well apparently this is a bug, but maybe we can give a correct
				 */
				if (exthit->hp_drain <= 0)
					HpLutAdvance(attacker_pos, hit->hpChange);
				else
					HpLutAdvance(attacker_pos, exthit->hp_drain);
			}

			if (hit->attributes & BATTLE_HIT_ATTR_POISON)
				anim_round[defender_pos] |= ANIM_ROUND_POISON;

			if (hit->attributes & BATTLE_HIT_ATTR_SILENCER)
				anim_round[defender_pos] |= ANIM_ROUND_SILENCER;

			if (hit->attributes & BATTLE_HIT_ATTR_PIERCE)
				anim_round[defender_pos] |= ANIM_ROUND_PIERCE;

			if (hit->attributes & BATTLE_HIT_ATTR_SURESHOT)
				anim_round[defender_pos] |= ANIM_ROUND_SURE_SHOT;

			if (hit->attributes & BATTLE_HIT_ATTR_GREATSHLD)
				anim_round[defender_pos] |= ANIM_ROUND_GREAT_SHIELD;
		}

		LTRACEF("Round %d: pos=%d, 0x%04X 0x%04X",
				i, attacker_pos, anim_round[attacker_pos], anim_round[defender_pos]);
	}
	#undef HpLutAdvance
}
