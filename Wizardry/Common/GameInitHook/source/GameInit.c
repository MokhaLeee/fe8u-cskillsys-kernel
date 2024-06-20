#include "common-chax.h"
#include "debug-kit.h"
#include "combat-art.h"
#include "battle-system.h"

typedef void (* GameInitHookFunc_t)(void);
// extern const GameInitHookFunc_t gGameInitHookTable[];
extern GameInitHookFunc_t const * const gpGameInitHookTable;
extern const GameInitHookFunc_t * gpExternalGameInitHook;

extern u8 FreeRamSpaceTop[], UsedFreeRamSpaceTop[];
extern u8 FreeRamSpace2Top[], UsedFreeRamSpace2Top[];
extern u8 EwramOverlay0_FreeRamSpaceTop[], EwramOverlay0_UsedFreeRamSpaceTop[];

STATIC_DECLAR void FreeRamSpaceDetection(void)
{
    Assert(&FreeRamSpaceTop[0] < &UsedFreeRamSpaceTop[0]);
    Assert(&FreeRamSpace2Top[0] < &UsedFreeRamSpace2Top[0]);
    Assert(&EwramOverlay0_FreeRamSpaceTop[0] < &EwramOverlay0_UsedFreeRamSpaceTop[0]);
}

/* LynJump! */
void StartGame(void)
{
    const GameInitHookFunc_t * it;
    struct GameCtrlProc * proc;

    SetMainUpdateRoutine(OnGameLoopMain);

    SetInterrupt_LCDVBlank(OnVBlank);

    proc = Proc_Start(gProcScr_GameControl, PROC_TREE_3);
    proc->nextAction = GAME_ACTION_EVENT_RETURN;
    proc->nextChapter = 0;
    proc->idle_status = 0;

    /* Internal hooks */
#ifdef CONFIG_USE_DEBUG
    LogInit();
#endif

    FreeRamSpaceDetection();

    /* External hooks */
    for (it = gpGameInitHookTable; *it; it++)
        (*it)();

    it = gpExternalGameInitHook;
    if (it)
        (*it)();

    return;
}

void GameInitHookFuncPad(void)
{
    return;
}
