#pragma once

#include "common-chax.h"

/**
 * IERevamp
 */
enum chax_ierevamp_use_effects {
	IER_START,

	IER_STAFF_HEAL,
	IER_STAFF_MEND,
	IER_STAFF_RECOVER,
	IER_STAFF_PHYSIC,
	IER_STAFF_FORTIFY,
	IER_STAFF_RESTORE,
	IER_STAFF_SILENCE,
	IER_STAFF_SLEEP,
	IER_STAFF_BERSERK,
	IER_STAFF_WARP,
	IER_STAFF_RESCUE,
	IER_STAFF_TORCH,
	IER_STAFF_REPAIR,
	IER_STAFF_UNLOCK,
	IER_STAFF_BARRIER,
	IER_BOOSTER_HP,
	IER_BOOSTER_POW,
	IER_BOOSTER_SKL,
	IER_BOOSTER_SPD,
	IER_BOOSTER_LCK,
	IER_BOOSTER_DEF,
	IER_BOOSTER_RES,
	IER_BOOSTER_MOV,
	IER_BOOSTER_CON,
	IER_HEROCREST,
	IER_KNIGHTCREST,
	IER_ORIONSBOLT,
	IER_ELYSIANWHIP,
	IER_GUIDINGRING,
	IER_CHESTKEY,
	IER_DOORKEY,
	IER_LOCKPICK,
	IER_VULNERARY,
	IER_ELIXIR,
	IER_PUREWATER,
	IER_ANTITOXIN,
	IER_TORCH,
	IER_CHESTKEY_BUNDLE,
	IER_MINE,
	IER_LIGHTRUNE,
	IER_FILLAS_MIGHT,
	IER_NINISS_GRACE,
	IER_THORS_IRE,
	IER_SETS_LITANY,
	IER_MASTERSEAL,
	IER_METISSTOME,
	IER_HEAVENSEAL,
	IER_OCEANSEAL,
	IER_LUNARBRACE,
	IER_SOLARBRACE,
	IER_VULNERARY_2,
	IER_UNK_GREENNOTE,
	IER_UNK_REDNOTE,
	IER_JUNAFRUIT,

	IER_CHAX_STAFF_LATONA,
	IER_CHAX_SKILL_SCROLL,

	IER_MAX = 0x51,
};

struct IERevamp {
	bool (*usability)(struct Unit *unit, int item);
	bool (*prep_usability)(struct Unit *unit, int item);
	void (*effect)(struct Unit *unit, int item);
	void (*action_effect)(ProcPtr proc, struct Unit *unit, int item);
	void (*post_action_effect)(ProcPtr proc);
};

extern struct IERevamp const IERevampTable[IER_MAX];
extern struct IERevamp const *const gpIERevampTable;
extern struct IERevamp const *const *const pr_gpIERevampTable;

typedef void (*PrepItemEffectFunc_t)(struct ProcPrepItemUse *proc, u16 item);
// extern PrepItemEffectFunc_t const gPrepItemEffectFunc[0x100];
extern PrepItemEffectFunc_t const *const gpPrepItemEffectFunc;

bool CanUnitUseMetiStome(struct Unit *unit, int item);
typedef bool (*PrepItemUsabilityFunc_t)(struct Unit *unit, int item);
// extern PrepItemUsabilityFunc_t const gPrepItemUsabilityFuncs[0x100];
extern PrepItemUsabilityFunc_t const *const gpPrepItemUsabilityFuncs;

/* DuraItem */
bool IsDuraItem(int item);
char *GetDuraItemName(int item);
int GetDuraItemDescId(int item);
int GetDuraItemUseDescId(int item);
int GetDuraItemIconId(int item);

/* Misc API functions */
void PrepItemEffect_StatBooster(struct ProcPrepItemUse *proc, u16 item);
void PrepItemEffect_Promotion(struct ProcPrepItemUse *proc, u16 item);
void PrepItemEffect_JunaFruit(struct ProcPrepItemUse *proc, u16 item);

typedef int (*HealAmountGetterFunc_t)(int old, struct Unit *actor, struct Unit *target);
extern HealAmountGetterFunc_t const *const gpHealAmountGetters;
