#pragma once

#include "common-chax.h"

enum KTUTORIAL_flags {
    KTUTORIAL_COMBO_ATK = 1,
    KTUTORIAL_BATTLE_SURROUNDER,
    KTUTORIAL_PREP_SKILL,
    KTUTORIAL_COMBATART_MENU,
    KTUTORIAL_RANGED_FAILOFF,
    KTUTORIAL_REAL_DAMAGE,

    KTUTORIAL_MAX = 96,
};

void ClearKtutorialFlags(void);
void EMS_SaveKTutorialFlags(u8 * dst, const u32 size); /* need 0x18 Bytes */
void EMS_LoadKTutorialFlags(u8 * src, const u32 size);

void TriggerKtutorial(int flag);
bool CheckKtutFlagTriggered(int flag);
void PutKtutHistory(int flag);
