#pragma once

#include "common-chax.h"

enum MoveLimitViewFlags {
	MOVLIMITV_MMAP_BLUE  = 1 << 0,
	MOVLIMITV_RMAP_RED   = 1 << 1,
	MOVLIMITV_RMAP_GREEN = 1 << 2,
	MOVLIMITV_RMAP_BLUE  = 1 << 4,
	MOVLIMITV_MMAP_RED   = 1 << 5,
	MOVLIMITV_MMAP_GREEN = 1 << 6,
};

int GetItemMinRangeRework(u16 item, struct Unit *unit);
int GetItemMaxRangeRework(u16 item, struct Unit *unit);
u32 GetItemReachBitsRework(u16 item, struct Unit *unit);
bool IsItemCoveringRangeRework(int item, int range, struct Unit *unit);

int GetUnitMinRange(struct Unit *unit);
int GetUnitMaxRange(struct Unit *unit);

void AddMap(int x, int y, u32 mask);
void AddMapForItem(struct Unit *unit, u16 item);
void ForEachUnit(void (*func)(struct Unit *), u8 **map, const int off);

int GetUnitWeaponReachBitsFromInventory(struct Unit *unit, int slot);
