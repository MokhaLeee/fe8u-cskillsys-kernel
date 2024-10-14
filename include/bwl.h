#pragma once

#include "common-chax.h"

#define NEW_BWL_ARRAY_NUM CONFIG_UNIT_AMT_ALLY

struct NewBwl {
	/* vanilla */
	u32 battleAmt : 12;
	u32 winAmt    : 11;
	u32 lossAmt   : 8;
	u32 levelGain : 9;

	/* bwl support */
	u8 supports[UNIT_SUPPORT_MAX_COUNT];

	u8 _pad_[0x10 - 0x0C];

} BITPACKED;

bool CheckHasBwl(u8 pid);
struct NewBwl *GetNewBwl(u8 pid);

// extern const s8 gClassPreLoadHiddenLevel[0x100];
extern s8 const *const gpClassPreLoadHiddenLevel;

void NewBwlRecordHiddenLevel(struct Unit *unit);
int GetUnitHiddenLevel(struct Unit *unit);
void UnitHiddenLevelPreLoad(struct Unit *unit);
