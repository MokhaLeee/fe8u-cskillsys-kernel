#include "global.h"
#include "bm.h"

#include "common-chax.h"

extern struct {
    void (*func)(void);
} gPhaseSwitchHooks[];

/* LynJump */
int BmMain_ChangePhase(void)
{
    int i;

    /* Vanilla */
    ClearActiveFactionGrayedStates();
    RefreshUnitSprites();
    SwitchPhases();

#if CHAX
    for (i = 0; !IS_ROM_THUMB(gPhaseSwitchHooks[i].func); i++)
        gPhaseSwitchHooks[i].func();
#endif

    /* Vanilla */
    if (RunPhaseSwitchEvents() == 1)
        return 0;

    return 1;
}
