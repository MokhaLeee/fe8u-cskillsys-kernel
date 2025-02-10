#include "common-chax.h"
#include "kernel-lib.h"
#include "combo-attack.h"

/**
 * Pre-hit
 */
extern HookProcFunc_t const gPreMapAnimBattleRoundHooks[];
extern HookProcFunc_t const *const gpPreMapAnimBattleRoundHooks;

STATIC_DECLAR void PreMapAnimBattleRound_OnStart(ProcPtr proc)
{
#ifdef CONFIG_USE_COMBO_ATTACK
	MapAnim_PrepareNextBattleRound_CleanPreRoundCombo();
#endif

	/**
	 * This is part of function MapAnim_PrepareNextBattleRound()
	 * we need to put it external
	 */
	if (gManimSt.pCurrentRound->info & BATTLE_HIT_INFO_END) {
		Proc_Break(proc);
		Proc_GotoScript(proc, ProcScr_MapAnimEnd);
		return;
	}
	MapAnim_PrepareNextBattleRound(proc);

	gManimSt.pCurrentRound--;
	KernelStartBlockingHookProc(gpPreMapAnimBattleRoundHooks, proc);
}

STATIC_DECLAR void PreMapAnimBattleRound_OnEnd(ProcPtr proc)
{
	struct Unit *unit = gManimSt.actor[gManimSt.subjectActorId].unit;

	EnsureCameraOntoPosition(proc, unit->xPos, unit->yPos);
	gManimSt.pCurrentRound++;
}

const struct ProcCmd ProcScr_MapAnimBattle_Rework[] = {
	PROC_CALL(LockGame),
	PROC_CALL(MapAnim_PrepareBattleTalk),
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
	PROC_YIELD,
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
