#include "common-chax.h"
#include "kernel-lib.h"
#include "combo-attack.h"

/**
 * Pre-hit
 */
extern HookProcFunc_t const gPreMapAnimBattleRoundHooks[];
extern HookProcFunc_t const * const gpPreMapAnimBattleRoundHooks;

STATIC_DECLAR void PreMapAnimBattleRound_OnStart(ProcPtr proc)
{
#ifdef CONFIG_USE_COMBO_ATTACK
    MapAnim_PrepareNextBattleRound_CleanPreRoundCombo();
#endif

    MapAnim_PrepareNextBattleRound(proc);

    gManimSt.pCurrentRound--;
    KernelStartBlockingHookProc(gpPreMapAnimBattleRoundHooks, proc);
}

STATIC_DECLAR void PreMapAnimBattleRound_OnEnd(ProcPtr proc)
{
    gManimSt.pCurrentRound++;
}

const struct ProcCmd ProcScr_MapAnimBattle_Rework[] = {
    PROC_CALL(LockGame),
    PROC_CALL(_InitFontForUIDefault),
    PROC_SLEEP(0x1),
    PROC_CALL(MapAnim_MoveCameraOntoSubject),
    PROC_SLEEP(0x2),
    PROC_CALL(MapAnim_CallBattleQuoteEvents),
    PROC_WHILE(BattleEventEngineExists),
    PROC_SLEEP(0x5),
    PROC_CALL(SetBattleMuPalette),
    PROC_CALL(SetupBattleMOVEUNITs),
    PROC_SLEEP(0x1),
    PROC_CALL(MapAnim_InitInfoBox),
    PROC_SLEEP(0xF),
PROC_LABEL(0x0),

#if CHAX
    /* Pre-hit */
    PROC_CALL(PreMapAnimBattleRound_OnStart),
    PROC_YIELD,
    PROC_CALL(PreMapAnimBattleRound_OnEnd),
#else
    PROC_REPEAT(MapAnim_PrepareNextBattleRound),
#endif

    PROC_CALL(MapAnim_DisplayRoundAnim),
    PROC_SLEEP(0x1),
    PROC_CALL(MapAnim_ShowPoisonEffectIfAny),
    PROC_SLEEP(0x1),
    PROC_SLEEP(0x5),
    PROC_GOTO(0x0),
};
