#include "common-chax.h"
#include "hooks.h"
#include "stat-screen.h"
#include "statscreen.h"
#include "skill-system.h"
#include "constants/texts.h"
#include "item-sys.h"

LYN_REPLACE_CHECK(InitText);
void InitText(struct Text *text, int tileWidth)
{
    text->chr_position = gActiveFont->chr_counter;
    text->tile_width = tileWidth;
    text->db_id = 0;
    text->db_enabled = false;
    text->is_printing = false;
    gActiveFont->chr_counter += tileWidth;
    ClearText(text);
}

/* This is page 2 in C Skill System*/

void DisplayPage1(void);

LYN_REPLACE_CHECK(DisplayPage1);
void DisplayPage1(void)
{
    int i, item;
    const char* str;

    Decompress(
        gUnknown_08A02204,
        gGenericBuffer);

    CallARM_FillTileRect(
        gUiTmScratchC + TILEMAP_INDEX(1, 11),
        gGenericBuffer, TILEREF(0x40, STATSCREEN_BGPAL_3));

    DisplayTexts(sPage1TextInfo);

    if (!UNIT_IS_GORGON_EGG(gStatScreen.unit))
    {
        for (i = 0; (i < UNIT_ITEM_COUNT) && (item = gStatScreen.unit->items[i]); ++i)
        {
            int color;

            if ((gStatScreen.unit->state & US_DROP_ITEM) && (i == GetUnitItemCount(gStatScreen.unit) - 1))
                color = TEXT_COLOR_SYSTEM_GREEN;
            else
                color = IsItemDisplayUsable(gStatScreen.unit, item)
                    ? TEXT_COLOR_SYSTEM_WHITE
                    : TEXT_COLOR_SYSTEM_GRAY;

            /* Decides the horizontal position of the text with the first parameter of TILEMAP_INDEX */
            /* Decides the vertical position of the item text with the second parameter of TILEMAP_INDEX */
            /* The parameter inside gStatScreen,ext is connected to sSSMasterTextInitInfo which controls that string's width
            ** but it seems to be linked to other things that mess up the graphics for that tile
            */
            if (IsDuraItem(gStatScreen.unit->items[i]))
                InitText(&gStatScreen.text[STATSCREEN_TEXT_ITEM0 + i], 11);

            DrawItemStatScreenLine(
            &gStatScreen.text[STATSCREEN_TEXT_ITEM0 + i],
            item, color,
            gUiTmScratchA + TILEMAP_INDEX(1, 1 + i*2));
        }
    }

    i = GetUnitEquippedWeaponSlot(gStatScreen.unit);
    item = 0;

    if (gStatScreen.unit->pClassData->number != CLASS_GORGONEGG)
    {
        if ((gStatScreen.unit->pClassData->number != CLASS_GORGONEGG2) && (i >= 0))
        {
            PutSpecialChar(
                gUiTmScratchA + TILEMAP_INDEX(16, 1 + i*2),
                TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_35);

            CallARM_FillTileRect(
                gUiTmScratchC + TILEMAP_INDEX(1, 2 + i*2),
                gUnknown_08A02250, TILEREF(0x40, STATSCREEN_BGPAL_3));

            item = gStatScreen.unit->items[i];
        }
    }

    /* Battle stats at the bottom of the page */
    if (!UNIT_IS_GORGON_EGG(gStatScreen.unit))
    {
        PutNumberOrBlank(
            gUiTmScratchA + TILEMAP_INDEX(8,  13),
            TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleAttack);

        PutNumberOrBlank(
            gUiTmScratchA + TILEMAP_INDEX(8,  15),
            TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleHitRate);

        PutNumberOrBlank(
            gUiTmScratchA + TILEMAP_INDEX(15, 13),
            TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleCritRate);

        PutNumberOrBlank(
            gUiTmScratchA + TILEMAP_INDEX(15, 15),
            TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleAvoidRate);
    }
    else
    {
        PutNumberOrBlank(
            gUiTmScratchA + TILEMAP_INDEX(8,  13),
            TEXT_COLOR_SYSTEM_BLUE, 0xFF);

        PutNumberOrBlank(
            gUiTmScratchA + TILEMAP_INDEX(8,  15),
            TEXT_COLOR_SYSTEM_BLUE, 0xFF);

        PutNumberOrBlank(
            gUiTmScratchA + TILEMAP_INDEX(15, 13),
            TEXT_COLOR_SYSTEM_BLUE, 0xFF);

        PutNumberOrBlank(
            gUiTmScratchA + TILEMAP_INDEX(15, 15),
            TEXT_COLOR_SYSTEM_BLUE, gBattleActor.battleAvoidRate);

        item = 0;
    }

    // TODO: macro, maybe?

    str = GetItemDisplayRangeString(item);
    Text_InsertDrawString(
        &gStatScreen.text[STATSCREEN_TEXT_BSRANGE],
        55 - GetStringTextLen(str),
        TEXT_COLOR_SYSTEM_BLUE, str);

    for (i = 0; i < 8; ++i)
    {
        gUiTmScratchA[TILEMAP_INDEX(1 + i, 11)] = TILEREF(0x60 + i, STATSCREEN_BGPAL_7);
        gUiTmScratchA[TILEMAP_INDEX(1 + i, 12)] = TILEREF(0x68 + i, STATSCREEN_BGPAL_7);
    }
}
