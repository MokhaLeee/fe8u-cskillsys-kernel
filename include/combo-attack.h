#pragma once

#include "common-chax.h"

#define COMBO_ATK_MAX 7
#define COMBO_ATK_UID_INVALID ((s8)-1)

/* User defined whether use combo-atk */
extern const int gpConfigUseComboAtk;

struct ComboAtkTarget {
    s8 uid;
    u8 weapon;
};
extern struct ComboAtkTarget gComboAtkList[COMBO_ATK_MAX + 1];

void ResetComboAtkList(void);
void BattleGenerateComboAtkList(void);
bool BattleComboGenerateHits(void);
