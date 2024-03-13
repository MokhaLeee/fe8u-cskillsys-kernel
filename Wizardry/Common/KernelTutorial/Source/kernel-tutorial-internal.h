#pragma once

#include "common-chax.h"
#include "kernel-tutorial.h"

enum kernel_tutorial_level {
    KTUT_LEVEL_NEVER  = 0,
    KTUT_LEVEL_MIDDLE = 1,
    KTUT_LEVEL_HIGH   = 2,
};
extern const int gConfigKTutorialLevel;

extern const u16 * gpKTutorialMsg;
extern u32 sKTutorialBitsHistory[3];

bool CanExecKTutorial(void);
int GetTriggerKtutorial(void);
void PutKtutHistory(int flag);
