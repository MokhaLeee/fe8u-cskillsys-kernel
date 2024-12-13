#include "LTF.h"

struct LTFPredictionDisplayProc
{
	PROC_HEADER;

	u8 prediction[LTF_DISPLAY_MAX_COUNT];
	unsigned predictionCount;

	unsigned doDisplay;
	unsigned xOff;
} LTFPredictionDisplayProc;

static u8 LTFDisplay_GetPhaseObjPalette(u8 phase)
{
	static const u8 phaseToPalIdLookup[4] = { 0xC, 0xE, 0xD, 0xF };
	return phaseToPalIdLookup[phase];
}

static void LTFDisplay_OnInit(struct LTFPredictionDisplayProc* proc)
{
	proc->predictionCount = 0;
	proc->doDisplay = FALSE;
	proc->xOff = LTF_DISPLAY_XOFF;
}

static void LTFDisplay_OnLoop(struct LTFPredictionDisplayProc* proc)
{
	// The middle one should be some variation of subtitleHelpOption, but the variable isn't named in the non-STAN clib
	if (!proc->doDisplay || gPlaySt.unk19 || GetEventTriggerState(0x84))
	{
		proc->xOff = LTF_DISPLAY_XOFF;
		return;
	}

	if (proc->xOff > 0)
		proc->xOff--;

	for (unsigned i = 0; i < proc->predictionCount; ++i)
	{
		CallARM_PushToSecondaryOAM(
			240 - ((LTF_DISPLAY_XOFF - proc->xOff) * LTF_DISPLAY_XOFF_STEP), 55 + (i * 10),
			gObject_8x8, 3 + (LTFDisplay_GetPhaseObjPalette(proc->prediction[i]) << 12));
	}
}

static const struct ProcCmd sProc_LTFPhasePredictionDisplay[] =
{
	PROC_NAME("Stan:LTF:PredictionDisplay"),

	PROC_CALL(LTFDisplay_OnInit),
	PROC_SLEEP(0),

	PROC_REPEAT(LTFDisplay_OnLoop),

	PROC_END
};

static void LTFDisplayLock_OnInit(struct Proc* proc)
{
	Proc_Start(sProc_LTFPhasePredictionDisplay, ROOT_PROC_3);
}

static void LTFDisplayLock_OnEnd(struct Proc* proc)
{
	Proc_End(Proc_Find(sProc_LTFPhasePredictionDisplay));
}

static const struct ProcCmd sProc_LTFPhasePredictionDisplayLock[] =
{
	PROC_NAME("Stan:LTF:PredictionDisplayLock"),
	PROC_SET_END_CB(LTFDisplayLock_OnEnd),

	PROC_CALL(LTFDisplayLock_OnInit),

	PROC_BLOCK,
	PROC_END
};

void LTF_StartPredictionDisplay(struct Proc* parent)
{
	Proc_Start(sProc_LTFPhasePredictionDisplayLock, parent);
}

void LTF_UpdatePredictionDisplay(void)
{
	struct LTFPredictionDisplayProc* proc = (struct LTFPredictionDisplayProc*) Proc_Find(sProc_LTFPhasePredictionDisplay);

	if (proc)
	{
		const unsigned maxCounts[4] = {
			LTF_GetPhaseEffectiveUnitCount(FACTION_BLUE),
			LTF_GetPhaseEffectiveUnitCount(FACTION_GREEN),
			LTF_GetPhaseEffectiveUnitCount(FACTION_RED),
			LTF_GetBerserkEffectiveUnitCount(),
		};

		unsigned ableCounts[4] = {
			LTF_GetPhaseAbleUnitCount(FACTION_BLUE),
			LTF_GetPhaseAbleUnitCount(FACTION_GREEN),
			LTF_GetPhaseAbleUnitCount(FACTION_RED),
			LTF_GetBerserkAbleUnitCount(),
		};

		unsigned i = 0;

		for (; i < LTF_DISPLAY_MAX_COUNT; ++i)
		{
			if ((ableCounts[0] == 0) && (ableCounts[1] == 0) && (ableCounts[2] == 0) && (ableCounts[3] == 0))
				break;

			unsigned nextPhase = LTF_PredictNextPhase(ableCounts, maxCounts);

			proc->prediction[i] = nextPhase;
			ableCounts[nextPhase]--;
		}

		proc->predictionCount = i;

		if (proc->predictionCount == 0)
			proc->doDisplay = FALSE;
	}
}

void LTF_EnablePredictionDisplay(void)
{
	struct LTFPredictionDisplayProc* proc = (struct LTFPredictionDisplayProc*) Proc_Find(sProc_LTFPhasePredictionDisplay);

	if (proc)
	{
		proc->doDisplay = TRUE;
		LTF_UpdatePredictionDisplay();
	}
}

void LTF_DisablePredictionDisplay(void)
{
	struct LTFPredictionDisplayProc* proc = (struct LTFPredictionDisplayProc*) Proc_Find(sProc_LTFPhasePredictionDisplay);

	if (proc)
	{
		proc->doDisplay = FALSE;
	}
}

// void LTF_DisplayPhasePredictions(struct Proc* maptaskProc) {
// 	static const u8 phaseToPalIdLookup[4] = { 0xC, 0xE, 0xD, 0xF };

// 	const unsigned maxCounts[4] = {
// 		LTF_GetPhaseEffectiveUnitCount(FACTION_BLUE),
// 		LTF_GetPhaseEffectiveUnitCount(FACTION_GREEN),
// 		LTF_GetPhaseEffectiveUnitCount(FACTION_RED),
// 		LTF_GetBerserkEffectiveUnitCount(),
// 	};

// 	unsigned ableCounts[4] = {
// 		LTF_GetPhaseAbleUnitCount(FACTION_BLUE),
// 		LTF_GetPhaseAbleUnitCount(FACTION_GREEN),
// 		LTF_GetPhaseAbleUnitCount(FACTION_RED),
// 		LTF_GetBerserkAbleUnitCount(),
// 	};

// 	for (unsigned i = 0; i < 5; ++i) {
// 		if ((ableCounts[0] == 0) && (ableCounts[1] == 0) && (ableCounts[2] == 0) && (ableCounts[3] == 0))
// 			break;

// 		unsigned nextPhase = LTF_PredictNextPhase(ableCounts, maxCounts);

// 		ableCounts[nextPhase]--;

// 		CallARM_PushToSecondaryOAM(
// 			240 - 10, 55 + (i * 10),
// 			gObject_8x8, 3 + (phaseToPalIdLookup[nextPhase] << 12)
// 		);
// 	}
// }
