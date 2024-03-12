#pragma once

#include "common-chax.h"

enum kernel_tutorial_level {
    KTUT_LEVEL_NEVER  = 0,
    KTUT_LEVEL_MIDDLE = 1,
    KTUT_LEVEL_HIGH   = 2,
};
extern int gConfigKTutorialLevel;

enum KTUTORIAL_flags {
    KTUTORIAL_COMBO_ATK = 1,
    KTUTORIAL_BATTLE_SURROUNDER,
};

void ResetKTutorialFlags(void);
void EMS_SaveKTutorialFlags(u8 * dst, const u32 size); /* need 0x18 Bytes */
void EMS_LoadKTutorialFlags(u8 * src, const u32 size);

void SetKTutorialFlag(int flag);
