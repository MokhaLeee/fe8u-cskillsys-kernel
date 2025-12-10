#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "popup-reowrk.h"
#include "playst-expa.h"
#include "bwl.h"
#include "unit-expa.h"
#include "debuff.h"

extern u8 FreeRamSpaceTop[], UsedFreeRamSpaceTop[], FreeRamSpaceBottom[];
extern u8 FreeRamSpace2Top[], FreeRamSpace2Bottom[], UsedFreeRamSpace2Bottom[];
extern u8 FreeRamSpace3Top[], UsedFreeRamSpace3Top[], FreeRamSpace3Bottom[];
extern u8 EwramOverlay0_FreeRamSpaceTop[], EwramOverlay0_UsedFreeRamSpaceTop[], EwramOverlay0_FreeRamSpaceBottom[];

void GameInit_OverflowDetection(void)
{
	Print("Dump RAM space");
	Printf("[1] top=0x%08X, used=%06X, free=0x%06X", UsedFreeRamSpaceTop, FreeRamSpaceBottom - UsedFreeRamSpaceTop, UsedFreeRamSpaceTop - FreeRamSpaceTop);
	Printf("[2] top=0x%08X, used=%06X, free=0x%06X", UsedFreeRamSpace2Bottom, UsedFreeRamSpace2Bottom - FreeRamSpace2Top, FreeRamSpace2Bottom - UsedFreeRamSpace2Bottom);
	Printf("[3] top=0x%08X, used=%06X, free=0x%06X", UsedFreeRamSpace3Top, FreeRamSpace3Bottom - UsedFreeRamSpace3Top, UsedFreeRamSpace3Top - FreeRamSpace3Top);
	Printf("[4] top=0x%08X, used=%06X, free=0x%06X", EwramOverlay0_UsedFreeRamSpaceTop, EwramOverlay0_FreeRamSpaceBottom - EwramOverlay0_UsedFreeRamSpaceTop, EwramOverlay0_UsedFreeRamSpaceTop - EwramOverlay0_FreeRamSpaceTop);

	/* Free RAM space */
	Assert(&FreeRamSpaceTop[0] < &UsedFreeRamSpaceTop[0]);
	Assert(&UsedFreeRamSpace2Bottom[0] < &FreeRamSpace2Bottom[0]);
	Assert(&FreeRamSpace3Top[0] < &UsedFreeRamSpace3Top[0]);
	Assert(&EwramOverlay0_FreeRamSpaceTop[0] < &EwramOverlay0_UsedFreeRamSpaceTop[0]);

	/* This is effective on protection of SkillList ARM */
	Assert(sizeof(struct SkillList) == 0x50);
	Assert(sizeof(struct UnitListHeader) == 0x10);

	/* If overflowed, more free space need to be allocated */
	Assert(sizeof(gBattleActorGlobalFlag) <= 0x10);
	Assert(sizeof(gBattleTargetGlobalFlag) <= 0x10);
	Assert(sizeof(gBattleTemporaryFlag) <= 8);
	Assert(sizeof(gDmg) <= 40);

	/* PopupR */
	Assert(CHAX_POPUP_OP_ALLOC_MAX > CHAX_POPUP_OP_MAX);

	/* PlaySt expa */
	Assert(PLAYSTEXPA_BIT_USED < PLAYSTEXPA_BIT_MAX);

	/* BWL */
	Assert(sizeof(struct NewBwl) == 0x10);

	/* unit-expa sus */
	Assert(sizeof(struct DemoUnitExpaSus) == sizeof(struct DuraStatus));
}
