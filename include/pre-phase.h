#pragma once

#include "common-chax.h"
#include "kernel-lib.h"

struct ProcPrePhase {
	PROC_HEADER;
	int index;
};

typedef bool (*PrePhaseFunc_t)(struct ProcPrePhase *proc);
extern const PrePhaseFunc_t gPrePhaseFuncs[];

/**
 * Misc
 */
struct PrePhaseSkillBuf {
	/* AversaNight */
	s8 AversaNight_Mhp;

	/* Chill */
	s8 Chill_ActorUids[UNIT_STATUS_MAX];
	s8 Chill_TargetUids[UNIT_STATUS_MAX];
	s8 Chill_MaxStatusRef[UNIT_STATUS_MAX];
};

extern struct PrePhaseSkillBuf gPrePhaseSkillBuf;

void ExecArmorMarch(struct Unit *unit);
