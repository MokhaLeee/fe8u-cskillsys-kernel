#pragma once

#include "common-chax.h"

#define NEW_BWL_ARRAY_NUM CONFIG_UNIT_AMT_ALLY

struct NewBwl {
	/* vanilla */
	u32 battleAmt : 10;
	u32 winAmt    : 8;
	u32 lossAmt   : 5;
	u32 levelGain : 9;

	u8 _pad_[5];

	/* bwl support */
	u8 supports[UNIT_SUPPORT_MAX_COUNT];
} BITPACKED;

#define BWL_BIT_MAX(bits) ((1 << (bits)) - 1)
#define BWL_BATTLEAMT_MAX BWL_BIT_MAX(10)
#define BWL_WINAMT_MAX    BWL_BIT_MAX(8)
#define BWL_LOSSAMT_MAX   BWL_BIT_MAX(5)

bool CheckHasBwl(u8 pid);
struct NewBwl *GetNewBwl(u8 pid);

// extern const s8 gClassPreLoadHiddenLevel[0x100];
extern s8 const *const gpClassPreLoadHiddenLevel;

void NewBwlRecordHiddenLevel(struct Unit *unit);
int GetUnitHiddenLevel(struct Unit *unit);
void UnitHiddenLevelPreLoad(struct Unit *unit);

// see limition rule 5
void WriteUnitLevelSafe(struct Unit *unit, int new_level);
extern void (*prWriteUnitLevelSafe)(struct Unit *unit, int new_level);
