#include "common-chax.h"

extern u8 FreeRamSpaceTop[], UsedFreeRamSpaceTop[];
extern u8 FreeRamSpace2Top[], UsedFreeRamSpace2Top[];
extern u8 EwramOverlay0_FreeRamSpaceTop[], EwramOverlay0_UsedFreeRamSpaceTop[];

void GameInit_OverflowDetection(void)
{
    /* Free RAM space */
    Assert(&FreeRamSpaceTop[0] < &UsedFreeRamSpaceTop[0]);
    Assert(&FreeRamSpace2Top[0] < &UsedFreeRamSpace2Top[0]);
    Assert(&EwramOverlay0_FreeRamSpaceTop[0] < &EwramOverlay0_UsedFreeRamSpaceTop[0]);
}
