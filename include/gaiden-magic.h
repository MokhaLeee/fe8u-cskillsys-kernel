#pragma once

#include <common-chax.h>
#include <kernel-lib.h>
#include <list-verify.h>
#include <battle-system.h>

struct GaidenPinfoConfigEnt { u8 level, iid; };
struct GaidenPinfoConfig { const struct GaidenPinfoConfigEnt *ent; };

// extern struct GaidenPinfoConfig const gGaidenPinfoConfigList[0x100];
extern struct GaidenPinfoConfig const *const gpGaidenPinfoConfigList;

// extern const u8 gGaidenWeaponCostList[0x100];
extern u8 const *const gpGaidenWeaponCostList;

struct GaidenChaxConfigEnt {
	u8 iid;
	u8 pid, jid, level;
	u8 faction;

	u16 skill, evflag;
};

// extern const struct GaidenChaxConfigEnt gGaidenChaxConfigs[];
extern struct GaidenChaxConfigEnt const *const gpGaidenChaxConfigs;

#define GAIDEN_MAGIC_LIST_LEN 7

struct GaidenMagicList {
	struct UnitListHeader header;

	u8 bmag_cnt;
	u8 bmags[GAIDEN_MAGIC_LIST_LEN];

	u8 wmag_cnt;
	u8 wmags[GAIDEN_MAGIC_LIST_LEN];
};

extern struct GaidenMagicList sGaidenMagicListObj;
extern struct GaidenMagicList *const gGaidenMagicList;

void ResetGaidenMagicList(struct GaidenMagicList *list);
void UpdateGaidenMagicList(struct Unit *unit, struct GaidenMagicList *list);
struct GaidenMagicList *GetGaidenMagicList(struct Unit *unit);

bool CanUnitUseGaidenMagic(struct Unit *unit, int item);
bool CanUnitUseGaidenMagicNow(struct Unit *unit, int item);

void BattleGenerateGaidenBMagSimulation(struct Unit *actor, struct Unit *target, int x, int y);
void BattleGenerateGaidenBMagReal(struct Unit *actor, struct Unit *target);

static inline bool CheckGaidenMagicAttack(struct BattleUnit *bu)
{
	switch (bu->weaponSlotIndex) {
	case CHAX_BUISLOT_GAIDEN_BMAG1:
	case CHAX_BUISLOT_GAIDEN_BMAG2:
	case CHAX_BUISLOT_GAIDEN_BMAG3:
	case CHAX_BUISLOT_GAIDEN_BMAG4:
	case CHAX_BUISLOT_GAIDEN_BMAG5:
	case CHAX_BUISLOT_GAIDEN_BMAG6:
	case CHAX_BUISLOT_GAIDEN_BMAG7:
	case CHAX_BUISLOT_GAIDEN_WMAG1:
	case CHAX_BUISLOT_GAIDEN_WMAG2:
	case CHAX_BUISLOT_GAIDEN_WMAG3:
	case CHAX_BUISLOT_GAIDEN_WMAG4:
	case CHAX_BUISLOT_GAIDEN_WMAG5:
	case CHAX_BUISLOT_GAIDEN_WMAG6:
	case CHAX_BUISLOT_GAIDEN_WMAG7:
		return true;

	default:
		return false;
	}
}

int GetGaidenWeaponHpCost(struct Unit *unit, int item);
void BattleGenerateHitHpCostForGaidenMagic(struct BattleUnit *attacker, struct BattleUnit *defender);
int GetGaidenMagicAutoEquipSlot(struct Unit *unit);
int GetGaidenMagicAutoEquipStaff(struct Unit *unit);
int GetGaidenMagicItem(struct Unit *unit, int slot);
void TryChangeGaidenMagicAction(void);

void PutGaidenMagicCostNumber(u16 *tm, int color, int number);
void DrawGaidenMagItemMenuLine(struct Text *text, int item, s8 isUsable, u16 *mapOut);

u8 GaidenBMagActionCommandUsability(const struct MenuItemDef *def, int number);
int GaidenBMagActionCommandOnDarw(struct MenuProc *menu, struct MenuItemProc *item);
u8 GaidenBMagActionCommandEffect(struct MenuProc *menu, struct MenuItemProc *menuItem);
int GaidenBMagActionCommandHover(struct MenuProc *menu, struct MenuItemProc *menuItem);
int GaidenBMagActionCommandUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem);

u8 GaidenWMagActionCommandUsability(const struct MenuItemDef *def, int number);
int GaidenWMagActionCommandOnDarw(struct MenuProc *menu, struct MenuItemProc *item);
u8 GaidenWMagActionCommandEffect(struct MenuProc *menu, struct MenuItemProc *menuItem);
int GaidenWMagActionCommandHover(struct MenuProc *menu, struct MenuItemProc *menuItem);
