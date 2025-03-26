#pragma once

#include "common-chax.h"

/** 
 * Individual_animation_ea_2
 */
struct IndividualAnimConf {
	u8 pid, jid;
	u8 _pad_[2];
	const struct BattleAnimDef *anim_conf;
};

const struct IndividualAnimConf *GetPrIndividualAnimConf(void);

/**
 * Bow2Decrease
 */
bool CheckWeaponCostForMissedBowAttack(struct BattleUnit *actor);

/**
 * Devil
 */
bool RollBaseDevilCheck(struct BattleUnit *bu);
