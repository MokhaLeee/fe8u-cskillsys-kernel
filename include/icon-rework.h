#pragma once

#include "global.h"
#include "icon.h"

#include "common-chax.h"

#define ICON_SHEET(icon) (((icon) >> 8) & 0xFF)
#define ICON_INDEX(icon) ((icon) & 0xFF)

#define MAX_SIMULTANEOUS_ICONS 32

extern u16 gIconReSts[MAX_SIMULTANEOUS_ICONS];

typedef const u8 * (* IconGetterFunc)(const u8 index);
extern const IconGetterFunc IconGetters[ICON_SHEET_AMT];

const u8 * GetIconGfx(u32 icon);
