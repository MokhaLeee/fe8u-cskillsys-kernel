#pragma once

#include "common-chax.h"

/**
 * map.c
 */
bool IsPositionValid(s8 x, s8 y);
struct Unit * GetUnitAtPosition(s8 x, s8 y);

/**
 * math.c
 */
u32 simple_div(u32 a, u32 b);
u32 simple_mod(u32 a, u32 b);

/**
 * banim.c
 */
bool IsAttackerAnim(struct Anim * anim);
int GetBattleActorHpAtRound(int round);
int GetBattleTargetHpAtRound(int round);

/**
 * event-call.c
 */
ProcPtr KernelCallEvent(const EventScr * eventscr, u8 execType, ProcPtr parent);

/**
 * hook-proc.c
 */
void KernelStartBlockingHookProc(HookProcFunc_t const * hook_list, ProcPtr parent);

/**
 * ranges.c
 */
#define ARRAY_COUNT_RANGE1x1 4
#define ARRAY_COUNT_RANGE2x2 12
#define ARRAY_COUNT_RANGE3x3 24

extern const struct Vec1 gVecs_1x1[ARRAY_COUNT_RANGE1x1];
extern const struct Vec1 gVecs_2x2[ARRAY_COUNT_RANGE2x2];
extern const struct Vec1 gVecs_3x3[ARRAY_COUNT_RANGE3x3];

extern const u8 gRange1_In2x2[ARRAY_COUNT_RANGE2x2];
extern const u8 gRange1_In3x3[ARRAY_COUNT_RANGE3x3];
extern const u8 gRange2_In3x3[ARRAY_COUNT_RANGE3x3];
