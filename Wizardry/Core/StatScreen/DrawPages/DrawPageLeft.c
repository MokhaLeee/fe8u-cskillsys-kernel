#include "common-chax.h"
#include "stat-screen.h"
#include "lvup.h"

static void DisplayHpStr(void)
{
    FORCE_DECLARE int bank, color;
    struct Unit * unit = gStatScreen.unit;

    color = FACTION_BLUE == UNIT_FACTION(unit)
          ? GetTextColorFromGrowth(GetUnitHpGrowth(unit))
          : TEXT_COLOR_SYSTEM_GOLD;

    PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(5, 17), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_SLASH);

#ifdef CONFIG_GROWTHS_AS_LETTERS
    PutTwoSpecialChar(
        gBG0TilemapBuffer + TILEMAP_INDEX(1, 17),
        TEXT_COLOR_SYSTEM_GOLD,
        0x22, 0x23);
#else 
    ModifyTextPal(bank, color);
    gActiveFont->tileref = TILEREF(gActiveFont->tileref & 0xFFF, bank);

    PutTwoSpecialChar(
        gBG0TilemapBuffer + TILEMAP_INDEX(1, 17),
        color,
        0x22, 0x23);
#endif

    ResetActiveFontPal();
}

STATIC_DECLAR void DisplayHpGrowthValue(void)
{
    int growth = GetUnitHpGrowth(gStatScreen.unit);

#ifdef CONFIG_GROWTHS_AS_LETTERS
    int character1 = 0;
    FORCE_DECLARE int character2 = 0;

    if (growth >= 100)
    {
        character1 = TEXT_SPECIAL_S;
    }
    else if (growth >= 90)
    {
        character1 = TEXT_SPECIAL_A;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth >= 80)
    {
        character1 = TEXT_SPECIAL_A;
    }
    else if (growth >= 70)
    {
        character1 = TEXT_SPECIAL_B;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth >= 60)
    {
        character1 = TEXT_SPECIAL_B;
    }
    else if (growth >= 50)
    {
        character1 = TEXT_SPECIAL_C;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth >= 40)
    {
        character1 = TEXT_SPECIAL_C;
    }
    else if (growth >= 30)
    {
        character1 = TEXT_SPECIAL_D;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth >= 20)
    {
        character1 = TEXT_SPECIAL_D;
    }
    else if (growth >= 10)
    {
        character1 = TEXT_SPECIAL_E;
        character2 = TEXT_SPECIAL_PLUS;
    }
    else if (growth > 0)
    {
        character1 = TEXT_SPECIAL_E;
    }
    else 
    {
        character1 = TEXT_SPECIAL_BIGNUM_0;
    }

    
    PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(5, 17),
    TEXT_COLOR_SYSTEM_BLUE,
    character1);

    if (character2 > 0)
    {
        PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(6, 17),
        TEXT_COLOR_SYSTEM_BLUE,
        character2);
    }
#else
    int bank, color = GetTextColorFromGrowth(growth);
    ModifyTextPal(bank, color);
    gActiveFont->tileref = TILEREF(gActiveFont->tileref & 0xFFF, bank);

    PutNumberOrBlank(
        gBG0TilemapBuffer + TILEMAP_INDEX(7, 17),
        TEXT_COLOR_SYSTEM_GOLD,
        growth);
#endif

    ResetActiveFontPal();
}

STATIC_DECLAR void DisplayHpBmValue(void)
{
    struct Unit * unit = gStatScreen.unit;

    int hpcur = GetUnitCurrentHp(unit);
    int hpmax = GetUnitMaxHp(unit);

    int color = hpcur == hpmax
              ? TEXT_COLOR_SYSTEM_GREEN
              : TEXT_COLOR_SYSTEM_BLUE;

    // Display '/' labels
#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
    if (hpmax > 99)
        PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(6, 17), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_SLASH);
    else
        PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(5, 17), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_SLASH);
#else
    PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(5, 17), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_SLASH);
#endif

    /* Display current hp */
#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
    if (hpmax > 99)
        PutNumberOrBlank(
            gBG0TilemapBuffer + TILEMAP_INDEX(5, 17),
            color, hpcur);
    else
        PutNumberOrBlank(
            gBG0TilemapBuffer + TILEMAP_INDEX(4, 17),
            color, hpcur);
#else
    if (hpcur > 99)
        PutTwoSpecialChar(
            gBG0TilemapBuffer + TILEMAP_INDEX(3, 17),
            color, 0x14, 0x14);
    else
        PutNumberOrBlank(
            gBG0TilemapBuffer + TILEMAP_INDEX(4, 17),
            color, hpcur);
#endif

    /* Display max hp */
#ifdef CONFIG_UNLOCK_ALLY_MHP_LIMIT
    if (hpmax > 99)
        PutNumberOrBlank(
            gBG0TilemapBuffer + TILEMAP_INDEX(9, 17),
            color, hpmax);
    else
        PutNumberOrBlank(
            gBG0TilemapBuffer + TILEMAP_INDEX(7, 17),
            color, hpmax);
#else
    if (hpmax > 99)
        PutTwoSpecialChar(
            gBG0TilemapBuffer + TILEMAP_INDEX(6, 17),
            color, 0x14, 0x14);
    else
        PutNumberOrBlank(
            gBG0TilemapBuffer + TILEMAP_INDEX(7, 17),
            color, hpmax);
#endif
}

void ToggleUnitLeftPage(bool toggle)
{
    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 3, 17), 5, 2, 0);

    if (toggle == false)
        DisplayHpBmValue();
    else
        DisplayHpGrowthValue();
}

STATIC_DECLAR void DisplayLeftPanelHp(void)
{
    DisplayHpStr();
    DisplayHpBmValue();
}

LYN_REPLACE_CHECK(DisplayLeftPanel);
void DisplayLeftPanel(void)
{
    const char * namestr = GetStringFromIndex(UNIT_NAME_ID(gStatScreen.unit));
    unsigned namexoff = GetStringTextCenteredPos(0x30, namestr);

    InstallExpandedTextPal();
    BG_Fill(gBG0TilemapBuffer, 0);

    // Generate battle stats for unit for display later
    BattleGenerateUiStats(
        gStatScreen.unit,
        GetUnitEquippedWeaponSlot(gStatScreen.unit));

    // Display character name
#ifdef CONFIG_AUTO_NARROW_FONT
    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_CHARANAME],
        gBG0TilemapBuffer + TILEMAP_INDEX(3, 10),
        TEXT_COLOR_SYSTEM_WHITE, namexoff, 0, Utf8ToNarrowFonts((char *)namestr));
#else 
    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_CHARANAME],
        gBG0TilemapBuffer + TILEMAP_INDEX(3, 10),
        TEXT_COLOR_SYSTEM_WHITE, namexoff, 0, namestr);
#endif

    // Display class name
    PutDrawText(
        &gStatScreen.text[STATSCREEN_TEXT_CLASSNAME],
        gBG0TilemapBuffer + TILEMAP_INDEX(1, 13),
        TEXT_COLOR_SYSTEM_WHITE, 0, 0,
        GetStringFromIndex(gStatScreen.unit->pClassData->nameTextId));

    // Display Lv/E labels
    PutTwoSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(1, 15), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_LV_A, TEXT_SPECIAL_LV_B);
    PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(5, 15), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_E);

    // Display level
    PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(4, 15), TEXT_COLOR_SYSTEM_BLUE,
        gStatScreen.unit->level);

    // Display exp
    PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(7, 15), TEXT_COLOR_SYSTEM_BLUE,
        gStatScreen.unit->exp);

#if CHAX
    DisplayLeftPanelHp();
#else
    // Display Hp label
    PutTwoSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(1, 17), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_HP_A, TEXT_SPECIAL_HP_B);

    // Display current hp
    if (GetUnitCurrentHp(gStatScreen.unit) > 99)
    {
        // Display '--' if current hp > 99
        PutTwoSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(3, 17), TEXT_COLOR_SYSTEM_BLUE,
            TEXT_SPECIAL_DASH, TEXT_SPECIAL_DASH);
    }
    else
    {
        // Display current hp
        PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(4, 17), TEXT_COLOR_SYSTEM_BLUE,
            GetUnitCurrentHp(gStatScreen.unit));
    }

    // Display max hp
    if (GetUnitMaxHp(gStatScreen.unit) > 99)
    {
        // Display '--' if max hp > 99
        PutTwoSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(6, 17), TEXT_COLOR_SYSTEM_BLUE,
            TEXT_SPECIAL_DASH, TEXT_SPECIAL_DASH);
    }
    else
    {
        // Display max hp
        PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(7, 17), TEXT_COLOR_SYSTEM_BLUE,
            GetUnitMaxHp(gStatScreen.unit));
    }
#endif
}
