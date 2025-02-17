#include <common-chax.h>
#include <battle-system.h>
#include <kernel-lib.h>
#include <weapon-range.h>
#include <combat-art.h>
#include <kernel-tutorial.h>
#include <constants/texts.h>

#include <gaiden-magic.h>

STATIC_DECLAR const struct MenuDef sGaidenWMagItemMenuDef;
STATIC_DECLAR const struct MenuItemDef sGaidenWMagItemMenuItems[];
STATIC_DECLAR u8 GaidenWMagHelpbox(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR u8 GaidenWMagItemSelUsability(const struct MenuItemDef *def, int number);
STATIC_DECLAR int GaidenWMagItemSelOnDraw(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR u8 GaidenWMagItemSelEffect(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR int GaidenWMagItemSelHover(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR int GaidenWMagItemSelUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR void UpdateMenuItemPanelGaidenWMag(int slot);

STATIC_DECLAR bool CanUnitUseGaidenWMagItem(struct Unit *unit, int item)
{
	/* Replacement of CanUnitUseItem() */
	int staff_wexp = unit->ranks[ITYPE_STAFF];
	bool ret;

	/**
	 * Here we must give the unit the highest level
	 * so that we may directly skip the judgement on staff required weapon rank!
	 * The wexp judgement has been completed in GetGaidenMagicList()!
	 */
	unit->ranks[ITYPE_STAFF] = WPN_EXP_S;
	ret = CanUnitUseItem(unit, MakeNewItem(item));

	unit->ranks[ITYPE_STAFF] = staff_wexp;
	return ret;
}

/**
 * Action menu
 */
u8 GaidenWMagActionCommandUsability(const struct MenuItemDef *def, int number)
{
	int i;
	struct Unit *unit = gActiveUnit;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);

	if (gActiveUnit->state & (US_HAS_MOVED | US_IN_BALLISTA))
		return MENU_NOTSHOWN;

	if (gpKernelDesigerConfig->gaiden_magic_en == false)
		return MENU_NOTSHOWN;

	if (!list)
		return MENU_NOTSHOWN;

	for (i = 0; i < list->wmag_cnt; i++) {
		int item = list->wmags[i];

		if (item == ITEM_NONE)
			continue;

		/* W.Mag must be a staff! */
		if (GetItemType(item) != ITYPE_STAFF)
			continue;

		if (!CanUnitUseGaidenMagic(unit, item))
			continue;

		if (CanUnitUseGaidenWMagItem(unit, item)) {
			if (IsUnitMagicSealed(unit))
				return MENU_DISABLED;
			else
				return MENU_ENABLED;
		}
	}

	return MENU_NOTSHOWN;
}

int GaidenWMagActionCommandOnDarw(struct MenuProc *menu, struct MenuItemProc *item)
{
	int color = TEXT_COLOR_SYSTEM_WHITE;

	if (!CheckKtutFlagTriggered(KTUTORIAL_GAIDEN_WMAG))
		color = TEXT_COLOR_SYSTEM_GREEN;

	Text_SetColor(&item->text, color);
	Text_DrawString(&item->text, GetStringFromIndex(item->def->nameMsgId));

	 PutText(
		&item->text,
		TILEMAP_LOCATED(BG_GetMapBuffer(menu->frontBg), item->xTile, item->yTile));

	return 0;
}

u8 GaidenWMagActionCommandEffect(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct MenuProc *sub_menu;

	if (menuItem->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_0851);
		return MENU_ACT_SND6B;
	}

	ResetIconGraphics();
	LoadIconPalettes(4);

	sub_menu = StartOrphanMenu(&sGaidenWMagItemMenuDef);

	StartFace(0, GetUnitPortraitId(gActiveUnit), 0xB0, 0xC, 2);
	SetFaceBlinkControlById(0, 1);

	ForceMenuItemPanel(sub_menu, gActiveUnit, 0xF, 0xB);
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

int GaidenWMagActionCommandHover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	int i, mask = 0;
	struct Unit *unit = gActiveUnit;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);

	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);

	if (list) {
		for (i = 0; i < list->wmag_cnt; i++) {
			int item = list->wmags[i];

			if (item == ITEM_NONE)
				continue;

			if (!CanUnitUseGaidenMagicNow(unit, item))
				continue;

			mask |= GetItemReachBitsRework(item, unit);
		}
	}

	GenerateUnitStandingReachRange(gActiveUnit, mask);
	DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_GREEN);
	return 0;
}

/**
 * Item menu
 */
STATIC_DECLAR const struct MenuDef sGaidenWMagItemMenuDef = {
	{1, 1, 0xE, 0},
	0,
	sGaidenWMagItemMenuItems,
	0, 0, 0,
	ItemMenu_ButtonBPressed,
	MenuAutoHelpBoxSelect,
	GaidenWMagHelpbox
};

STATIC_DECLAR const struct MenuItemDef sGaidenWMagItemMenuItems[] = {
	{"", 0, 0, 0, 0x49, GaidenWMagItemSelUsability, GaidenWMagItemSelOnDraw, GaidenWMagItemSelEffect, 0, GaidenWMagItemSelHover, GaidenWMagItemSelUnhover},
	{"", 0, 0, 0, 0x4A, GaidenWMagItemSelUsability, GaidenWMagItemSelOnDraw, GaidenWMagItemSelEffect, 0, GaidenWMagItemSelHover, GaidenWMagItemSelUnhover},
	{"", 0, 0, 0, 0x4B, GaidenWMagItemSelUsability, GaidenWMagItemSelOnDraw, GaidenWMagItemSelEffect, 0, GaidenWMagItemSelHover, GaidenWMagItemSelUnhover},
	{"", 0, 0, 0, 0x4C, GaidenWMagItemSelUsability, GaidenWMagItemSelOnDraw, GaidenWMagItemSelEffect, 0, GaidenWMagItemSelHover, GaidenWMagItemSelUnhover},
	{"", 0, 0, 0, 0x4D, GaidenWMagItemSelUsability, GaidenWMagItemSelOnDraw, GaidenWMagItemSelEffect, 0, GaidenWMagItemSelHover, GaidenWMagItemSelUnhover},
	{"", 0, 0, 0, 0x4E, GaidenWMagItemSelUsability, GaidenWMagItemSelOnDraw, GaidenWMagItemSelEffect, 0, GaidenWMagItemSelHover, GaidenWMagItemSelUnhover},
	{"", 0, 0, 0, 0x4F, GaidenWMagItemSelUsability, GaidenWMagItemSelOnDraw, GaidenWMagItemSelEffect, 0, GaidenWMagItemSelHover, GaidenWMagItemSelUnhover},
	{0}
};

STATIC_DECLAR u8 GaidenWMagHelpbox(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);

	StartItemHelpBox(
		menuItem->xTile << 3,
		menuItem->yTile << 3,
		list->wmags[menuItem->itemNumber]);

	return 0;
}

STATIC_DECLAR u8 GaidenWMagItemSelUsability(const struct MenuItemDef *def, int number)
{
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);
	int item = list->wmags[number];

	if (!list || list->wmag_cnt <= number)
		return MENU_NOTSHOWN;

	if (!(GetItemAttributes(item) & ITYPE_STAFF))
		return MENU_NOTSHOWN;

	if (!CanUnitUseGaidenWMagItem(gActiveUnit, item))
		return MENU_NOTSHOWN;

	if (!CanUnitUseGaidenMagicNow(gActiveUnit, item))
		return MENU_DISABLED;

	return MENU_ENABLED;
}

STATIC_DECLAR int GaidenWMagItemSelOnDraw(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);
	int item = list->wmags[menuItem->itemNumber];
	u16 *tm = gBG0TilemapBuffer + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile);
	bool en = CanUnitUseGaidenMagicNow(gActiveUnit, item);

	DrawGaidenMagItemMenuLine(
		&menuItem->text,
		item,
		en,
		tm
	);

	PutGaidenMagicCostNumber(
		tm + 11,
		en ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY,
		GetGaidenWeaponHpCost(gActiveUnit, item));

	return 0;
}

STATIC_DECLAR u8 GaidenWMagItemSelEffect(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);
	int item = list->wmags[menuItem->itemNumber];

	if (menuItem->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_GaidenMagic_ItemMenuDisabled);
		return MENU_ACT_SND6B;
	}

	ClearBg0Bg1();
	gActionData.itemSlotIndex = CHAX_BUISLOT_GAIDEN_WMAG1 + menuItem->itemNumber;
	DoItemUse(gActiveUnit, item);

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_ENDFACE;
}

STATIC_DECLAR int GaidenWMagItemSelHover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	int slot = menuItem->itemNumber;
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);
	int weapon = list->wmags[slot];

	UpdateMenuItemPanelGaidenWMag(slot);

	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);

	RegisterCombatArtStatus(gActiveUnit, GetBestRangeBonusCid(gActiveUnit, weapon));
	GenerateUnitStandingReachRange(gActiveUnit, GetItemReachBitsRework(weapon, gActiveUnit));
	DisplayMoveRangeGraphics(MOVLIMITV_RMAP_GREEN);
	return 0;
}

STATIC_DECLAR int GaidenWMagItemSelUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	if (!(menu->state & MENU_STATE_ENDING))
		HideMoveRangeGraphics();

	return 0;
}

/**
 * Misc
 */
STATIC_DECLAR void UpdateMenuItemPanelGaidenWMag(int slot)
{
	struct MenuItemPanelProc *proc = Proc_Find(gProcCmd_MenuItemPanel);
	u16 *bg_base = BG_GetMapBuffer(0) + proc->x + 0x20 * proc->y;
	struct Text *texts = &proc->text[0];
	struct Unit *unit = proc->unit;
	int i;
	char *str;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);
	int item = list->wmags[slot];

	ClearText(&proc->text[0]);
	ClearText(&proc->text[1]);
	ClearText(&proc->text[2]);

	DrawUiFrame2(proc->x, proc->y, 0xE, 0x8, 0x0);

	str = GetStringFromIndex(GetItemUseDescId(item));

	for (i = 0; i < 3; i++) {
		Text_InsertDrawString(&texts[i], 0, TEXT_COLOR_SYSTEM_WHITE, str);
		str = GetStringLineEnd(str);

		if (*str == '\0')
			break;

		str = str + 1;
	}

	gBattleTarget.battleAttack = gBattleActor.battleAttack;
	gBattleTarget.battleHitRate = gBattleActor.battleHitRate;
	gBattleTarget.battleCritRate = gBattleActor.battleCritRate;
	gBattleTarget.battleAvoidRate = gBattleActor.battleAvoidRate;

	PutText(&texts[0], TILEMAP_LOCATED(bg_base, 1, 1));
	PutText(&texts[1], TILEMAP_LOCATED(bg_base, 1, 3));
	PutText(&texts[2], TILEMAP_LOCATED(bg_base, 1, 5));

	BG_EnableSyncByMask(BG0_SYNC_BIT);
}
