#include "common-chax.h"
#include "debug-kit.h"
#include "combat-art.h"
#include "battle-system.h"

typedef void (*GameInitHookFunc_t)(void);
// extern const GameInitHookFunc_t gGameInitHookTable[];
extern GameInitHookFunc_t const *const gpGameInitHookTable;
extern const GameInitHookFunc_t *gpExternalGameInitHook;

LYN_REPLACE_CHECK(StartGame);
void StartGame(void)
{
	const GameInitHookFunc_t *it;
	struct GameCtrlProc *proc;

	SetMainUpdateRoutine(OnMain);

	SetInterrupt_LCDVBlank(OnVBlank);

	proc = Proc_Start(gProcScr_GameControl, PROC_TREE_3);
	proc->nextAction = GAME_ACTION_EVENT_RETURN;
	proc->nextChapter = 0;
	proc->idle_status = 0;

	/* Internal hooks */
#if CHAX
	LogInit();
#endif

	/* External hooks */
	for (it = gpGameInitHookTable; *it; it++)
		(*it)();

	it = gpExternalGameInitHook;
	if (it)
		(*it)();
}

void GameInitHookFuncPad(void) {}
