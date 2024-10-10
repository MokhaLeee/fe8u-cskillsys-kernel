#include "common-chax.h"
#include "skill-system.h"
#include "battle-system.h"

extern u8 FreeRamSpaceTop[], UsedFreeRamSpaceTop[];
extern u8 FreeRamSpace2Top[], UsedFreeRamSpace2Top[];
extern u8 EwramOverlay0_FreeRamSpaceTop[], EwramOverlay0_UsedFreeRamSpaceTop[];

void GameInit_OverflowDetection(void)
{
	/* Free RAM space */
	Assert(&FreeRamSpaceTop[0] < &UsedFreeRamSpaceTop[0]);
	Assert(&FreeRamSpace2Top[0] < &UsedFreeRamSpace2Top[0]);
	Assert(&EwramOverlay0_FreeRamSpaceTop[0] < &EwramOverlay0_UsedFreeRamSpaceTop[0]);

	/* This is effective on protection of SkillList ARM */
	Assert(sizeof(struct SkillList) == 0x40);
	Assert(sizeof(struct UnitListHeader) == 0x10);

	/* If overflowed, more free space need to be allocated */
	Assert(sizeof(gBattleActorGlobalFlag) <= 0x10);
	Assert(sizeof(gBattleTargetGlobalFlag) <= 0x10);
	Assert(sizeof(gBattleTemporaryFlag) <= 8);
	Assert(sizeof(gDmg) <= 40);
}
