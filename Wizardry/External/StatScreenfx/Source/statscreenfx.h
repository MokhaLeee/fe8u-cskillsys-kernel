#pragma once

#include "global.h"
#include "statscreen.h"

enum {
    STATSCREEN_BG1_WIDTH  = 19,
    STATSCREEN_BG1_HEIGHT = 19,

    STATSCREEN_LEFT_WIDTH  = 12,
    STATSCREEN_LEFT_HEIGHT = 13,
};

struct StatScreenBg1Tsa {
    u8 width, height;
    u16 data[STATSCREEN_BG1_WIDTH * STATSCREEN_BG1_HEIGHT + 1];
};

extern struct StatScreenBg1Tsa const *const TsaLut_StatScreenBgPlayer[5];
extern struct StatScreenBg1Tsa const *const TsaLut_StatScreenBgNonePlayer[5];
extern const u8 ZTSA_StatScreenPageLeft[];

#define STATSCREEN_MUAREA_CHR_BASE 0x7360
#define STATSCREEN_MUAREA_PAL_BASE 2

extern const u16 Sprite_StatScreenMuAreaBackground[];

extern u8 const *const gpImg_StatScreenMuAreaBg;
extern u16 const *const gpPal_StatScreenMuAreaBg;

extern u8 const *const gpImg_StatScreenScrollBG;
extern u16 const *const gpPal_StatScreenScrollBG;

extern u8 const *const gpImg_StatScreenUI;
extern u16 const *const gpPal_StatScreenUI;

extern u8 const *const gpImg_StatScreenUIRed;
extern u16 const *const gpPal_StatScreenUIRed;
