#pragma once

#include "common-chax.h"

bool IsPositionValid(s8 x, s8 y);
struct Unit * GetUnitAtPosition(s8 x, s8 y);

u32 simple_div(u32 a, u32 b);
u32 simple_mod(u32 a, u32 b);

bool IsAttackerAnim(struct Anim * anim);
int GetBattleActorHpAtRound(int round);
int GetBattleTargetHpAtRound(int round);

ProcPtr KernelCallEvent(const EventScr * eventscr, u8 execType, ProcPtr parent);
void KernelStartBlockingHookProc(HookProcFunc_t const * hook_list, ProcPtr parent);

extern const struct Vec1 gVecs_1x1[4];
extern const struct Vec1 gVecs_2x2[12];
extern const struct Vec1 gVecs_3x3[24];
extern const u8 gRange1_In2x2[ARRAY_COUNT(gVecs_2x2)];
extern const u8 gRange1_In3x3[ARRAY_COUNT(gVecs_3x3)];
extern const u8 gRange2_In3x3[ARRAY_COUNT(gVecs_3x3)];
