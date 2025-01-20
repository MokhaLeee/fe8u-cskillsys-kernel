#pragma once

#include "common-chax.h"

enum icon_sheet_idx {
	ICON_SHEET_VANILLA,
	ICON_SHEET_AFFIN,
	ICON_SHEET_MOUNT,
	ICON_SHEET_WTYPE,
	ICON_SHEET_COMBATART,
	ICON_SHEET_SKILL0,
	ICON_SHEET_SKILL1,
	ICON_SHEET_SKILL2,
	ICON_SHEET_SKILL3,

	ICON_SHEET_AMT = 16
};

#define AFFIN_ICON(affin) ((ICON_SHEET_AFFIN << 8) + (affin))
#define MOUNT_ICON(mount) ((ICON_SHEET_MOUNT << 8) + (mount))
#define WTYPE_ICON(wtype) ((ICON_SHEET_WTYPE << 8) + (wtype))
#define COMBART_ICON(cid) ((ICON_SHEET_COMBATART << 8) + (cid))
#define SKILL_ICON(sid)   ((ICON_SHEET_SKILL0 << 8) + (sid))

#define ICON_SHEET(icon) (((icon) >> 8) & 0xFF)
#define ICON_INDEX(icon) ((icon) & 0xFF)

#define MAX_SIMULTANEOUS_ICONS 32

extern u16 gIconReSts[MAX_SIMULTANEOUS_ICONS];

typedef const u8 *(* IconGetterFunc)(const u8 index);
// extern const IconGetterFunc IconGetters[ICON_SHEET_AMT];
extern const IconGetterFunc *const gpIconGetters;

const u8 *GetIconGfx(u32 icon);
const u8 *IconGetterVanilla(const u8 index);
const u8 *GetAffinIcon(const u8 index);
const u8 *GetMountIcon(const u8 index);
const u8 *GetWTypeIcon(const u8 index);

extern u8 const *const pr_item_icon_tiles;
extern u8 const *const *const gpAffinIcons;
extern u8 const *const *const gpMountIcons;
extern u8 const *const *const gpWTypeIcons;
