#include <common-chax.h>
#include <battle-system.h>
#include <kernel-lib.h>
#include <weapon-range.h>
#include <combat-art.h>
#include <kernel-tutorial.h>
#include <constants/texts.h>

#include <gaiden-magic.h>

STATIC_DECLAR const struct MenuDef sGaidenBMagItemMenuDef;
STATIC_DECLAR const struct MenuItemDef sGaidenBMagItemMenuItems[];
STATIC_DECLAR u8 GaidenBMagHelpbox(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR u8 GaidenBMagItemSelMenuOnCancel(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR u8 GaidenBMagItemSelUsability(const struct MenuItemDef *def, int number);
STATIC_DECLAR int GaidenBMagItemSelOnDraw(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR u8 GaidenBMagItemSelEffect(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR int GaidenBMagItemSelHover(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR int GaidenBMagItemSelUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem);
STATIC_DECLAR const struct SelectInfo sSelectInfoGaidenBMag;
STATIC_DECLAR u8 GaidenBMagAttackMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget *target);
STATIC_DECLAR u8 GaidenBMagAttackMapSelect_Select(ProcPtr proc, struct SelectTarget *target);
STATIC_DECLAR u8 GaidenBMagAttackMapSelect_Cancel(ProcPtr proc, struct SelectTarget *target);
STATIC_DECLAR const struct ProcCmd ProcScr_PostGaidenBMagSelectTarget[];
STATIC_DECLAR void RebuildGaidenBMagSelMenu(void);
STATIC_DECLAR void UpdateMenuItemPanelGaidenBMag(int slot);

/**
 * Action menu
 */
u8 GaidenBMagActionCommandUsability(const struct MenuItemDef *def, int number)
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

	ResetCombatArtList();
	ResetCombatArtStatus();

	for (i = 0; i < list->bmag_cnt; i++) {
		int item = list->bmags[i];

		if (item == ITEM_NONE)
			continue;

		/* B.Mag must be a weapon! */
		if (!(GetItemAttributes(item) & IA_WEAPON))
			continue;

		if (!CanUnitUseGaidenMagicNow(unit, item))
			continue;

		if (CanUnitPlayCombatArt(unit, item))
			RegisterCombatArtStatus(unit, GetBestRangeBonusCid(unit, item));

		MakeTargetListForWeapon(unit, item);

		if (GetSelectTargetCount() != 0)
			return MENU_ENABLED;
	}

	return MENU_NOTSHOWN;
}

int GaidenBMagActionCommandOnDarw(struct MenuProc *menu, struct MenuItemProc *item)
{
	int color = TEXT_COLOR_SYSTEM_WHITE;

	if (!CheckKtutFlagTriggered(KTUTORIAL_GAIDEN_BMAG))
		color = TEXT_COLOR_SYSTEM_GREEN;

	Text_SetColor(&item->text, color);
	Text_DrawString(&item->text, GetStringFromIndex(item->def->nameMsgId));

	PutText(
		&item->text,
		TILEMAP_LOCATED(BG_GetMapBuffer(menu->frontBg), item->xTile, item->yTile));

	return 0;
}

u8 GaidenBMagActionCommandEffect(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct MenuProc *sub_menu;

	if (menuItem->availability == MENU_DISABLED)
		return MENU_ACT_SND6B;

	ResetIconGraphics();
	LoadIconPalettes(4);

	sub_menu = StartOrphanMenu(&sGaidenBMagItemMenuDef);

	if (gActiveUnit->pClassData->number != CLASS_PHANTOM) {
		StartFace(0, GetUnitPortraitId(gActiveUnit), 0xB0, 0xC, 2);
		SetFaceBlinkControlById(0, 1);
	}

	ForceMenuItemPanel(sub_menu, gActiveUnit, 0xF, 0xB);
	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

int GaidenBMagActionCommandHover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	int i, mask = 0;
	struct Unit *unit = gActiveUnit;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);

	ResetCombatArtList();
	ResetCombatArtStatus();

	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);

	if (list) {
		for (i = 0; i < list->bmag_cnt; i++) {
			int item = list->bmags[i];

			if (item == ITEM_NONE)
				continue;

			if (!CanUnitUseGaidenMagicNow(unit, item))
				continue;

			RegisterCombatArtStatus(unit, GetBestRangeBonusCid(unit, item));
			mask |= GetItemReachBitsRework(item, unit);
		}
	}

	GenerateUnitStandingReachRange(gActiveUnit, mask);
	DisplayMoveRangeGraphics(MOVLIMITV_MMAP_BLUE | MOVLIMITV_RMAP_RED);
	return 0;
}

int GaidenBMagActionCommandUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	ResetCombatArtList();
	ResetCombatArtStatus();
	HideMoveRangeGraphics();
	return 0;
}

/**
 * Item menu
 */
STATIC_DECLAR const struct MenuDef sGaidenBMagItemMenuDef = {
	{1, 1, 0xE, 0},
	0,
	sGaidenBMagItemMenuItems,
	0, 0, 0,
	GaidenBMagItemSelMenuOnCancel,
	MenuAutoHelpBoxSelect,
	GaidenBMagHelpbox
};

u8 GaidenBMagHelpbox(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);

	StartItemHelpBox(
		menuItem->xTile << 3,
		menuItem->yTile << 3,
		list->bmags[menuItem->itemNumber]);

	return 0;
}

STATIC_DECLAR u8 GaidenBMagItemSelMenuOnCancel(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	ResetCombatArtStatus();
	return ItemMenu_ButtonBPressed(menu, menuItem);
}

STATIC_DECLAR const struct MenuItemDef sGaidenBMagItemMenuItems[] = {
	{"", 0, 0, 0, 0x49, GaidenBMagItemSelUsability, GaidenBMagItemSelOnDraw, GaidenBMagItemSelEffect, 0, GaidenBMagItemSelHover, GaidenBMagItemSelUnhover},
	{"", 0, 0, 0, 0x4A, GaidenBMagItemSelUsability, GaidenBMagItemSelOnDraw, GaidenBMagItemSelEffect, 0, GaidenBMagItemSelHover, GaidenBMagItemSelUnhover},
	{"", 0, 0, 0, 0x4B, GaidenBMagItemSelUsability, GaidenBMagItemSelOnDraw, GaidenBMagItemSelEffect, 0, GaidenBMagItemSelHover, GaidenBMagItemSelUnhover},
	{"", 0, 0, 0, 0x4C, GaidenBMagItemSelUsability, GaidenBMagItemSelOnDraw, GaidenBMagItemSelEffect, 0, GaidenBMagItemSelHover, GaidenBMagItemSelUnhover},
	{"", 0, 0, 0, 0x4D, GaidenBMagItemSelUsability, GaidenBMagItemSelOnDraw, GaidenBMagItemSelEffect, 0, GaidenBMagItemSelHover, GaidenBMagItemSelUnhover},
	{"", 0, 0, 0, 0x4E, GaidenBMagItemSelUsability, GaidenBMagItemSelOnDraw, GaidenBMagItemSelEffect, 0, GaidenBMagItemSelHover, GaidenBMagItemSelUnhover},
	{"", 0, 0, 0, 0x4F, GaidenBMagItemSelUsability, GaidenBMagItemSelOnDraw, GaidenBMagItemSelEffect, 0, GaidenBMagItemSelHover, GaidenBMagItemSelUnhover},
	{0}
};

STATIC_DECLAR u8 GaidenBMagItemSelUsability(const struct MenuItemDef *def, int number)
{
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);
	int item = list->bmags[number];

	if (!list || list->bmag_cnt <= number)
		return MENU_NOTSHOWN;

	if (!(GetItemAttributes(item) & IA_WEAPON))
		return MENU_NOTSHOWN;

	if (!CanUnitUseGaidenMagicNow(gActiveUnit, item))
		return MENU_DISABLED;

	RegisterCombatArtStatus(gActiveUnit, GetBestRangeBonusCid(gActiveUnit, item));
	MakeTargetListForWeapon(gActiveUnit, item);

	if (GetSelectTargetCount() == 0)
		return MENU_DISABLED;

	return MENU_ENABLED;
}

STATIC_DECLAR int GaidenBMagItemSelOnDraw(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);
	int item = list->bmags[menuItem->itemNumber];
	u16 *tm = gBG0TilemapBuffer + TILEMAP_INDEX(menuItem->xTile, menuItem->yTile);
	bool en = true;

	if (!CanUnitUseGaidenMagicNow(gActiveUnit, item))
		en = false;

	RegisterCombatArtStatus(gActiveUnit, GetBestRangeBonusCid(gActiveUnit, item));
	MakeTargetListForWeapon(gActiveUnit, item);
	if (GetSelectTargetCount() == 0)
		en = false;

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

STATIC_DECLAR u8 GaidenBMagItemSelEffect(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);
	int item = list->bmags[menuItem->itemNumber];

	if (menuItem->availability == MENU_DISABLED) {
		MenuFrozenHelpBox(menu, MSG_GaidenMagic_ItemMenuDisabled);
		return MENU_ACT_SND6B;
	}

	ClearBg0Bg1();
	gActionData.itemSlotIndex = CHAX_BUISLOT_GAIDEN_BMAG1 + menuItem->itemNumber;

	MakeTargetListForWeapon(gActiveUnit, item);
	NewTargetSelectionRework(&sSelectInfoGaidenBMag);

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_ENDFACE;
}

STATIC_DECLAR int GaidenBMagItemSelHover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	int slot = menuItem->itemNumber;
	struct GaidenMagicList *list = GetGaidenMagicList(gActiveUnit);
	int weapon = list->bmags[slot];

	UpdateMenuItemPanelGaidenBMag(slot);

	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);

	RegisterCombatArtStatus(gActiveUnit, GetBestRangeBonusCid(gActiveUnit, weapon));
	GenerateUnitStandingReachRange(gActiveUnit, GetItemReachBitsRework(weapon, gActiveUnit));
	DisplayMoveRangeGraphics(MOVLIMITV_RMAP_RED);
	return 0;
}

STATIC_DECLAR int GaidenBMagItemSelUnhover(struct MenuProc *menu, struct MenuItemProc *menuItem)
{
	if (!(menu->state & MENU_STATE_ENDING))
		HideMoveRangeGraphics();

	return 0;
}

/**
 * Select target
 */
STATIC_DECLAR const struct SelectInfo sSelectInfoGaidenBMag = {
	.onInit = MISMATCHED_SIGNATURE(NewBattleForecast),
	.onEnd = MISMATCHED_SIGNATURE(AttackMapSelect_End),
	.onSwitchIn = GaidenBMagAttackMapSelect_SwitchIn,
	.onSelect = GaidenBMagAttackMapSelect_Select,
	.onCancel = GaidenBMagAttackMapSelect_Cancel,
	.onHelp = MISMATCHED_SIGNATURE(StartBattleForecastHelpBox),
};

STATIC_DECLAR u8 GaidenBMagAttackMapSelect_SwitchIn(ProcPtr proc, struct SelectTarget *target)
{
	struct Unit *unit = GetUnit(target->uid);

	ChangeActiveUnitFacing(target->x, target->y);

	if (target->uid == 0) {
		gActionData.xOther = target->x;
		gActionData.yOther = target->y;
		gActionData.trapType = target->extra;
		InitObstacleBattleUnit();
	}

	BattleGenerateGaidenBMagSimulation(
		gActiveUnit, unit, gActiveUnit->xPos, gActiveUnit->yPos);

	UpdateBattleForecastContents();
	return 0;
}

STATIC_DECLAR u8 GaidenBMagAttackMapSelect_Select(ProcPtr proc, struct SelectTarget *target)
{
	if (EventEngineExists() == 1)
		return 0;

	gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat;
	gActionData.targetIndex = target->uid;

	if (target->uid == 0) {
		gActionData.xOther = target->x;
		gActionData.yOther = target->y;
		gActionData.trapType = target->extra;
	}

	// TriggerKtutorial(KTUTORIAL_GAIDEN_BMAG);
	PutKtutHistory(KTUTORIAL_GAIDEN_BMAG);

	Proc_EndEach(gProcScr_BKSEL);

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}

STATIC_DECLAR u8 GaidenBMagAttackMapSelect_Cancel(ProcPtr proc, struct SelectTarget *target)
{
	if (EventEngineExists() == 1)
		return 0;

	Proc_Start(ProcScr_PostGaidenBMagSelectTarget, PROC_TREE_3);

	return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6B;
}

/**
 * Post select-target
 */
STATIC_DECLAR const struct ProcCmd ProcScr_PostGaidenBMagSelectTarget[] = {
	PROC_NAME("PostGaidenBMagSelectTarget"),
	PROC_CALL(LockGame),
	PROC_CALL(sub_8022E38),
	PROC_WHILE_EXISTS(gProcScr_BKSEL),
	PROC_WHILE_EXISTS(ProcScr_CamMove),
	PROC_CALL(RebuildGaidenBMagSelMenu),
	PROC_CALL(UnlockGame),
	PROC_END
};

STATIC_DECLAR void RebuildGaidenBMagSelMenu(void)
{
	struct MenuProc *proc;

	ResetTextFont();
	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSyncByMask(BG2_SYNC_BIT);

	/* The same as CombatArtActionCommandEffect() */
	ResetIconGraphics();
	LoadIconPalettes(4);

	proc = StartOrphanMenu(&sGaidenBMagItemMenuDef);
	ForceMenuItemPanel(proc, gActiveUnit, 0xF, 0xB);

	StartFace(0, GetUnitPortraitId(gActiveUnit), 0xB0, 0xC, 2);
	SetFaceBlinkControlById(0, 5);
}

/**
 * Misc
 */
STATIC_DECLAR void UpdateMenuItemPanelGaidenBMag(int slot)
{
	struct MenuItemPanelProc *proc = Proc_Find(gProcCmd_MenuItemPanel);
	u16 *bg_base = BG_GetMapBuffer(0) + proc->x + 0x20 * proc->y;
	struct Text *texts = &proc->text[0];
	struct Unit *unit = proc->unit;
	int i, color, icon_pal = proc->IconPalIndex;
	char *str;
	struct GaidenMagicList *list = GetGaidenMagicList(unit);
	int item = list->bmags[slot];

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

	BattleGenerateUiStats(unit, slot + CHAX_BUISLOT_GAIDEN_BMAG1);
	gBattleTarget.battleAttack = gBattleActor.battleAttack;
	gBattleTarget.battleHitRate = gBattleActor.battleHitRate;
	gBattleTarget.battleCritRate = gBattleActor.battleCritRate;
	gBattleTarget.battleAvoidRate = gBattleActor.battleAvoidRate;

	color = TEXT_COLOR_SYSTEM_BLUE;

	Text_InsertDrawString(&texts[0], 0x1C, TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x4F1));
	Text_InsertDrawString(&texts[1], 0x02, TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x4F3));
	Text_InsertDrawString(&texts[2], 0x02, TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x4F4));
	Text_InsertDrawString(&texts[1], 0x32, TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x501));
	Text_InsertDrawString(&texts[2], 0x32, TEXT_COLOR_SYSTEM_WHITE, GetStringFromIndex(0x4F5));

	Text_InsertDrawNumberOrBlank(&texts[1], 0x24, color, gBattleActor.battleAttack);
	Text_InsertDrawNumberOrBlank(&texts[2], 0x24, color, gBattleActor.battleHitRate);
	Text_InsertDrawNumberOrBlank(&texts[1], 0x54, color, gBattleActor.battleCritRate);
	Text_InsertDrawNumberOrBlank(&texts[2], 0x54, color, gBattleActor.battleAvoidRate);

	PutText(&proc->text[0], TILEMAP_LOCATED(gBG0TilemapBuffer, proc->x + 1, proc->y + 0x1));
	PutText(&proc->text[1], TILEMAP_LOCATED(gBG0TilemapBuffer, proc->x + 1, proc->y + 0x3));
	PutText(&proc->text[2], TILEMAP_LOCATED(gBG0TilemapBuffer, proc->x + 1, proc->y + 0x5));

	DrawIcon(
		TILEMAP_LOCATED(bg_base, 8, 1),
		GetItemType(item) + 0x70,
		icon_pal << 0xC);

	BG_EnableSyncByMask(BG0_SYNC_BIT);
}
