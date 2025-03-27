#include "common-chax.h"
#include "stat-screen.h"
#include "lvup.h"

STATIC_DECLAR void DisplayHpStr(void)
{
	int bank, color;
	struct Unit *unit = gStatScreen.unit;

	color = UNIT_FACTION(unit) == FACTION_BLUE
		  ? GetTextColorFromGrowth(GetUnitHpGrowth(unit))
		  : TEXT_COLOR_SYSTEM_GOLD;

	PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(5, 17), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_SLASH);

	ModifyTextPal(bank, color);
	gActiveFont->tileref = TILEREF(gActiveFont->tileref & 0xFFF, bank);

	PutTwoSpecialChar(
		gBG0TilemapBuffer + TILEMAP_INDEX(1, 17),
		color,
		0x22, 0x23);

	ResetActiveFontPal();
}

STATIC_DECLAR void DisplayHpGrowthValue(void)
{
	int bank, color = GetTextColorFromGrowth(GetUnitHpGrowth(gStatScreen.unit));

	ModifyTextPal(bank, color);
	gActiveFont->tileref = TILEREF(gActiveFont->tileref & 0xFFF, bank);

	PutNumberOrBlank(
		gBG0TilemapBuffer + TILEMAP_INDEX(7, 17),
		TEXT_COLOR_SYSTEM_GOLD,
		GetUnitHpGrowth(gStatScreen.unit));

	ResetActiveFontPal();
}

STATIC_DECLAR void DisplayHpBmValue(void)
{
	struct Unit *unit = gStatScreen.unit;

	int hpcur = GetUnitCurrentHp(unit);
	int hpmax = GetUnitMaxHp(unit);

	int color = hpcur == hpmax
			  ? TEXT_COLOR_SYSTEM_GREEN
			  : TEXT_COLOR_SYSTEM_BLUE;

	// Display '/' labels
	PutSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(5, 17), TEXT_COLOR_SYSTEM_GOLD, TEXT_SPECIAL_SLASH);

	/* Display current hp */
	if (hpcur > 99)
		PutTwoSpecialChar(
			gBG0TilemapBuffer + TILEMAP_INDEX(3, 17),
			color, 0x14, 0x14);
	else
		PutNumberOrBlank(
			gBG0TilemapBuffer + TILEMAP_INDEX(4, 17),
			color, hpcur);

	/* Display max hp */
	if (hpmax > 99)
		PutTwoSpecialChar(
			gBG0TilemapBuffer + TILEMAP_INDEX(6, 17),
			color, 0x14, 0x14);
	else
		PutNumberOrBlank(
			gBG0TilemapBuffer + TILEMAP_INDEX(7, 17),
			color, hpmax);
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
	const char *namestr = GetStringFromIndex(UNIT_NAME_ID(gStatScreen.unit));
	unsigned int namexoff = GetStringTextCenteredPos(0x30, namestr);

	InstallExpandedTextPal();
	BG_Fill(gBG0TilemapBuffer, 0);

	// Generate battle stats for unit for display later
	BattleGenerateUiStats(
		gStatScreen.unit,
		GetUnitEquippedWeaponSlot(gStatScreen.unit));

	// Display character name
	PutDrawText(
		&gStatScreen.text[STATSCREEN_TEXT_CHARANAME],
		gBG0TilemapBuffer + TILEMAP_INDEX(3, 10),
		TEXT_COLOR_SYSTEM_WHITE, namexoff, 0, namestr);

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
	if (GetUnitCurrentHp(gStatScreen.unit) > 99) {
		// Display '--' if current hp > 99
		PutTwoSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(3, 17), TEXT_COLOR_SYSTEM_BLUE,
			TEXT_SPECIAL_DASH, TEXT_SPECIAL_DASH);
	} else {
		// Display current hp
		PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(4, 17), TEXT_COLOR_SYSTEM_BLUE,
			GetUnitCurrentHp(gStatScreen.unit));
	}

	// Display max hp
	if (GetUnitMaxHp(gStatScreen.unit) > 99) {
		// Display '--' if max hp > 99
		PutTwoSpecialChar(gBG0TilemapBuffer + TILEMAP_INDEX(6, 17), TEXT_COLOR_SYSTEM_BLUE,
			TEXT_SPECIAL_DASH, TEXT_SPECIAL_DASH);
	} else {
		// Display max hp
		PutNumberOrBlank(gBG0TilemapBuffer + TILEMAP_INDEX(7, 17), TEXT_COLOR_SYSTEM_BLUE,
			GetUnitMaxHp(gStatScreen.unit));
	}
#endif
}
