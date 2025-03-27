#include "common-chax.h"
#include "icon-rework.h"
#include "kernel-lib.h"

LYN_REPLACE_CHECK(GetUnitAidIconId);
int GetUnitAidIconId(u32 attributes)
{
	if (attributes & CA_MOUNTED)
		return MOUNT_ICON(1);

	if (attributes & CA_PEGASUS)
		return MOUNT_ICON(2);

	if (attributes & CA_WYVERN)
		return MOUNT_ICON(3);

	return (-1);
}

LYN_REPLACE_CHECK(GetUnitAffinityIcon);
int GetUnitAffinityIcon(struct Unit *unit)
{
	int affinity = unit->pCharacterData->affinity;

	if (!affinity)
		return -1;

	return AFFIN_ICON(affinity);
}

LYN_REPLACE_CHECK(UpdateMenuItemPanel);
void UpdateMenuItemPanel(int slot_or_item)
{
	struct MenuItemPanelProc *proc = Proc_Find(gProcCmd_MenuItemPanel);
	u16 *bg_base = BG_GetMapBuffer(0) + proc->x + 0x20 * proc->y;
	struct Text *texts = &proc->text[0];
	struct Unit *unit = proc->unit;
	int i, item, color, icon_pal = proc->IconPalIndex;
	char *str;

	ClearText(&proc->text[0]);
	ClearText(&proc->text[1]);
	ClearText(&proc->text[2]);

	DrawUiFrame2(proc->x, proc->y, 0xE, 0x8, 0x0);

	switch (slot_or_item) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		item = unit->items[slot_or_item];
		break;

	case BU_ISLOT_5:
		item = gBmSt.um_tmp_item;
		break;

	default:
		item = slot_or_item;
		slot_or_item = BU_ISLOT_BALLISTA;
		break;
	}

	switch (GetItemType(item)) {
	case ITYPE_STAFF:
	case ITYPE_ITEM:
	case ITYPE_11:
	case ITYPE_12:
		str = GetStringFromIndex(GetItemUseDescId(item));
		i = 0;
		while (1) {
			Text_InsertDrawString(&texts[i], 0, TEXT_COLOR_SYSTEM_WHITE, str);
			str = GetStringLineEnd(str);

			if (*str == '\0')
				break;

			str++;
			i++;
		}

		gBattleActor.battleAttack = gBattleTarget.battleAttack;
		gBattleActor.battleHitRate = gBattleTarget.battleHitRate;
		gBattleActor.battleCritRate = gBattleTarget.battleCritRate;
		gBattleActor.battleAvoidRate = gBattleTarget.battleAvoidRate;

		PutText(&texts[0], TILEMAP_LOCATED(bg_base, 1, 1));
		PutText(&texts[1], TILEMAP_LOCATED(bg_base, 1, 3));
		PutText(&texts[2], TILEMAP_LOCATED(bg_base, 1, 5));
		break;

	default:
		BattleGenerateUiStats(unit, slot_or_item);

		if (slot_or_item == BU_ISLOT_BALLISTA) {
			gBattleTarget.battleAttack = gBattleActor.battleAttack;
			gBattleTarget.battleHitRate = gBattleActor.battleHitRate;
			gBattleTarget.battleCritRate = gBattleActor.battleCritRate;
			gBattleTarget.battleAvoidRate = gBattleActor.battleAvoidRate;
		}

		color = CanUnitUseWeapon(unit, gBattleActor.weapon) ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY;

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
#if CHAX
			WTYPE_ICON(GetItemType(gBattleActor.weapon)),
#else
			GetItemType(gBattleActor.weapon) + 0x70,
#endif
			icon_pal << 0xC);
		break;
	} /* switch item type */

	BG_EnableSyncByMask(BG0_SYNC_BIT);
}

LYN_REPLACE_CHECK(DrawBattlePopup);
void DrawBattlePopup(struct ProcEkrPopup *proc, int type, u32 priv)
{
	const char *str;
	int width1, width2, width3, width_popupbox, width5, xcursor, tmpr0;

	struct Text *text;
	struct Anim *anim;

	LZ77UnCompVram(Img_EkrPopup, (void *)BG_VRAM + 0x2000);
	LZ77UnCompWram(Tsa_EkrPopup, (void *)gEkrTsaBuffer);
	InitTextFont(&gBanimFont, (void *)BG_VRAM + 0x2100, 0x108, 1);
	SetTextDrawNoClear();
	CpuFastCopy(Pal_EkrPopup, PAL_BG(0x1), 0x20);

	if (type == 0) {
		/*  [.] */
		str = GetStringFromIndex(0x01);
		width1 = GetStringTextLen(str);
		width2 = width1 + 0x10;

		/* Weapon Level increased.[.] */
		str = GetStringFromIndex(0x02);
		width3 = GetStringTextLen(str) + width2;
	} else if (type == 1) {
		width1 = 0;
		str = GetItemNameWithArticle(priv, 1);
		width2 = GetStringTextLen(str) + 0x10;

		/* broke! */
		str = GetStringFromIndex(0x03);
		width3 = GetStringTextLen(str) + width2 + 0x04;
	} else {
		width1 = 0;
		width2 = 0x10;

		/* You can now use  */
		str = GetStringFromIndex(0x0D);
		width3 = GetStringTextLen(str) + 0x10;
	}

	width_popupbox = (width3 + 7) >> 3;
	MakeBattlePopupTileMapFromTSA(gBG1TilemapBuffer, width_popupbox);

	text = &gBanimText[0];
	InitText(text, width_popupbox);
	xcursor = (width_popupbox * 8 - width3) >> 1;
	Text_SetCursor(text, xcursor);
	LZ77UnCompVram(gUnknown_08803BD0, (void *)BG_VRAM + 0x2100);

	if (type == 0) {
		/*  [.] */
		str = GetStringFromIndex(0x01);
		Text_SetColor(text, TEXT_COLOR_SYSTEM_WHITE);
		Text_DrawString(text, str);

		Text_Skip(text, 0x10);

		/* Weapon Level increased.[.] */
		str = GetStringFromIndex(0x02);
		Text_SetColor(text, TEXT_COLOR_SYSTEM_WHITE);
		Text_DrawString(text, str);
	} else if (type == 1) {
		str = GetItemNameWithArticle(priv, 1);
		Text_SetColor(text, TEXT_COLOR_SYSTEM_GRAY);
		Text_DrawString(text, str);
		Text_Skip(text, 0x14);

		/* broke! */
		str = GetStringFromIndex(0x03);
		Text_SetColor(text, TEXT_COLOR_SYSTEM_WHITE);
		Text_DrawString(text, str);
	} else {
		/* You can now use  */
		str = GetStringFromIndex(0x0D);
		Text_SetColor(text, TEXT_COLOR_SYSTEM_WHITE);
		Text_DrawString(text, str);
	}

	width5 = (0xE0 - (width_popupbox + 2) * 8) / 2;
	BG_SetPosition(BG_1, -width5, 0xFFD0);
	BG_EnableSyncByMask(BG1_SYNC_BIT);
	ResetIconGraphics_();

	if (type == 0) {
		LoadIconPalette(1, 0x12);

#if !CHAX
		LoadIconObjectGraphics(GetItemType(priv) + 0x70, 0x40);
#else
		LoadIconObjectGraphics(WTYPE_ICON(GetItemType(priv)), 0x40);
#endif
	} else if (type == 1) {
		LoadIconPalette(0, 0x12);
		LoadIconObjectGraphics(GetItemIconId(priv), 0x40);
		tmpr0 = width5 - 0xE;
		width5 = tmpr0 + width2;
	} else {
		LoadIconPalette(1, 0x12);

#if !CHAX
		LoadIconObjectGraphics(priv + 0x70, 0x40);
#else
		LoadIconObjectGraphics(WTYPE_ICON(priv), 0x40);
#endif
		tmpr0 = width5 - 0x10;
		width5 = tmpr0 + width3;
	}

	anim = AnimCreate(AnimScr_EkrPopup, 0x96);
	proc->anim = anim;
	anim->oam2Base = OAM2_PAL(0x2) + OAM2_LAYER(0x1) + OAM2_CHR(0x0800 / 0x20);

	anim->xPosition = width5 + ({ xcursor + 0x10; }) + width1;
	anim->yPosition = 0x38;

	EnablePaletteSync();
	SetDefaultColorEffects();
	SetWinEnable(0, 0, 0);
}

LYN_REPLACE_CHECK(DisplayWeaponExp);
void DisplayWeaponExp(int num, int x, int y, int wtype)
{
	int progress, progressMax, color;

	int wexp = UNIT_WRANK(gStatScreen.unit, wtype);

	// Display weapon type icon
	DrawIcon(gUiTmScratchA + TILEMAP_INDEX(x, y),
		WTYPE_ICON(wtype),
		TILEREF(0, STATSCREEN_BGPAL_EXTICONS));

	color = wexp >= WPN_EXP_S
		? TEXT_COLOR_SYSTEM_GREEN
		: TEXT_COLOR_SYSTEM_BLUE;

	// Display rank letter
	PutSpecialChar(gUiTmScratchA + TILEMAP_INDEX(x + 4, y),
		color,
		GetDisplayRankStringFromExp(wexp));

	GetWeaponExpProgressState(wexp, &progress, &progressMax);

	DrawStatBarGfx(0x401 + num*6, 5,
		gUiTmScratchC + TILEMAP_INDEX(x + 2, y + 1), TILEREF(0, STATSCREEN_BGPAL_6),
		0x22, (progress*34)/(progressMax-1), 0);
}
