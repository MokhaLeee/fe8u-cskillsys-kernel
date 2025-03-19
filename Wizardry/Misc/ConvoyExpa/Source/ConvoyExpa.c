#include "common-chax.h"

enum { CHAX_CONVOY_MAX_VOLUME = 0x300 };

extern int sExpaConvoyItemCount; // gConvoyItemCount is just a u8 that can be no more than 255, not enough
extern u16 sExpaConvoyItemArray[CHAX_CONVOY_MAX_VOLUME];

extern const int sExpaConvoyItemAmount;

/* Save API */
void SaveExpaConvoy(u8 *dst, const u32 size)
{
	Assert(size <= sizeof(sExpaConvoyItemArray));
	WriteAndVerifySramFast(sExpaConvoyItemArray, dst, size);
}

void LoadExpaConvoy(u8 *src, const u32 size)
{
	Assert(size <= sizeof(sExpaConvoyItemArray));
	memset(sExpaConvoyItemArray, 0, sizeof(sExpaConvoyItemArray));
	ReadSramFast(src, sExpaConvoyItemArray, size);
}

/* External hook: this function is too short to inject lyn-jump! */
u16 *__GetConvoyItemArray(void)
{
#if CHAX
	return sExpaConvoyItemArray;
#else
	return gConvoyItemArray;
#endif
}

LYN_REPLACE_CHECK(HasConvoyAccess);
bool8 HasConvoyAccess(void)
{
	return true;
}

static void ClearSupplyItemsExt(void *dst)
{
	CpuFill16(0, dst, sExpaConvoyItemAmount * sizeof(u16));
}

LYN_REPLACE_CHECK(ClearSupplyItems);
void ClearSupplyItems(void)
{
#if CHAX
	ClearSupplyItemsExt(sExpaConvoyItemArray);
#else
	CpuFill16(0, gConvoyItemArray, CONVOY_ITEM_COUNT * sizeof(u16));
#endif
}

LYN_REPLACE_CHECK(ShrinkConvoyItemList);
void ShrinkConvoyItemList(void)
{
	u16 i;
	u16 *buffer = (void *) gGenericBuffer;
	u16 *bufferIt = buffer;
	u16 *convoy = GetConvoyItemArray();

	WARN_GENERIC_BUF_USED;

	ClearSupplyItemsExt(buffer);

#if CHAX
	for (i = 0; i < sExpaConvoyItemAmount; i++)
#else
	for (i = 0; i < CONVOY_ITEM_COUNT; i++)
#endif
	{
		if (*convoy != 0) {
			*bufferIt = *convoy;
			bufferIt++;
		}
		convoy++;
	}

	ClearSupplyItems();
	memcpy(GetConvoyItemArray(), buffer, sExpaConvoyItemAmount * sizeof(u16));

	WARN_GENERIC_BUF_RELEASED;
}

LYN_REPLACE_CHECK(GetConvoyItemCount);
int GetConvoyItemCount(void)
{
	int i;
	int count = 0;

#if CHAX
	u16 *convoy = sExpaConvoyItemArray;

	for (i = 0; i < sExpaConvoyItemAmount; i++)
#else
	u16 *convoy = gConvoyItemArray;

	for (i = 0; i < CONVOY_ITEM_COUNT; i++)
#endif
		if (convoy[i] != 0)
			count++;

	return count;
}

LYN_REPLACE_CHECK(RemoveItemFromConvoy);
void RemoveItemFromConvoy(int index)
{
#if CHAX
	sExpaConvoyItemArray[index] = 0;
#else
	gConvoyItemArray[index] = 0;
#endif

	ShrinkConvoyItemList();
}

LYN_REPLACE_CHECK(GetConvoyItemSlot);
int GetConvoyItemSlot(int item)
{
	int i;
	u16 *convoy;

	item = GetItemIndex(item);

#if CHAX
	convoy = GetConvoyItemArray();
#else
	convoy = gConvoyItemArray;
#endif

#if CHAX
	for (i = 0; i < sExpaConvoyItemAmount; i++)
#else
	for (i = 0; i < CONVOY_ITEM_COUNT; i++)
#endif
		if (item == ITEM_INDEX(convoy[i]))
			return i;

	return -1;
}

LYN_REPLACE_CHECK(AddItemToConvoy);
int AddItemToConvoy(int item)
{
	int i;
	u16 *convoy;

	gBmSt.itemUnk2E = 0;

#if CHAX
	convoy = sExpaConvoyItemArray;
	for (i = 0; i < sExpaConvoyItemAmount; i++)
#else
	convoy = gConvoyItemArray;
	for (i = 0; i < CONVOY_ITEM_COUNT; i++)
#endif
	{
		if (convoy[i] == 0) {
			convoy[i] = item;
			return i;
		}
	}
	gBmSt.itemUnk2E = item;
	return -1;
}

LYN_REPLACE_CHECK(GetConvoyItemCostSum);
int GetConvoyItemCostSum(void)
{
	int i, result = 0;
	const u16 *convoy = GetConvoyItemArray();

#if CHAX
	for (i = 0; (i < sExpaConvoyItemAmount) && (*convoy); i++)
#else
	for (i = 0; (i < CONVOY_ITEM_COUNT) && (*convoy); i++)
#endif
	{
		result += GetItemCost(*convoy);
		convoy++;
	}
	return result;
}

LYN_REPLACE_CHECK(Shop_CheckIfConvoyFull);
void Shop_CheckIfConvoyFull(struct ProcShop *proc)
{
#if CHAX
	if (GetConvoyItemCount() < sExpaConvoyItemAmount)
#else
	if (GetConvoyItemCount() < CONVOY_ITEM_COUNT)
#endif
		Proc_Goto(proc, PL_SHOP_SENDTO_INVENTORY_EXT);
}

LYN_REPLACE_CHECK(BonusClaim_StartSelectTargetSubMenu);
void BonusClaim_StartSelectTargetSubMenu(struct BonusClaimProc *proc)
{
	int i;

	struct Text *th = gpBonusClaimText + 12;
	int sl = proc->targets;
	int tmp = (proc->targets * 2);

	DrawUiFrame2(11, 5, 14, tmp + 2, 1);

	gLCDControlBuffer.dispcnt.win0_on = 1;
	gLCDControlBuffer.dispcnt.win1_on = 1;
	gLCDControlBuffer.dispcnt.objWin_on = 0;

	gLCDControlBuffer.wincnt.win0_enableBg0 = 1;
	gLCDControlBuffer.wincnt.win0_enableBg1 = 1;
	gLCDControlBuffer.wincnt.win0_enableBg2 = 0;
	gLCDControlBuffer.wincnt.win0_enableBg3 = 1;
	gLCDControlBuffer.wincnt.win0_enableObj = 1;

	gLCDControlBuffer.win0_left = 88;
	gLCDControlBuffer.win0_top = 40;
	gLCDControlBuffer.win0_right = 200;
	gLCDControlBuffer.win0_bottom = (tmp + 7) * 8;

	SetUiCursorHandConfig(0, 40, proc->menuIndex * 16 + 56 - proc->unk_2c, 1);

	ShowSysHandCursor(92, proc->submenuIndex * 16 + 48, 12, 0x800);

	for (i = 0; i < sl; th++, i++) {
		int count;
		int color = 0;
		struct Unit *unit = gpBonusClaimConfig[i].unit;
		u16 *tm = gBG0TilemapBuffer + 14;

		ClearText(th);
		Text_SetCursor(th, 0);

		if (i == sl - 1) {
			count = GetConvoyItemCount();

#if CHAX
			color = (count == sExpaConvoyItemAmount) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE;
#else
			color = (count == CONVOY_ITEM_COUNT) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE;
#endif

			Text_SetParams(th, 0, color);
			Text_DrawString(th, GetStringFromIndex(0x308)); // TODO: msgid "Supply"
		} else {
			count = GetUnitItemCount(unit);
			color = (count == UNIT_ITEM_COUNT) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE;
			Text_SetParams(th, 0, color);
			Text_DrawString(th, GetStringFromIndex(unit->pCharacterData->nameTextId));
		}

		if (color == 0)
			gpBonusClaimConfig[i].hasInventorySpace = 1;
		else
			gpBonusClaimConfig[i].hasInventorySpace = 0;

		PutText(th, tm + 0xc0 + 0x40 * i);

		PutNumber(
			tm + 0xc9 + 0x40 * i,
			color == 0 ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY,
			count
		);
	}

	proc->unk_34 = StartParallelWorker(BonusClaim_DrawTargetUnitSprites, proc);
}

LYN_REPLACE_CHECK(ConvoyMenuProc_StarMenu);
int ConvoyMenuProc_StarMenu(ProcPtr proc)
{
#if CHAX
	sExpaConvoyItemCount = GetConvoyItemCount();
#else
	gConvoyItemCount = GetConvoyItemCount();
#endif

	LoadIconPalettes(4);

#if CHAX
	if (HasConvoyAccess() && (sExpaConvoyItemCount < sExpaConvoyItemAmount))
#else
	if (HasConvoyAccess() && (gConvoyItemCount < CONVOY_ITEM_COUNT))
#endif
		StartMenu(&gSendToConvoyMenuDef, (ProcPtr)proc);
	else
		StartMenu(&gConvoyMenuDef, (ProcPtr)proc);

	return 0;
}

LYN_REPLACE_CHECK(ConvoyMenuProc_ExecBootlegPopup);
void ConvoyMenuProc_ExecBootlegPopup(ProcPtr proc)
{
	if (HasConvoyAccess()) {
		if (sExpaConvoyItemCount < sExpaConvoyItemAmount)
			NewPopup2_SendItem(proc, gActionData.item);
		else
			NewPopup2_DropItem(proc, gActionData.item);
	} else
		NewPopup2_DropItem(proc, gActionData.item);
}

LYN_REPLACE_CHECK(HandleNewItemGetFromDrop);
void HandleNewItemGetFromDrop(struct Unit *unit, int item, ProcPtr proc)
{
	u8 ret = UnitAddItem(unit, item);

	if (ret != false)
		return;

	gActiveUnit = unit;
	gBmSt.um_tmp_item = item;
	StartFace(0, GetUnitPortraitId(unit), 0xB0, 4, 2);
	SetFaceBlinkControlById(0, 5);
	ForceMenuItemPanel(proc, unit, 0xF, 0xA);

#if CHAX
	if (HasConvoyAccess() && (sExpaConvoyItemCount < sExpaConvoyItemAmount))
#else
	if (HasConvoyAccess() && GetConvoyItemCount() < CONVOY_ITEM_COUNT)
#endif
		/* Your inventory is full. Send an item to Supply.[.] */
		StartSubtitleHelp(proc, GetStringFromIndex(0x867));
	else
		/* Your inventory is full. Select an item to discard. */
		StartSubtitleHelp(proc, GetStringFromIndex(0x866));

	SetTalkChoiceResult(TALK_CHOICE_NO);
	Proc_StartBlocking(gProcCmd_ConvoyMenu, proc);
}

LYN_REPLACE_CHECK(PrepItemScreen_GiveAll);
bool PrepItemScreen_GiveAll(struct Unit *unit)
{
	int i;
	int unitItemCount = GetUnitItemCount(unit);
#if CHAX
	int convoyItemCount = GetConvoyItemCount();

	for (i = 0; (i < unitItemCount) && ((i + convoyItemCount) < sExpaConvoyItemAmount); i++)
#else
	int convoyItemCount = GetConvoyItemCount_();

	for (i = 0; (i < unitItemCount) && ((i + convoyItemCount) < CONVOY_ITEM_COUNT); i++)
#endif
	{
		AddItemToConvoy(unit->items[0]);
		UnitRemoveItem(unit, 0);
	}

	if (i > 0)
		return true;

	return false;
}

LYN_REPLACE_CHECK(sub_809D644);
void sub_809D644(struct PrepItemSupplyProc *proc)
{
	int color;
	struct Text *th;
#if CHAX
	int convoyItemCount = GetConvoyItemCount();
#else
	int convoyItemCount = GetConvoyItemCount_();
#endif
	int unitItemCount = GetUnitItemCount(proc->unit);

	SetTextFont(&PrepItemSuppyTexts.font);
	SetTextFontGlyphs(0);

	SpriteText_DrawBackgroundExt(&PrepItemSuppyTexts.th[0xf], 0);
	th = &PrepItemSuppyTexts.th[0xf];

	color = 0;

#if CHAX
	if ((convoyItemCount == sExpaConvoyItemAmount) || (unitItemCount == 0))
#else
	if ((convoyItemCount == CONVOY_ITEM_COUNT) || (unitItemCount == 0))
#endif
		color = 1;

	Text_InsertDrawString(
		th,
		0,
		color,
		GetStringFromIndex(0x59E) // TODO: msgid "Give"
	);

	Text_InsertDrawString(
		&PrepItemSuppyTexts.th[0xf],
		0x40,
		unitItemCount == UNIT_ITEM_COUNT ? 1 : 0,
		GetStringFromIndex(0x59F) // TODO: msgid "Take"
	);

	SetTextFont(NULL);
}

LYN_REPLACE_CHECK(sub_809D6CC);
void sub_809D6CC(void)
{
	SetTextFont(NULL);
	TileMap_FillRect(gBG0TilemapBuffer + 0x34, 12, 1, 0);

	PutDrawText(&PrepItemSuppyTexts.th[0], gBG0TilemapBuffer + 0x34 + 0x6d, 0, 2, 0, GetStringFromIndex(0x598));
	PutFaceChibi(FID_SUPPLY, gBG0TilemapBuffer + 0x34 - 0x13, 0x270, 2, 1);
	PutDrawText(&PrepItemSuppyTexts.th[0] + 1, gBG0TilemapBuffer + 0x34 - 1, 0, 4, 0, GetStringFromIndex(0x5a0));

	PutNumber(
		gBG0TilemapBuffer + 0x34 + 5,
#if CHAX
		(GetConvoyItemCount() == sExpaConvoyItemAmount) ? 4 : 2,
		GetConvoyItemCount()
#else
		(GetConvoyItemCount_() == CONVOY_ITEM_COUNT) ? 4 : 2,
		GetConvoyItemCount_()
#endif
	);
	PutSpecialChar(gBG0TilemapBuffer + 0x34 + 6, TEXT_COLOR_SYSTEM_WHITE, TEXT_SPECIAL_SLASH);

#if CHAX
	PutNumber(gBG0TilemapBuffer + 0x34 + 9, 2, sExpaConvoyItemAmount);
#else
	PutNumber(gBG0TilemapBuffer + 0x34 + 9, 2, CONVOY_ITEM_COUNT);
#endif

	BG_EnableSyncByMask(1);
}

LYN_REPLACE_CHECK(PrepItemSupply_Loop_GiveTakeKeyHandler);
void PrepItemSupply_Loop_GiveTakeKeyHandler(struct PrepItemSupplyProc *proc)
{
	int idx = proc->unk_33;

	if (proc->unk_38 == 0) {
		if (gKeyStatusPtr->newKeys & A_BUTTON) {
			switch (idx) {
			case 0:
#if CHAX
				if ((GetConvoyItemCount() < sExpaConvoyItemAmount) && (GetUnitItemCount(proc->unit) > 0))
#else
				if ((GetConvoyItemCount_() < CONVOY_ITEM_COUNT) && (GetUnitItemCount(proc->unit) > 0))
#endif
				{
					SetUiCursorHandConfig(0, 68, proc->unk_33 * 16 + 36, 2);
					Proc_End(GetParallelWorker(PutGiveTakeBoxSprites));
					StartParallelWorker(PutGiveSprites, proc);
					sub_809D278(1, proc);
					PlaySoundEffect(0x6a);
					Proc_Goto(proc, 3);
					return;
				}

				PlaySoundEffect(0x6c);
				break;
			case 1:
				if (GetUnitItemCount(proc->unit) < UNIT_ITEM_COUNT) {
					SetUiCursorHandConfig(0, 68, proc->unk_33 * 16 + 36, 2);
					Proc_End(GetParallelWorker(PutGiveTakeBoxSprites));
					StartParallelWorker(PutTakeSprites, proc);
					sub_809D278(2, proc);
					PlaySoundEffect(0x6a);
					Proc_Goto(proc, 4);
					return;
				}

				PlaySoundEffect(0x6c);
				break;
			}
			return;
		}

		if (gKeyStatusPtr->newKeys & B_BUTTON) {
			Proc_Goto(proc, 8);
			PlaySoundEffect(0x6b);
			return;
		}

		if (gKeyStatusPtr->newKeys & R_BUTTON) {
			StartHelpBox(68, proc->unk_33 * 16 + 36, gSupplyHelpTextIndexLookup[idx]);
			proc->unk_38 = 1;
			return;
		}
	} else if (gKeyStatusPtr->newKeys & (R_BUTTON | B_BUTTON)) {
		CloseHelpBox();
		proc->unk_38 = 0;
		return;
	}

	if (gKeyStatusPtr->repeatedKeys & DPAD_UP) {
		if (proc->unk_33 != 0)
			proc->unk_33--;
		else if (gKeyStatusPtr->newKeys & DPAD_UP)
			proc->unk_33 = 1;
	}

	if (gKeyStatusPtr->repeatedKeys & DPAD_DOWN) {
		if (proc->unk_33 == 0)
			proc->unk_33++;
		else if (gKeyStatusPtr->newKeys & DPAD_DOWN)
			proc->unk_33 = 0;
	}

	if (idx != proc->unk_33) {
		PlaySoundEffect(0x66);
		ShowSysHandCursor(68, proc->unk_33 * 16 + 36, 4, 0x400);
		if (proc->unk_38 != 0)
			StartHelpBox(68, proc->unk_33 * 16 + 36, gSupplyHelpTextIndexLookup[proc->unk_33]);
	}
}

LYN_REPLACE_CHECK(PrepItemSupply_GiveItemToSupply);
void PrepItemSupply_GiveItemToSupply(struct PrepItemSupplyProc *proc)
{
	int unitItemCount;
	u16 item = proc->unit->items[proc->unitInvIdx];

	GetUnitItemCount(proc->unit); // Necessary for match, but pointless
	proc->unit->items[proc->unitInvIdx] = 0;
	UnitRemoveInvalidItems(proc->unit);

	proc->currentPage = GetPrepPageForItem(item);
	AddItemToConvoy(item);

	SomethingPrepListRelated(proc->unit, proc->currentPage, 1);
	sub_809E100(proc);

	ResetIconGraphics_();
	DrawPrepScreenItems(gBG0TilemapBuffer + 0x122, &PrepItemSuppyTexts.th[2], proc->unit, 0);
	sub_809D300(&PrepItemSuppyTexts.th[7], gBG2TilemapBuffer + 0xF, proc->yOffsetPerPage[proc->currentPage] >> 4, proc->unit);
	StartParallelFiniteLoop(sub_809E2BC, 1, proc);

	BG_EnableSyncByMask(4);

	unitItemCount = GetUnitItemCount(proc->unit);

	gActionData.unitActionType = UNIT_ACTION_TRADED_SUPPLY;

#if CHAX
	if ((unitItemCount == 0) || (GetConvoyItemCount() == sExpaConvoyItemAmount))
#else
	if ((unitItemCount == 0) || (GetConvoyItemCount_() == CONVOY_ITEM_COUNT))
#endif
	{
		Proc_Goto(proc, 1);
		PlaySoundEffect(0x6b);
	} else {
		PlaySoundEffect(0x6a);
		if (unitItemCount <= proc->unitInvIdx) {
			proc->unitInvIdx = unitItemCount - 1;
			ShowSysHandCursor(16, proc->unitInvIdx * 16 + 72, 0xb, 0x800);
		}
	}
}

LYN_REPLACE_CHECK(SomethingPrepListRelated);
void SomethingPrepListRelated(struct Unit *pUnit, int page, int flags)
{
	struct PrepScreenItemListEnt *pPrepItemList = gPrepScreenItemList;

	gUnknown_02012F54 = 0;

	if (flags & 2) {
		int i;

		for (i = FACTION_BLUE + 1; i < FACTION_GREEN; i++) {
			int j;
			int itemCount;
			struct Unit *unit = GetUnit(i);

			if (!UNIT_IS_VALID(unit))
				continue;

			if (unit->state & (US_DEAD | US_BIT16))
				continue;

			if (unit == pUnit)
				continue;

			itemCount = GetUnitItemCount(unit);

			for (j = 0; j < itemCount; j++) {
				pPrepItemList->pid = unit->pCharacterData->number;
				pPrepItemList->item = unit->items[j];
				pPrepItemList->itemSlot = j;
				pPrepItemList++;

				gUnknown_02012F54++;
			}
		}
	}

	if (flags & 1) {
		int j;
		u16 *convoy = GetConvoyItemArray();

#if CHAX
		for (j = 0; j < sExpaConvoyItemAmount && convoy[j] != 0; j++)
#else
		for (j = 0; j < CONVOY_ITEM_COUNT && convoy[j] != 0; j++)
#endif
		{
			pPrepItemList->item = convoy[j];
			pPrepItemList->pid = 0;
			pPrepItemList->itemSlot = j;
			pPrepItemList++;

			gUnknown_02012F54++;
		}
	}
	sub_8098048(page);
}

LYN_REPLACE_CHECK(sub_80982FC);
void sub_80982FC(void)
{
	u16 i;

	ClearSupplyItems();

#if CHAX
	for (i = 0; i < CONVOY_ITEM_COUNT; i++) // fine
#else
	for (i = 0; i < CONVOY_ITEM_COUNT; i++)
#endif
		AddItemToConvoy(0x88 - i);
}
