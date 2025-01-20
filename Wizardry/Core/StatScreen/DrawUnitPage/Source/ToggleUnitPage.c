#include "common-chax.h"
#include "stat-screen.h"
#include "lvup.h"
#include "strmag.h"
#include "kernel-lib.h"

static void _growth_disp(int x, int y, int growth)
{
	int bank, color = GetTextColorFromGrowth(growth);

	ModifyTextPal(bank, color);
	gActiveFont->tileref = TILEREF(gActiveFont->tileref & 0xFFF, bank);

	PutNumberOrBlank(
		gBG0TilemapBuffer + TILEMAP_INDEX(x, y),
		color,
		growth);
}

STATIC_DECLAR void ToggleUnitPageGrowth(void)
{
	struct Unit *unit = gStatScreen.unit;

	_growth_disp(18, 3,  GetUnitPowGrowth(unit));
	_growth_disp(18, 5,  GetUnitMagGrowth(unit));
	_growth_disp(18, 7,  GetUnitSklGrowth(unit));
	_growth_disp(18, 9,  GetUnitSpdGrowth(unit));
	_growth_disp(18, 11, GetUnitLckGrowth(unit));
	_growth_disp(18, 13, GetUnitDefGrowth(unit));
	_growth_disp(18, 15, GetUnitResGrowth(unit));

	ResetActiveFontPal();
}

STATIC_DECLAR void ToggleUnitPageBm(void)
{
	struct Unit *unit = gStatScreen.unit;

	DrawStatWithBarRework(0, 0x5, 0x1,
					gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
					gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
					unit->pow,
					GetUnitPower(unit),
					UNIT_POW_MAX(unit));

	DrawStatWithBarRework(1, 0x5, 0x3,
					gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
					gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
					UNIT_MAG(unit),
					GetUnitMagic(unit),
					GetUnitMaxMagic(unit));

	DrawStatWithBarRework(2, 0x5, 0x5,
					gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
					gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
					unit->skl,
					GetUnitSkill(unit),
					UNIT_SKL_MAX(unit));

	DrawStatWithBarRework(3, 0x5, 0x7,
					gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
					gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
					unit->spd,
					GetUnitSpeed(unit),
					UNIT_SPD_MAX(unit));

	DrawStatWithBarRework(4, 0x5, 0x9,
					gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
					gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
					unit->lck,
					GetUnitLuck(unit),
					UNIT_LCK_MAX(unit));

	DrawStatWithBarRework(5, 0x5, 0xB,
					gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
					gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
					unit->def,
					GetUnitDefense(unit),
					UNIT_DEF_MAX(unit));

	DrawStatWithBarRework(6, 0x5, 0xD,
					gBG0TilemapBuffer + TILEMAP_INDEX(12, 2),
					gBG2TilemapBuffer + TILEMAP_INDEX(12, 2),
					unit->res,
					GetUnitResistance(unit),
					UNIT_RES_MAX(unit));
}



STATIC_DECLAR void ToggleUnitPage(bool toggle)
{
	TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 16, 3), 4, 13, 0);
	if (toggle == false)
		ToggleUnitPageBm();
	else {
		TileMap_FillRect(TILEMAP_LOCATED(gBG2TilemapBuffer, 15, 3), 6, 13, 0);
		ToggleUnitPageGrowth();
	}

	ToggleUnitLeftPage(toggle);
	BG_EnableSyncByMask(BG0_SYNC_BIT | BG2_SYNC_BIT);
}

LYN_REPLACE_CHECK(PageNumCtrl_DisplayBlinkIcons);
void PageNumCtrl_DisplayBlinkIcons(struct StatScreenPageNameProc *proc)
{
	bool blinking;
	static const u16 palidLut[3] = { 0xC, 0xE, 0xD }; // TODO: palid constants

	/* No idle in transition */
	if (gStatScreen.inTransition)
		return;

	blinking = (GetGameClock() % 32) < 20;

	if (gStatScreen.page == STATSCREEN_PAGE_0) {
		if (gStatScreen.unit->state & US_RESCUING) {
			UpdateStatArrowSprites(120, 56, 1);
			UpdateStatArrowSprites(120, 72, 1);

			if (blinking) {
				if (gStatScreenStExpa.talkee == 0)
					PutSprite(4,
						184, 94, gObject_8x8,
						TILEREF(3, 0xF & palidLut[gStatScreen.unit->rescue >> 6]) + OAM2_LAYER(2));
				else
					PutSprite(4,
						28, 86, gObject_8x8,
						TILEREF(3, 0xF & palidLut[gStatScreen.unit->rescue>>6]) + OAM2_LAYER(2));
			}
		}
	}

	if ((gStatScreen.unit->state & US_RESCUED) && blinking)
		PutSprite(4,
			28, 86, gObject_8x8,
			TILEREF(3, 0xF & palidLut[gStatScreen.unit->rescue>>6]) + OAM2_LAYER(2));

	/**
	 * We direcly put page toggle here since here has been a hook during statscreen IDLE.
	 * It's true that better to put such process into StatScreen_OnIdle(), but both are okay.
	 */
	switch (gpKernelDesigerConfig->unit_page_style) {
	case CONFIG_PAGE1_WITH_BWL:
	case CONFIG_PAGE1_WITH_LEADERSHIP:
		if ((gStatScreen.page == STATSCREEN_PAGE_0) && (UNIT_FACTION(gStatScreen.unit) == FACTION_BLUE)) {
			if (gStatScreenStExpa.toggle_timer == 0) {
				if ((gKeyStatusPtr->newKeys & SELECT_BUTTON))
					gStatScreenStExpa.toggle_timer = STATSCREEN_TOGGLE_DURATION;
			} else {
				gStatScreenStExpa.toggle_timer--;
				if (gStatScreenStExpa.toggle_timer == (STATSCREEN_TOGGLE_DURATION / 2)) {
					PlaySoundEffect(0x6A);

					gStatScreenStExpa.toggle = ~gStatScreenStExpa.toggle;
					ToggleUnitPage(gStatScreenStExpa.toggle);
				}
			}
		}

		/* Refrain left panel */
		if (gStatScreenStExpa.toggle && gStatScreen.page != STATSCREEN_PAGE_0) {
			gStatScreenStExpa.toggle_timer = 0;
			gStatScreenStExpa.toggle = false;
			ToggleUnitLeftPage(false);
			BG_EnableSyncByMask(BG0_SYNC_BIT);
		}
		break;

	default:
		break;
	}
}

LYN_REPLACE_CHECK(GlowBlendCtrl_OnLoop);
void GlowBlendCtrl_OnLoop(struct StatScreenEffectProc *proc)
{
	/**
	 * Since BG1 may also consume chr resource on texts,
	 * so I think it is better to directly remove them.
	 */
	return;
}
