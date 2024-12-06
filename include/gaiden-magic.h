#pragma once

#include <common-chax.h>
#include <kernel-lib.h>
#include <list-verify.h>
#include <battle-system.h>

#ifdef CONFIG_USE_GAIDEN_MAGIC

struct GaidenPinfoConfigEnt {
	u8 level, iid;
};
extern struct GaidenPinfoConfigEnt const *const gGaidenPinfoConfigList[0x100];
extern const u8 gGaidenWeaponCostList[0x100];

struct GaidenChaxConfigEnt {
	u8 iid;
	u8 pid, jid, level;

	u16 skill, evflag;
};
extern const struct GaidenChaxConfigEnt gGaidenChaxConfigs[];
extern struct GaidenChaxConfigEnt const *const gpGaidenChaxConfigs;

struct GaidenMagicList {
	struct UnitListHeader header;

	u8 bmag_cnt;
	u8 bmags[7];

	u8 wmag_cnt;
	u8 wmags[7];
};

extern struct GaidenMagicList sGaidenMagicListObj;
extern struct GaidenMagicList *const gGaidenMagicList;

void ResetGaidenMagicList(struct GaidenMagicList *list);
void UpdateGaidenMagicList(struct Unit *unit, struct GaidenMagicList *list);
struct GaidenMagicList *GetGaidenMagicList(struct Unit *unit);

bool CanUnitUseGaidenMagic(struct Unit *unit, int item);
bool CanUnitUseGaidenMagicNow(struct Unit *unit, int item);
int GetGaidenMagicAutoEquipSlot(struct Unit *unit);
int GetGaidenMagicItem(struct Unit *unit, int slot);

void BattleGenerateGaidenBMagSimulation(struct Unit *actor, struct Unit *target, int x, int y);
void BattleGenerateGaidenBMagReal(struct Unit *actor, struct Unit *target);

u8 GaidenBMagActionCommandUsability(const struct MenuItemDef *def, int number);
int GaidenBMagActionCommandOnDarw(struct MenuProc *menu, struct MenuItemProc *item);
u8 GaidenBMagActionCommandEffect(struct MenuProc *menu, struct MenuItemProc *menuItem);
int GaidenBMagActionCommandHover(struct MenuProc *menu, struct MenuItemProc *menuItem);
int GaidenBMagActionCommandUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem);

#else

static inline int GetGaidenMagicAutoEquipSlot(struct Unit *unit) { return -1; }
static inline int GetGaidenMagicItem(struct Unit *unit, int slot) { return ITEM_NONE; }

static inline u8 GaidenBMagActionCommandUsability(const struct MenuItemDef *def, int number) { return MENU_NOTSHOWN; }
static inline int GaidenBMagActionCommandOnDarw(struct MenuProc *menu, struct MenuItemProc *item) { return 0; }
static inline u8 GaidenBMagActionCommandEffect(struct MenuProc *menu, struct MenuItemProc *menuItem) { return 0; }
static inline int GaidenBMagActionCommandHover(struct MenuProc *menu, struct MenuItemProc *menuItem) { return 0; }
static inline int GaidenBMagActionCommandUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem) { return 0; }

#endif /* USE_GAIDEN_MAGIC */
