#pragma once

#include "common-chax.h"

extern HookProcFunc_t const *const gpPostActionFuncs;
extern bool (*gpExternalPostActionHook)(ProcPtr proc);

struct PostActionCommonBuffer {
	/* 00 */ int total_damage_actor;
	/* 04 */ int total_damage_target;

	/* max_len = 0x20 */
};

extern EWRAM_OVERLAY(0) struct PostActionCommonBuffer gPostActionCommonBuffer;
