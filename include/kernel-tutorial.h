#pragma once

#include "common-chax.h"

enum KTUTORIAL_flags {
	KTUTORIAL_COMBO_ATK = 1,
	KTUTORIAL_BATTLE_SURROUNDER,
	KTUTORIAL_PREP_SKILL,
	KTUTORIAL_COMBATART_MENU,
	KTUTORIAL_RANGED_FAILOFF,
	KTUTORIAL_REAL_DAMAGE,
	KTUTORIAL_GAIDEN_BMAG,
	KTUTORIAL_GAIDEN_WMAG,
	KTUTORIAL_REORGANIZE,

	KTUTORIAL_MAX = 96,
};

enum kernel_tutorial_level {
	KTUT_LEVEL_NEVER  = 0,
	KTUT_LEVEL_MIDDLE = 1,
	KTUT_LEVEL_HIGH   = 2,
};

extern const u16 *gpKTutorialMsg;
extern u32 sKTutorialBitsHistory[3];

extern u32 sKTutorialBits[3]; /* total 96 flags */
extern u32 sKTutorialBitsHistory[3];

bool CanExecKTutorial(void);
int GetTriggerKtutorial(void);

void ClearKtutorialFlags(void);
void EMS_SaveKTutorialFlags(u8 *dst, const u32 size); /* need 0x18 Bytes */
void EMS_LoadKTutorialFlags(u8 *src, const u32 size);

void TriggerKtutorial(int flag);
bool CheckKtutFlagTriggered(int flag);
void PutKtutHistory(int flag);
