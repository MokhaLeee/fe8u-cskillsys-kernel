#pragma once

#include "common-chax.h"

bool IsPositionValid(s8 x, s8 y);
struct Unit * GetUnitAtPosition(s8 x, s8 y);
u32 simple_div(u32 a, u32 b);
u32 simple_mod(u32 a, u32 b);

bool IsAttackerAnim(struct Anim * anim);
int GetBattleActorHpAtRound(int round);
int GetBattleTargetHpAtRound(int round);
