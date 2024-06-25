#include "common-chax.h"

struct PhaseSwitchHook { void (*func)(void); };
// extern const struct PhaseSwitchHook gPhaseSwitchHooks[];
extern struct PhaseSwitchHook const * const gpPhaseSwitchHooks;

/* LynJump */
int BmMain_ChangePhase(void)
{
    int i;

    /* Vanilla */
    ClearActiveFactionGrayedStates();
    RefreshUnitSprites();
    SwitchPhases();

#if CHAX
    for (i = 0; !IS_ROM_THUMB(gpPhaseSwitchHooks[i].func); i++)
        gpPhaseSwitchHooks[i].func();
#endif

    /* Vanilla */
    if (RunPhaseSwitchEvents() == 1)
        return 0;

    return 1;
}
