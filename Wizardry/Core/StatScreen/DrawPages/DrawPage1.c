#include "global.h"
#include "fontgrp.h"
#include "hardware.h"
#include "bmunit.h"
#include "statscreen.h"
#include "bmreliance.h"
#include "icon.h"
#include "ctc.h"
#include "bm.h"

#include "common-chax.h"
#include "stat-screen.h"
#include "strmag.h"
#include "lvup.h"
#include "bwl.h"
#include "debuff.h"
#include "constants/texts.h"

extern struct Font * gActiveFont;
extern int sStatScreenPage1BarMax;

STATIC_DECLAR void InstallExpandedTextPal(void)
{
    extern const u16 ExpandedTextPals[];
    ApplyPalettes(ExpandedTextPals, 0x8, 2);
};

STATIC_DECLAR void ResetActiveFontPal(void)
{
    gActiveFont->tileref = gActiveFont->tileref & 0xFFF;
}

STATIC_DECLAR void PutDrawTextRework(struct Text * text, u16 * tm, int color, int x, int tile_width, char const * str)
{
    int palid;
    switch (color) {
    case 0 ... 4:
        palid = 0;
        break;

    case 5 ... 9:
        palid = 8;
        color = color - 5;
        break;

    case 10 ... 14:
        palid = 9;
        color = color - 10;
        break;

    default:
        palid = 0;
        color = 0;
    }

    gActiveFont->tileref = TILEREF(gActiveFont->tileref & 0xFFF, palid);
    PutDrawText(text, tm, color, x, tile_width, str);
}

STATIC_DECLAR int GetTextColorFromGrowth(int growth)
{
    int _mod10 = growth / 10;
    LIMIT_AREA(_mod10, 0, 9);
    return (9 - _mod10) + 5;
}

STATIC_DECLAR int SortMax(const int * buf, int size)
{
    int i, max = 0;
    for (i = 0; i < size; i++)
    {
        if (max < buf[i])
            max = buf[i];
    }

    return max;
}

STATIC_DECLAR void DrawStatWithBarReworkExt(int num, int x, int y, int base, int total, int max, int max_ref)
{
    int diff = total - base;

    LIMIT_AREA(base, 0, max);
    LIMIT_AREA(total, 0, max);

    base  = base * STAT_BAR_MAX_INDENTIFIER / max_ref;
    total = total * STAT_BAR_MAX_INDENTIFIER / max_ref;
    max   = max * STAT_BAR_MAX_INDENTIFIER / max_ref;

    diff = total - base;

    DrawStatBarGfx(
        0x401 + num * 6, 6,
        gUiTmScratchC + TILEMAP_INDEX(x - 2, y + 1),
        TILEREF(0, STATSCREEN_BGPAL_6),
        max,
        base,
        diff);
}

STATIC_DECLAR void DrawStatWithBarRework(int num, int x, int y, int base, int total, int max)
{
    /**
     * Here the max value maybe more than 35,
     * but we need to fix the bar's length shorter than 35
     */
    int diff = total - base;
    int max_bar = sStatScreenPage1BarMax;

    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(x, y),
        base == max
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        base);

    PutNumberBonus(
        diff,
        gUiTmScratchA + TILEMAP_INDEX(x + 1, y));

    DrawStatWithBarReworkExt(num, x, y, base, total, max, max_bar);
}

STATIC_DECLAR void DrawPage1TextCommon(void)
{
    struct Unit * unit = gStatScreen.unit;

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_POWLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x1),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitPowGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4FE)); // Str

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_ITEM0],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x3),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitMagGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4FF)); // Mag

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_SKLLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x5),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitSklGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EC)); // Skl

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_SPDLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x7),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitSpdGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4ED)); // Spd

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_LCKLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x9),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitLckGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EE)); // Lck

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_DEFLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0xB),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitDefGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EF)); // Def

    PutDrawTextRework(
        &gStatScreen.text[STATSCREEN_TEXT_RESLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0xD),
        FACTION_BLUE == UNIT_FACTION(unit)
            ? GetTextColorFromGrowth(GetUnitResGrowth(unit))
            : TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F0)); // Res

    /* All growth related value done */
    ResetActiveFontPal();

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_MOVLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x1),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F6)); // Mov

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_CONLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x3),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F7)); // Con

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_AIDLABEL],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x5),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F8)); // Aid

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_SUPPORT4],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x7),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F1)); // Affin

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_RESCUENAME],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0x9),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F9)); // Trv

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_STATUS],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0xB),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4FA)); // Cond
}

STATIC_DECLAR void DrawPage1ValueReal(void)
{
    struct Unit * unit = gStatScreen.unit;

    DrawStatWithBarRework(0, 0x5, 0x1,
                    unit->pow,
                    GetUnitPower(unit),
                    UNIT_POW_MAX(unit));

    DrawStatWithBarRework(1, 0x5, 0x3,
                    UNIT_MAG(unit),
                    GetUnitMagic(unit),
                    GetUnitMaxMagic(unit));

    DrawStatWithBarRework(2, 0x5, 0x5,
                    unit->skl,
                    GetUnitSkill(unit),
                    UNIT_SKL_MAX(unit));

    DrawStatWithBarRework(3, 0x5, 0x7,
                    unit->spd,
                    GetUnitSpeed(unit),
                    UNIT_SPD_MAX(unit));

    DrawStatWithBarRework(4, 0x5, 0x9,
                    unit->lck,
                    GetUnitLuck(unit),
                    UNIT_LCK_MAX(unit));

    DrawStatWithBarRework(5, 0x5, 0xB,
                    unit->def,
                    GetUnitDefense(unit),
                    UNIT_DEF_MAX(unit));

    DrawStatWithBarRework(6, 0x5, 0xD,
                    unit->res,
                    GetUnitResistance(unit),
                    UNIT_RES_MAX(unit));
}

STATIC_DECLAR void DrawPage1ValueCommon(void)
{
    struct Unit * unit = gStatScreen.unit;

    DrawStatWithBarRework(7, 0xD, 0x1,
                    MovGetter(unit),
                    MovGetter(unit),
                    UNIT_MOV_MAX(unit));

    DrawStatWithBarRework(8, 0xD, 0x3,
                    UNIT_CON_BASE(unit),
                    UNIT_CON_BASE(unit),
                    UNIT_CON_MAX(unit));

    // displaying unit aid
    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(0xD, 0x5),
        TEXT_COLOR_SYSTEM_BLUE,
        GetUnitAid(unit));

    // displaying unit aid icon
    DrawIcon(gUiTmScratchA + TILEMAP_INDEX(0xE, 0x5),
             GetUnitAidIconId(UNIT_CATTRIBUTES(unit)),
             TILEREF(0, STATSCREEN_BGPAL_EXTICONS));

    // displaying unit rescue name
    Text_InsertDrawString(
        &gStatScreen.text[STATSCREEN_TEXT_RESCUENAME],
        24, TEXT_COLOR_SYSTEM_BLUE,
        GetUnitRescueName(unit));

    // displaying unit status name and turns
    // if (unit->statusIndex == UNIT_STATUS_NONE)
    if (GetUnitStatusIndex(unit) == UNIT_STATUS_NONE)
    {
        Text_InsertDrawString(
            &gStatScreen.text[STATSCREEN_TEXT_STATUS],
            24, TEXT_COLOR_SYSTEM_BLUE,
            GetUnitStatusName(unit));
    }
    else
    {
        Text_InsertDrawString(
            &gStatScreen.text[STATSCREEN_TEXT_STATUS],
            22, TEXT_COLOR_SYSTEM_BLUE,
            GetUnitStatusName(unit));
    }

    // display turns
    // if (gStatScreen.unit->statusIndex != UNIT_STATUS_NONE)
    if (GetUnitStatusIndex(gStatScreen.unit) != UNIT_STATUS_NONE)
    {
        PutNumberSmall(
            gUiTmScratchA + TILEMAP_INDEX(0x10, 0xB),
            0,
            // unit->statusDuration);
            GetUnitStatusDuration(unit));
    }
}

STATIC_DECLAR int GetUnitBattleAmt(struct Unit * unit)
{
    int status = 0;
    status += GetUnitPower(unit);
    status += GetUnitMagic(unit);
    status += GetUnitSkill(unit);
    status += GetUnitSpeed(unit);
    status += GetUnitLuck(unit);
    status += GetUnitDefense(unit);
    status += GetUnitResistance(unit);

    return status;
}

STATIC_DECLAR void DrawPage1BattleAmt(void)
{
    int amt = GetUnitBattleAmt(gStatScreen.unit);
    int max = 50 * 7;

    if (amt > max)
        amt = max;

    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_ITEM3],
        gUiTmScratchA + TILEMAP_INDEX(0x9, 0xD),
        TEXT_COLOR_SYSTEM_GOLD, 0, 0,
        GetStringFromIndex(MSG_MSS_BattleAmt));

    PutNumber(gUiTmScratchA + TILEMAP_INDEX(0xC + CountDigits(amt), 0xD),
        TEXT_COLOR_SYSTEM_BLUE, amt);

    DrawStatWithBarReworkExt(
        0x9, 0xD, 0xD,
        amt, amt, max, max);
}

/* BWL */
STATIC_DECLAR void DrawPage1BWL(void)
{
    struct NewBwl * bwl = GetNewBwl(UNIT_CHAR_ID(gStatScreen.unit));
    if (!bwl)
        return;

    if (UNIT_FACTION(gStatScreen.unit) != FACTION_BLUE)
        return;

    ClearText(&gStatScreen.text[STATSCREEN_TEXT_BWL]);

    // Draw B label
    Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
        0, TEXT_COLOR_SYSTEM_GOLD,
        GetStringFromIndex(MSG_MSS_BWL_BATTLE));

    // Draw W label
    Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
        32, TEXT_COLOR_SYSTEM_GOLD,
        GetStringFromIndex(MSG_MSS_BWL_WIN));

    // Draw L label
    Text_InsertDrawString(&gStatScreen.text[STATSCREEN_TEXT_BWL],
        64, TEXT_COLOR_SYSTEM_GOLD,
        GetStringFromIndex(MSG_MSS_BWL_LOSE));

    // Display labels
    PutText(&gStatScreen.text[STATSCREEN_TEXT_BWL],
        gUiTmScratchA + TILEMAP_INDEX(3, 0xF));

    // Display Battle Amt
    PutNumber(gUiTmScratchA + TILEMAP_INDEX(3 + CountDigits(bwl->battleAmt), 0xF),
        TEXT_COLOR_SYSTEM_BLUE, bwl->battleAmt);

    // Display Win Amt
    PutNumber(gUiTmScratchA + TILEMAP_INDEX(7 + CountDigits(bwl->winAmt), 0xF),
        TEXT_COLOR_SYSTEM_BLUE, bwl->winAmt);

    // Display Loss Amt
    PutNumber(gUiTmScratchA + TILEMAP_INDEX(11 + CountDigits(bwl->lossAmt), 0xF),
        TEXT_COLOR_SYSTEM_BLUE, bwl->lossAmt);
}

STATIC_DECLAR void DrawPage1Affin(void)
{
    struct Unit * unit = gStatScreen.unit;
    int affin = unit->pCharacterData->affinity;

#ifdef CONFIG_LANG_CHINESE
    const char * cn_affin[] = {
        [UNIT_AFFIN_FIRE]    = "炎",
        [UNIT_AFFIN_THUNDER] = "雷",
        [UNIT_AFFIN_WIND]    = "風",
        [UNIT_AFFIN_ICE]     = "冰",
        [UNIT_AFFIN_DARK]    = "闇",
        [UNIT_AFFIN_LIGHT]   = "光",
        [UNIT_AFFIN_ANIMA]   = "理",
    };
#endif

    if (affin)
    {
        DrawIcon(
            gUiTmScratchA + TILEMAP_INDEX(0xC, 0x7),
            GetUnitAffinityIcon(unit),
            TILEREF(0, STATSCREEN_BGPAL_EXTICONS));

#ifdef CONFIG_LANG_CHINESE
        PutDrawText(
            &gStatScreen.text[STATSCREEN_TEXT_ITEM1],
            gUiTmScratchA + TILEMAP_INDEX(0xE, 0x7),
            TEXT_COLOR_SYSTEM_GOLD,
            0, 0,
            cn_affin[affin]);
#endif
    }
    else
    {
        Text_InsertDrawString(
            &gStatScreen.text[STATSCREEN_TEXT_SUPPORT4],
            24, TEXT_COLOR_SYSTEM_BLUE,
            GetStringFromIndex(0x536));
    }
}

/* LynJump */
void DisplayPage0(void)
{
    struct Unit * unit = gStatScreen.unit;

    int i, max_vals[] = {
        UNIT_POW_MAX(unit),
        GetUnitMaxMagic(unit),
        UNIT_SKL_MAX(unit),
        UNIT_SPD_MAX(unit),
        UNIT_LCK_MAX(unit),
        UNIT_DEF_MAX(unit),
        UNIT_RES_MAX(unit),
    };

    sStatScreenPage1BarMax = SortMax(max_vals, sizeof(max_vals) / sizeof(int));

    for (i = STATSCREEN_TEXT_POWLABEL; i < STATSCREEN_TEXT_BSATKLABEL; i++)
        ClearText(&gStatScreen.text[i]);

    ClearText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT3]);
    ClearText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT4]);

    InstallExpandedTextPal();
    DrawPage1TextCommon();
    DrawPage1ValueReal();
    DrawPage1ValueCommon();
    DrawPage1BattleAmt();
    DrawPage1BWL();
    DrawPage1Affin();
}

/* LynJump */
void PageNumCtrl_DisplayBlinkIcons(struct StatScreenPageNameProc * proc)
{
    s8 displayIcon = (GetGameClock() % 32) < 20;
    const u16 palidLut[3] = { 0xC, 0xE, 0xD }; // TODO: palid constants

    if (!gStatScreen.inTransition)
    {
        if ((gStatScreen.page == STATSCREEN_PAGE_0) && (gStatScreen.unit->state & US_RESCUING))
        {
            UpdateStatArrowSprites(120, 56, 1);
            UpdateStatArrowSprites(120, 72, 1);

            if (displayIcon)
            {
                PutSprite(4,
                    184, 94, gObject_8x8,
                    TILEREF(3, 0xF & palidLut[gStatScreen.unit->rescue >> 6]) + OAM2_LAYER(2));
            }
        }

        if (gStatScreen.unit->state & US_RESCUED)
        {
            if (displayIcon)
            {
                PutSprite(4,
                    10, 86, gObject_8x8,
                    TILEREF(3, 0xF & palidLut[gStatScreen.unit->rescue>>6]) + OAM2_LAYER(2));
            }
        }
    }
}
