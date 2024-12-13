#include "LTF.h"

// extern unsigned GetPhaseAbleUnitCount(unsigned phase) __attribute__((long_call));
// extern void ProcessSupportGains(void) __attribute__((long_call));
// extern s8 RunPhaseSwitchEvents(void) __attribute__((long_call));

void LTF_ResetUnitsStateForTurnSwitch(void)
{
	for (unsigned index = 1; index < 0x100; ++index)
	{
		struct Unit* unit = GetUnit(index);

		if (UNIT_IS_VALID(unit))
			unit->state &= ~(US_UNSELECTABLE | US_HAS_MOVED | US_HAS_MOVED_AI);
	}
}

int LTF_GotoNextTurn(struct Proc* mapMainProc)
{
	if (gPlaySt.chapterTurnNumber < 999)
		gPlaySt.chapterTurnNumber++;

	RefreshUnitSprites();

	Proc_Goto(mapMainProc, 9); // goto turn start

	if (RunPhaseSwitchEvents())
		return 0; // Events are running, proc yield

	return 1;
}

int LTF_MapMainPhaseSwitch(struct Proc* mapMainProc)
{
	const unsigned maxCounts[4] =
	{
		LTF_GetPhaseEffectiveUnitCount(FACTION_BLUE),
		LTF_GetPhaseEffectiveUnitCount(FACTION_GREEN),
		LTF_GetPhaseEffectiveUnitCount(FACTION_RED),
		LTF_GetBerserkEffectiveUnitCount(),
	};

	const unsigned ableCounts[4] =
	{
		LTF_GetPhaseAbleUnitCount(FACTION_BLUE),
		LTF_GetPhaseAbleUnitCount(FACTION_GREEN),
		LTF_GetPhaseAbleUnitCount(FACTION_RED),
		LTF_GetBerserkAbleUnitCount(),
	};

	LTF_UpdatePredictionDisplay();

	if ((ableCounts[0] == 0) && (ableCounts[1] == 0) && (ableCounts[2] == 0) && (ableCounts[3] == 0))
		return LTF_GotoNextTurn(mapMainProc);

	RefreshUnitSprites();

	unsigned nextPhase = LTF_PredictNextPhase(ableCounts, maxCounts);

	if (nextPhase == 3)
		Proc_Goto(mapMainProc, 12); // goto berserk phase (new label! see LazberianTurnFlow.event)
	else
		gPlaySt.faction = nextPhase << 6;

	return 1;
}
