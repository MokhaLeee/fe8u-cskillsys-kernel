#include "common-chax.h"
#include "constants/skills.h"

extern u8 ARM_MapFloodCoreRe[];

const int gSID_Pass = SID_Pass;

void GameInit_RedirectMapFloodFunc(void)
{
    gUnknown_03003128 = (void *)ARM_MapFloodCoreRe;
}
