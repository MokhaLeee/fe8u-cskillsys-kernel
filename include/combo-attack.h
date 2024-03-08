#pragma once

#include "common-chax.h"

#define COMBO_ATK_MAX 7
#define COMBO_ATK_UID_INVALID ((s8)-1)

struct ComboAtkTarget {
    s8 uid;
    u8 weapon;
};
extern struct ComboAtkTarget gComboAtkList[COMBO_ATK_MAX + 1];

void BattleGenerateComboAtkList(void);
bool BattleComboGenerateHits(void);
