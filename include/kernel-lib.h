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

	u8 gaiden_magic_en, gaiden_magic_must_be_magic, gaiden_magic_requires_wrank, \
		gaiden_magic_ai_en, gaiden_magic_ext_conf_en;

	u8 no_suspend_in_aiphase;

	u8 shield_en, shield_ext_equip_config_en;

	u8 auto_narrow_font;

	u8 skill_sub_menu_width;

	// auto generate rtext for weapon rank bonus in statscreen
	u8 wrank_bonux_rtext_auto_gen;
};

struct KernelBattleDesignerConfig {
	// ATK percentage on CON calc in attack-speed calculation
	u8 as_calc_atk_perc;

	// HP drain item percentage
	u8 nosferatu_hpdrain_perc;

	// AVO deacy indoor for riders
	u8 rider_debuff_indoor;

	// CRIT damage correction rate
	u8 crit_correction;
};

extern const struct KernelDesigerConfig *const gpKernelDesigerConfig;
extern const struct KernelBattleDesignerConfig *const gpKernelBattleDesignerConfig;

/**
 * map.c
 */
bool IsPositionValid(s8 x, s8 y);
struct Unit *GetUnitAtPosition(s8 x, s8 y);
bool CheckOutdoorTerrain(int terrain);

/**
 * mapanim.c
 */
void MapAnim_CommonInit(void);
void MapAnim_CommonEnd(void);

/**
 * math.c
 */
u32 k_udiv(u32 a, u32 b);
u32 k_umod(u32 a, u32 b);

static inline u32 perc_of(u32 num, u32 perc)
{
	return k_udiv(num * perc, 100);
}

#define DIV_ROUND_CLOSEST(x, divisor)( \
{ \
	typeof(x) __x = x; \
	typeof(divisor) __d = divisor; \
	(((typeof(x))-1) > 0 || \
	((typeof(divisor))-1) > 0 || (__x) > 0) ? \
		(k_udiv(((__x) + ((__d) / 2)), (__d))) : \
		(k_udiv(((__x) - ((__d) / 2)), (__d))); \
} \
)

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
bool CheckKernelHookSkippingFlag(void);
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
 * unit-validation.c
 */
bool UnitAvaliable(struct Unit *unit);
bool UnitOnMapAvaliable(struct Unit *unit);
bool IsUnitStruct(struct Unit *maybe_unit);

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

// Maybe there could be an external "FOR_UNITS" macro
#undef FOR_UNITS

#define FOR_UNITS_VALID(begin, end, var_name, body) \
{ \
	int ___uid; \
	struct Unit *var_name; \
	for (___uid = (begin); ___uid < (end); ++___uid) { \
		var_name = GetUnit(___uid); \
		if (!var_name) \
			continue; \
		if (!UNIT_IS_VALID(var_name)) \
			continue; \
		body \
	} \
}

#define FOR_UNITS_ONMAP(begin, end, var_name, body) \
{ \
	int ___uid; \
	struct Unit *var_name; \
	for (___uid = (begin); ___uid < (end); ++___uid) { \
		var_name = GetUnit(___uid); \
		if (!var_name) \
			continue; \
		if (!UnitOnMapAvaliable(var_name)) \
			continue; \
		body \
	} \
}

#define FOR_UNITS_ONMAP_FACTION(faction, var_name, body) \
	FOR_UNITS_ONMAP(faction + 1, faction + 0x40, var_name, body)

#define FOR_UNITS_VALID_FACTION(faction, var_name, body) \
	FOR_UNITS_VALID(faction + 1, faction + 0x40, var_name, body)

#define FOR_UNITS_ONMAP_ALL(var_name, body) \
	FOR_UNITS_ONMAP(1, 0xC0, var_name, body)

#define FOR_UNITS_VALID_ALL(var_name, body) \
	FOR_UNITS_VALID(1, 0xC0, var_name, body)

int GetUidFaction(u8 uid);
int GetUnitFaction(struct Unit *unit);

/**
 * Bits
 */
static inline void _BIT_SET(void *bits, int idx)
{
	u8 *_bits = bits;

	_bits[idx >> 3] |= 1 << (idx & 0x7);
}

static inline void _BIT_CLR(void *bits, int idx)
{
	u8 *_bits = bits;

	_bits[idx >> 3] &= ~(1 << (idx & 0x7));
}

static inline bool _BIT_CHK(void *bits, int idx)
{
	u8 *_bits = bits;

	return !!(_bits[idx >> 3] & (1 << (idx & 0x7)));
}

/**
 * pad.c
 */
void KernelPad1(void);
int KernelPad2(int val);
bool KernelPad3(void);
bool KernelPad4(void);

/**
 * wexp.c
 */
int WRankToWExp(int wrank);

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

/**
 * unit.c
 */
#define UNIT_WRANK(unit, wtype) ((unit)->ranks[wtype])
