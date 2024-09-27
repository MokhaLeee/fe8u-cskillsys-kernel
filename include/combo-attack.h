#pragma once

#include "common-chax.h"

#define COMBO_ATK_MAX 7
#define COMBO_ATK_UID_INVALID ((s8)-1)

struct ComboAtkTarget {
	s8 uid;
	u8 weapon;
};
extern struct ComboAtkTarget gComboAtkList[COMBO_ATK_MAX + 1];

void ResetComboAtkList(void);
void BattleGenerateComboAtkList(void);
bool BattleComboGenerateHits(void);
void MapAnim_PrepareNextBattleRound_CleanPreRoundCombo(void);

static inline struct Unit *GetMapAnimComboUnit(int round)
{
	if (gComboAtkList[round].uid == COMBO_ATK_UID_INVALID)
		return NULL;

	return GetUnit(gComboAtkList[round].uid);
}
