#pragma once

#include "common-chax.h"

/**
 * designer-config.c
 */
struct KernelDesigerConfig {
	u8 equip_skill_en;
	u8 gen_new_scroll;
	u8 max_equipable_skill;
	u8 remove_move_path;
	u8 use_chinese_character;
	u8 kernel_tutorial_level;
	u8 combo_attack_en;
	u8 battle_surrend_en;
	u8 hit_decrease_on_range;
	u8 debug_autoload_skills;

	u8 guaranteed_lvup;
	u8 lvup_mode_tutorial;
	u8 lvup_mode_normal;
	u8 lvup_mode_hard;

	u8 unit_page_style, skil_page_style;
};

extern const struct KernelDesigerConfig *const gpKernelDesigerConfig;

/**
 * map.c
 */
bool IsPositionValid(s8 x, s8 y);
struct Unit *GetUnitAtPosition(s8 x, s8 y);

/**
 * mapanim.c
 */
void MapAnim_CommonInit(void);
void MapAnim_CommonEnd(void);

/**
 * math.c
 */
#define DIV_ROUND_CLOSEST(x, divisor)( \
{ \
	typeof(x) __x = x; \
	typeof(divisor) __d = divisor; \
	(((typeof(x))-1) > 0 || \
	((typeof(divisor))-1) > 0 || (__x) > 0) ? \
		(((__x) + ((__d) / 2)) / (__d)) : \
		(((__x) - ((__d) / 2)) / (__d)); \
} \
)

u32 simple_div(u32 a, u32 b);
u32 simple_mod(u32 a, u32 b);

/**
 * banim.c
 */
bool IsAttackerAnim(struct Anim *anim);

/**
 * event-call.c
 */
ProcPtr KernelCallEvent(const EventScr *eventscr, u8 execType, ProcPtr parent);

/**
 * hook-proc.c
 */
void KernelStartBlockingHookProc(HookProcFunc_t const *hook_list, ProcPtr parent);

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
#define GetFactionUnitAmount(faction) \
( \
	(faction) == FACTION_BLUE  ? CONFIG_UNIT_AMT_ALLY  : \
	(faction) == FACTION_RED   ? CONFIG_UNIT_AMT_ENEMY : \
	(faction) == FACTION_GREEN ? CONFIG_UNIT_AMT_NPC   : \
	0 \
)

/**
 * Bits
 */
static inline void _BIT_SET(u32 *bits, int idx)
{
	bits[idx / 32] |= 1 << (idx % 32);
}

static inline void _BIT_CLR(u32 *bits, int idx)
{
	bits[idx / 32] &= ~(1 << (idx % 32));
}

static inline bool _BIT_CHK(u32 *bits, int idx)
{
	return !!(bits[idx / 32] & (1 << (idx % 32)));
}

/**
 * pad.c
 */
void KernelPad1(void);
int KernelPad2(int val);
bool KernelPad3(void);
bool KernelPad4(void);

/**
 * Misc
 */
enum UnitStatusIdxRef {
	UNIT_STATUS_POW,
	UNIT_STATUS_MAG,
	UNIT_STATUS_SKL,
	UNIT_STATUS_SPD,
	UNIT_STATUS_LCK,
	UNIT_STATUS_DEF,
	UNIT_STATUS_RES,
	UNIT_STATUS_MOV,
	UNIT_STATUS_CON,

	UNIT_STATUS_MAX
};

enum BattleStatusIdxRef {
	BATTLE_STATUS_ATK,
	BATTLE_STATUS_DEF,
	BATTLE_STATUS_AS,
	BATTLE_STATUS_HIT,
	BATTLE_STATUS_AVO,
	BATTLE_STATUS_CRIT,
	BATTLE_STATUS_DODGE,
	BATTLE_STATUS_SILENCER,

	BATTLE_STATUS_MAX
};
