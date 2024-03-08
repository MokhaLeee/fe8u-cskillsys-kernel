#include "global.h"
#include "fontgrp.h"
#include "statscreen.h"

#include "common-chax.h"
#include "stat-screen.h"
#include "constants/texts.h"

static _DECL_INFO RText_Pow, RText_Mag, RText_Skl, RText_Spd, RText_Lck, RText_Def, RText_Res;
static _DECL_INFO RText_Mov, RText_Con, RText_Aid, RText_Affin, RText_Trv, RText_Cond, RText_BAmt;
static _DECL_INFO RText_Name, RText_Class, RText_Level, RText_Exp, RText_Hp;

_DECL_INFO * const RTextPageUnit = &RText_Pow;

/* Line #1 */
static _DECL_INFO RText_Pow = {
    NULL, &RText_Mag, &RText_Name, &RText_Mov,
    0x66, 0x18, 0x546,
    NULL, NULL
};

static _DECL_INFO RText_Mag = {
    &RText_Pow, &RText_Skl, &RText_Name, &RText_Con,
    0x66, 0x28, 0x547,
    NULL, NULL
};

static _DECL_INFO RText_Skl = {
    &RText_Mag, &RText_Spd, &RText_Name, &RText_Aid,
    0x66, 0x38, 0x548,
    NULL, NULL
};

static _DECL_INFO RText_Spd = {
    &RText_Skl, &RText_Lck, &RText_Name, &RText_Affin,
    0x66, 0x48, 0x549,
    NULL, NULL
};

static _DECL_INFO RText_Lck = {
    &RText_Spd, &RText_Def, &RText_Class, &RText_Trv,
    0x66, 0x58, 0x54A,
    NULL, NULL
};

static _DECL_INFO RText_Def = {
    &RText_Lck, &RText_Res, &RText_Class, &RText_Cond,
    0x66, 0x68, 0x54B,
    NULL, NULL
};

static _DECL_INFO RText_Res = {
    &RText_Def, NULL, &RText_Exp, &RText_BAmt,
    0x66, 0x78, 0x54C,
    NULL, NULL
};

/* Line #2 */
static _DECL_INFO RText_Mov = {
    NULL, &RText_Con, &RText_Pow, NULL,
    0xA6, 0x18, 0x54D,
    NULL, NULL
};

static _DECL_INFO RText_Con = {
    &RText_Mov, &RText_Aid, &RText_Mag, NULL,
    0xA6, 0x28, 0x54E,
    NULL, NULL
};

static _DECL_INFO RText_Aid = {
    &RText_Con, &RText_Affin, &RText_Skl, NULL,
    0xA6, 0x38, 0x54F,
    NULL, NULL
};

static _DECL_INFO RText_Affin = {
    &RText_Aid, &RText_Trv, &RText_Spd, NULL,
    0xA6, 0x48, 0x551,
    NULL, NULL
};

static _DECL_INFO RText_Trv = {
    &RText_Affin, &RText_Cond, &RText_Lck, NULL,
    0xA6, 0x58, 0x550,
    NULL, NULL
};

static _DECL_INFO RText_Cond = {
    &RText_Trv, &RText_BAmt, &RText_Def, NULL,
    0xA6, 0x68, 0x552,
    NULL, HbPopulate_SSStatus
};

static _DECL_INFO RText_BAmt = {
    &RText_Cond, NULL, &RText_Res, NULL,
    0xA6, 0x78, MSG_MSS_BattleAmtDesc,
    NULL, NULL
};

/* Page left */
static _DECL_INFO RText_Name = {
    &RText_Hp, &RText_Class, NULL, &RText_Spd,
    0x18, 0x50, 0,
    NULL, HbPopulate_SSCharacter
};

static _DECL_INFO RText_Class = {
    &RText_Name, &RText_Level, NULL, &RText_Def,
    0x06, 0x68, 0,
    NULL, HbPopulate_SSClass
};

static _DECL_INFO RText_Level = {
    &RText_Class, &RText_Hp, NULL, &RText_Exp,
    0x06, 0x78, 0x542,
    NULL, NULL
};

static _DECL_INFO RText_Exp = {
    &RText_Class, &RText_Hp, &RText_Level, &RText_Res,
    0x26, 0x78, 0x543,
    NULL, NULL
};

static _DECL_INFO RText_Hp = {
    &RText_Level, &RText_Name, NULL, &RText_Res,
    0x06, 0x88, 0x544,
    NULL, NULL
};
