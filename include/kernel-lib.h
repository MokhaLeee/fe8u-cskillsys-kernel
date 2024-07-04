#pragma once

#include "common-chax.h"

/**
 * map.c
 */
bool IsPositionValid(s8 x, s8 y);
struct Unit * GetUnitAtPosition(s8 x, s8 y);

void MapAnim_CommonInit(void);
void MapAnim_CommonEnd(void);

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

/**
 * faction
 */
#define GetFactionUnitAmount(faction)                    \
(                                                        \
    (faction) == FACTION_BLUE  ? CONFIG_UNIT_AMT_ALLY  : \
    (faction) == FACTION_RED   ? CONFIG_UNIT_AMT_ENEMY : \
    (faction) == FACTION_GREEN ? CONFIG_UNIT_AMT_NPC   : \
    0                                                    \
)

/**
 * Bits
 */
static inline void _BIT_SET(u32 * bits, int idx)
{
    bits[idx / 32] |= 1 << (idx % 32);
}

static inline void _BIT_CLR(u32 * bits, int idx)
{
    bits[idx / 32] &= ~(1 << (idx % 32));
}

static inline bool _BIT_CHK(u32 * bits, int idx)
{
    return !!(bits[idx / 32] & (1 << (idx % 32)));
}
