#include "global.h"
#include "hardware.h"
#include "fontgrp.h"
#include "bmlib.h"
#include "ctc.h"
#include "icon.h"
#include "bmreliance.h"
#include "statscreen.h"

#include "common-chax.h"
#include "hooks.h"
#include "stat-screen.h"
#include "constants/texts.h"

#define PAGE4_PINFO_MAX 8

STATIC_DECLAR void DrawPage4SupportBonus(void)
{
    struct SupportBonuses bonuses;
    struct Unit * unit = gStatScreen.unit;

    /* Draw texts */
    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_SUPPORT3],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x1),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(MSG_MSS_SupportBonus));

    PutDrawText(
        &gStatScreen.text[3],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x3),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F3)); // Atk

    PutDrawText(
        &gStatScreen.text[4],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x5),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EF)); // Def

    PutDrawText(
        &gStatScreen.text[5],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x7),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F4)); // Hit

    PutDrawText(
        &gStatScreen.text[6],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0x9),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F5)); // Avo

    PutDrawText(
        &gStatScreen.text[7],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0xB),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x501)); // Crit

    PutDrawText(
        &gStatScreen.text[8],
        gUiTmScratchA + TILEMAP_INDEX(0x1, 0xD),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x51E)); // Dodge
    
    /* Generate bonus value */
    GetUnitSupportBonuses(unit, &bonuses);

    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(0x6, 0x3),
        TEXT_COLOR_SYSTEM_BLUE,
        bonuses.bonusAttack);

    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(0x6, 0x5),
        TEXT_COLOR_SYSTEM_BLUE,
        bonuses.bonusDefense);

    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(0x6, 0x7),
        TEXT_COLOR_SYSTEM_BLUE,
        bonuses.bonusHit);

    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(0x6, 0x9),
        TEXT_COLOR_SYSTEM_BLUE,
        bonuses.bonusAvoid);

    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(0x6, 0xB),
        TEXT_COLOR_SYSTEM_BLUE,
        bonuses.bonusCrit);

    PutNumberOrBlank(
        gUiTmScratchA + TILEMAP_INDEX(0x6, 0xD),
        TEXT_COLOR_SYSTEM_BLUE,
        bonuses.bonusDodge);
}

STATIC_DECLAR void DrawPage4SupportPInfo(void)
{
    struct Unit * unit = gStatScreen.unit;

    int i, line, max = GetUnitSupporterCount(unit);
    const u8 text_id[8] = {
        STATSCREEN_TEXT_SUPPORT0,
        STATSCREEN_TEXT_SUPPORT1,
        STATSCREEN_TEXT_SUPPORT2,
        STATSCREEN_TEXT_ITEM0,
        STATSCREEN_TEXT_ITEM1,
        STATSCREEN_TEXT_ITEM2,
        STATSCREEN_TEXT_ITEM3,
        STATSCREEN_TEXT_ITEM4,
    };

    for (i = 0, line = 0; i < max; i++)
    {
        int level = GetUnitSupportLevel(unit, i);
        u8 pid = GetUnitSupporterCharacter(unit, i);

        if (level != 0)
        {
            struct Unit unit_tmp = {
                .pCharacterData = &gpCharacterData[pid],
            };

            DrawIcon(
                gUiTmScratchA + TILEMAP_INDEX(0x7, 0x1 + 2 * line),
                GetUnitAffinityIcon(&unit_tmp),
                TILEREF(0, STATSCREEN_BGPAL_EXTICONS));

            PutDrawText(
                &gStatScreen.text[text_id[line]],
                gUiTmScratchA + TILEMAP_INDEX(0x9, 0x1 + 2 * line),
                TEXT_COLOR_SYSTEM_WHITE,
                0, 0,
                GetStringFromIndex(GetCharacterData(pid)->nameTextId));

            PutSpecialChar(
                gUiTmScratchA + TILEMAP_INDEX(0x0F, 0x1 + 2 * line),
                level == 3
                    ? TEXT_COLOR_SYSTEM_GREEN
                    : TEXT_COLOR_SYSTEM_BLUE,
                GetSupportLevelUiChar(level));

            line = line + 1;
        }

        if (line >= (PAGE4_PINFO_MAX - 1))
            break;
    }
}

void DrawPage4Rework(void)
{
    int i;
    for (i = STATSCREEN_TEXT_POWLABEL; i < STATSCREEN_TEXT_BSRANGE; i++)
        ClearText(&gStatScreen.text[i]);

    ClearText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT0]);
    ClearText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT1]);
    ClearText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT2]);
    ClearText(&gStatScreen.text[STATSCREEN_TEXT_SUPPORT3]);

    for (i = STATSCREEN_TEXT_BSRANGE; i < (STATSCREEN_TEXT_BSRANGE + 8); i++)
        ClearText(&gStatScreen.text[i]);

    DrawPage4SupportBonus();
    DrawPage4SupportPInfo();
}
