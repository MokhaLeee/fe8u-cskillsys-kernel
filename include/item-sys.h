#pragma once

#include "common-chax.h"

/**
 * IERevamp
 */
enum chax_ierevamp_use_effects {
	IER_START = 0,

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

	/**
	 * Boosters
	 */
	IER_BOOSTER_HP,
	IER_BOOSTER_POW,
	IER_BOOSTER_SKL,
	IER_BOOSTER_SPD,
	IER_BOOSTER_LCK,
	IER_BOOSTER_DEF,
	IER_BOOSTER_RES,
	IER_BOOSTER_MOV,
	IER_BOOSTER_CON,

	/**
	 * Promotion
	 */
	IER_HEROCREST,
	IER_KNIGHTCREST,
	IER_ORIONSBOLT,
	IER_ELYSIANWHIP,
	IER_GUIDINGRING,

	/**
	 * Key
	 */
	IER_CHESTKEY,
	IER_DOORKEY,
	IER_LOCKPICK,

	/**
	 * Vulnerary
	 */
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

	/**
	 * Promotion
	 */
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
	IER_CHAX_STAFF_NIGHTMARE,

	IER_MAX = 0x51,
};

#define IER_INVALID IER_START
#define IER_IS_VALID(__use_effect) (((__use_effect) > 0) && ((__use_effect) < IER_MAX))

struct IERevamp {
	bool (*usability)(struct Unit *unit, int item);
	bool (*prep_usability)(struct Unit *unit, int item);
	void (*effect)(struct Unit *unit, int item);
	void (*action_effect)(ProcPtr proc, struct Unit *unit, int item);
	void (*prep_effect)(struct ProcPrepItemUse *proc, u16 item);
};

// extern struct IERevamp const IERevampTable[IER_MAX];
// extern struct IERevamp const *const gpIERevampTable;
extern struct IERevamp const *const *const pr_gpIERevampTable;
const struct IERevamp *GetIERevamp(int item);

enum { IINFO_IER_BYTE = 22 };
#define IER_BYTE(item) \
	*(((u8 *)GetItemData(ITEM_INDEX(item))) + IINFO_IER_BYTE)

/**
 * IER extra
 */
struct IER_PromoConfig {
	u16 item;
	u16 _pad_;
	const u8 *job_list;
};

// extern struct IER_PromoConfig const IER_PromotionItemTable[];
// extern struct IER_PromoConfig const *const gpIER_PromotionItemTable;
extern struct IER_PromoConfig const *const *const pr_gpIER_PromotionItemTable;

/* DuraItem */
bool IsDuraItem(int item);
char *GetDuraItemName(int item);
int GetDuraItemDescId(int item);
int GetDuraItemUseDescId(int item);
int GetDuraItemIconId(int item);

/**
 * Modular heal
 */
typedef int (*HealAmountGetterFunc_t)(int old, struct Unit *actor, struct Unit *target);
extern HealAmountGetterFunc_t const *const gpHealAmountGetters;

int HealAmountGetter(int base, struct Unit *actor, struct Unit *target);

/**
 * IER functions
 */
#define IER_DECLEARE_Usability(__func)     bool __func(struct Unit *unit, int item)
#define IER_DECLEARE_PrepUsability(__func) bool __func(struct Unit *unit, int item)
#define IER_DECLEARE_Effect(__func)        void __func(struct Unit *unit, int item);
#define IER_DECLEARE_Action(__func)  void __func(ProcPtr proc, struct Unit *unit, int item);
#define IER_DECLEARE_PrepEffect(__func)    void __func(struct ProcPrepItemUse *proc, u16 item);

IER_DECLEARE_Usability(IER_Usability_AdjacentHeal);
IER_DECLEARE_Usability(IER_Usability_RangedHeal);
IER_DECLEARE_Usability(IER_Usability_Restore);
IER_DECLEARE_Usability(IER_Usability_Rescue);
IER_DECLEARE_Usability(IER_Usability_Barrier);
IER_DECLEARE_Usability(IER_Usability_Silence);
IER_DECLEARE_Usability(IER_Usability_Sleep);
IER_DECLEARE_Usability(IER_Usability_Berserk);
IER_DECLEARE_Usability(IER_Usability_Warp);
IER_DECLEARE_Usability(IER_Usability_Hammerne);
IER_DECLEARE_Usability(IER_Usability_Unlock);
IER_DECLEARE_Usability(IER_Usability_Booster);
IER_DECLEARE_Usability(IER_Usability_Promotion);
IER_DECLEARE_Usability(IER_Usability_Vulnerary);
IER_DECLEARE_Usability(IER_Usability_PureWater);
IER_DECLEARE_Usability(IER_Usability_Torch);
IER_DECLEARE_Usability(IER_Usability_Antitoxin);
IER_DECLEARE_Usability(IER_Usability_ChestKey);
IER_DECLEARE_Usability(IER_Usability_DoorKey);
IER_DECLEARE_Usability(IER_Usability_LockPick);
IER_DECLEARE_Usability(IER_Usability_Latona);
IER_DECLEARE_Usability(IER_Usability_Mine);
IER_DECLEARE_Usability(IER_Usability_LightRune);
IER_DECLEARE_Usability(IER_Usability_StaffTorch);
IER_DECLEARE_Usability(IER_Usability_DanceRing);
IER_DECLEARE_Usability(IER_Usability_MetisStone);
IER_DECLEARE_Usability(IER_Usability_JunaFruit);
IER_DECLEARE_Usability(IER_Usability_NightMare);
IER_DECLEARE_Effect(IER_Effect_AdjacentHeal);
IER_DECLEARE_Effect(IER_Effect_RangedHeal);
IER_DECLEARE_Effect(IER_Effect_RescueStaff);
IER_DECLEARE_Effect(IER_Effect_Restore);
IER_DECLEARE_Effect(IER_Effect_Silence);
IER_DECLEARE_Effect(IER_Effect_Sleep);
IER_DECLEARE_Effect(IER_Effect_Berserk);
IER_DECLEARE_Effect(IER_Effect_Barrier);
IER_DECLEARE_Effect(IER_Effect_Unlock);
IER_DECLEARE_Effect(IER_Effect_Warp);
IER_DECLEARE_Effect(IER_Effect_Repair);
IER_DECLEARE_Effect(IER_Effect_CommonStaff);
IER_DECLEARE_Effect(IER_Effect_Mine);
IER_DECLEARE_Effect(IER_Effect_LightRune);
IER_DECLEARE_Effect(IER_Effect_Torch);
IER_DECLEARE_Effect(IER_Effect_SpeicalDance);
IER_DECLEARE_Action(IER_Action_StandardHeal);
IER_DECLEARE_Action(IER_Action_StatusStaff);
IER_DECLEARE_Action(IER_Action_NightMare);
IER_DECLEARE_Action(IER_Action_Fortify);
IER_DECLEARE_Action(IER_Action_Restore);
IER_DECLEARE_Action(IER_Action_RescueStaff);
IER_DECLEARE_Action(IER_Action_BarrierStaff);
IER_DECLEARE_Action(IER_Action_WarpStaff);
IER_DECLEARE_Action(IER_Action_UnlockStaff);
IER_DECLEARE_Action(IER_Action_Hammerne);
IER_DECLEARE_Action(IER_Action_TorchItem);
IER_DECLEARE_Action(IER_Action_VulneraryItem);
IER_DECLEARE_Action(IER_Action_ElixirItem);
IER_DECLEARE_Action(IER_Action_PureWaterItem);
IER_DECLEARE_Action(IER_Action_AntitoxinItem);
IER_DECLEARE_Action(IER_Action_KeyItem);
IER_DECLEARE_Action(IER_Action_Promotion);
IER_DECLEARE_Action(IER_Action_Booster);
IER_DECLEARE_Action(IER_Action_Latona);
IER_DECLEARE_Action(IER_Action_Mine);
IER_DECLEARE_Action(IER_Action_LightRune);
IER_DECLEARE_Action(IER_Action_TorchStaff);
IER_DECLEARE_Action(IER_Action_DanceRing);
IER_DECLEARE_Action(IER_Action_JunaFruitItem);
IER_DECLEARE_PrepEffect(IER_PrepEffect_StatBooster);
IER_DECLEARE_PrepEffect(IER_PrepEffect_Promotion);
IER_DECLEARE_PrepEffect(IER_PrepEffect_JunaFruit);

IER_DECLEARE_Usability(IER_Usability_SkillScroll);
IER_DECLEARE_PrepUsability(IER_PrepUsability_SkillScroll);
IER_DECLEARE_Effect(IER_Effect_SkillScroll);
IER_DECLEARE_Action(IER_ActionEffect_SkillScroll);
IER_DECLEARE_PrepEffect(IER_PrepEffect_SkillScroll);
