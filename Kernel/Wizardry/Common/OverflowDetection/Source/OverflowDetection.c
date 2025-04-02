#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"
#include "popup-reowrk.h"
#include "playst-expa.h"

extern u8 FreeRamSpaceTop[], UsedFreeRamSpaceTop[], FreeRamSpaceBottom[];
extern u8 FreeRamSpace2Top[], UsedFreeRamSpace2Top[], FreeRamSpace2Bottom[];
extern u8 EwramOverlay0_FreeRamSpaceTop[], EwramOverlay0_UsedFreeRamSpaceTop[], EwramOverlay0_FreeRamSpaceBottom[];

void GameInit_OverflowDetection(void)
{
	Print("Dump RAM space");
	Printf("[1] top=0x%08X, used=%06X, free=0x%06X", UsedFreeRamSpaceTop, FreeRamSpaceBottom - UsedFreeRamSpaceTop, UsedFreeRamSpaceTop - FreeRamSpaceTop);
	Printf("[2] top=0x%08X, used=%06X, free=0x%06X", UsedFreeRamSpace2Top, FreeRamSpace2Bottom - UsedFreeRamSpace2Top, UsedFreeRamSpace2Top - FreeRamSpace2Top);
	Printf("[3] top=0x%08X, used=%06X, free=0x%06X", EwramOverlay0_UsedFreeRamSpaceTop, EwramOverlay0_FreeRamSpaceBottom - EwramOverlay0_UsedFreeRamSpaceTop, EwramOverlay0_UsedFreeRamSpaceTop - EwramOverlay0_FreeRamSpaceTop);

	/* Free RAM space */
	Assert(&FreeRamSpaceTop[0] < &UsedFreeRamSpaceTop[0]);
	Assert(&FreeRamSpace2Top[0] < &UsedFreeRamSpace2Top[0]);
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
}
