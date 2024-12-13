#ifndef LTF_H
#define LTF_H

#include "gbafe.h"
#include "common-chax.h"
#include "kernel-lib.h"

#define FORCE_DECLARE __attribute__((unused))

#define LYN_REPLACE_CHECK(name) static void const * const __attribute__((unused)) lyn_exists_check_ ## name = &name

// TODO: I can't believe I don't have that in CLib
s8 CheckEventId(int id);

// config

enum
{
	// maximum number of moves to predict
	LTF_DISPLAY_MAX_COUNT = 5,

	// display offsets relative to right of screen and transition speed
	LTF_DISPLAY_XOFF      = 5, // (offset / speed)
	LTF_DISPLAY_XOFF_STEP = 2, // speed
};

// provided by user

int IsLtfEnabled(void);

// in LTFCore.c

unsigned LTF_IsUnitReadyToMove(const struct Unit*);
unsigned LTF_IsUnitAbleToMove(const struct Unit*);

unsigned LTF_GetPhaseEffectiveUnitCount(unsigned phase);
unsigned LTF_GetPhaseAbleUnitCount(unsigned phase);

unsigned LTF_GetBerserkEffectiveUnitCount(void);
unsigned LTF_GetBerserkAbleUnitCount(void);

unsigned LTF_PredictNextPhase(const unsigned ableCounts[4], const unsigned maxCounts[4]);

// in LTFPhaseSwitch.c

void LTF_ResetUnitsStateForTurnSwitch(void);
int LTF_GotoNextTurn(struct Proc* mapMainProc);
int LTF_MapMainPhaseSwitch(struct Proc* mapMainProc);

// in LTFDisplay.c

void LTF_StartPredictionDisplay(struct Proc* parent);
void LTF_UpdatePredictionDisplay(void);

void LTF_EnablePredictionDisplay(void);
void LTF_DisablePredictionDisplay(void);

typedef struct Proc Proc;
#define ROOT_PROC_3 (Proc*)(3)

struct ObjData {
    int height;
    int data[3];
};

#define VRAM_OBJ (VRAM + 0x10000)

// in LTFTurnSwitchFx.c

// const struct ProcInstruction gProc_TurnSwitchFx[];

#endif // LTF_H
