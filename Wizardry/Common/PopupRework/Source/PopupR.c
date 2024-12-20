#include <common-chax.h>
#include <icon-rework.h>
#include <popup-reowrk.h>

/**
 * Components
 */
static int PoprGetLen_Space(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	return inst->data;
}

static void PoprDisp_Space(struct Text *text, const struct PopupInstruction *inst)
{
	Text_Skip(text, inst->data);
}

static int PoprGetLen_ItemName(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	return GetStringTextLen(GetItemName(gPopupItem));
}

static void PoprDisp_ItemName(struct Text *text, const struct PopupInstruction *inst)
{
	Text_DrawString(text, GetItemName(gPopupItem));
}

static int PoprGetLen_ItemStrCap(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	return GetStringTextLen(GetItemNameWithArticle(gPopupItem, true));
}

static void PoprDisp_ItemStrCap(struct Text *text, const struct PopupInstruction *inst)
{
	Text_DrawString(text, GetItemNameWithArticle(gPopupItem, true));
}

static int PoprGetLen_ItemStr(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	return GetStringTextLen(GetItemNameWithArticle(gPopupItem, false));
}

static void PoprDisp_ItemStr(struct Text *text, const struct PopupInstruction *inst)
{
	Text_DrawString(text, GetItemNameWithArticle(gPopupItem, false));
}

static int PoprGetLen_UnitName(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	Assert(IS_EWRAM_PTR(gpPopupUnit));

	return GetStringTextLen(GetStringFromIndex(UNIT_NAME_ID(gpPopupUnit)));
}

static void PoprDisp_UnitName(struct Text *text, const struct PopupInstruction *inst)
{
	Assert(IS_EWRAM_PTR(gpPopupUnit));

	Text_DrawString(text, GetStringFromIndex(UNIT_NAME_ID(gpPopupUnit)));
}

static int PoprGetLen_Msg(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	return GetStringTextLen(GetStringFromIndex(inst->data));
}

static void PoprDisp_Msg(struct Text *text, const struct PopupInstruction *inst)
{
	Text_DrawString(text, GetStringFromIndex(inst->data));
}

static int PoprGetLen_Str(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	return GetStringTextLen((char *)inst->data);
}

static void PoprDisp_Str(struct Text *text, const struct PopupInstruction *inst)
{
	Text_DrawString(text, (char *)inst->data);
}

static int PoprGetLen_Color(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	return 0;
}

static void PoprDisp_Color(struct Text *text, const struct PopupInstruction *inst)
{
	Text_SetColor(text, inst->data);
}

static int PoprGetLen_ItemIcon(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	proc->iconX = proc->xGfxSize;
	proc->iconId = GetItemIconId(gPopupItem);
	LoadIconPalette(0, proc->iconPalId);
	return 0x10;
}

static void PoprDisp_ItemIcon(struct Text *text, const struct PopupInstruction *inst)
{
	Text_Skip(text, 0x10);
}

static int PoprGetLen_WtypeIcon(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	proc->iconX = proc->xGfxSize;
	proc->iconId = WTYPE_ICON(gPopupItem);
	LoadIconPalette(1, proc->iconPalId);
	return 0x10;
}

static void PoprDisp_WtypeIcon(struct Text *text, const struct PopupInstruction *inst)
{
	Text_Skip(text, 0x10);
}

static int PoprGetLen_Number(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	char str[0x10];

	return NumberToStringAscii(gPopupNumber, str) * 8;
}

static void PoprDisp_Number(struct Text *text, const struct PopupInstruction *inst)
{
	char str[0x10];

	NumberToStringAscii(gPopupNumber, str);
	Text_DrawString(text, str);
}

static int PoprGetLen_Sound(struct PopupProc *proc, const struct PopupInstruction *inst)
{
	proc->soundId = inst->data;
	return 0;
}

static void PoprDisp_Sound(struct Text *text, const struct PopupInstruction *inst) {}

struct PopupComponent const gPopupComponents[CHAX_POPUP_OP_ALLOC_MAX] = {
	[POPUP_OP_SPACE] =        { PoprGetLen_Space,      PoprDisp_Space },
	[POPUP_OP_ITEM_NAME] =    { PoprGetLen_ItemName,   PoprDisp_ItemName },
	[POPUP_OP_ITEM_STR_CAP] = { PoprGetLen_ItemStrCap, PoprDisp_ItemStrCap },
	[POPUP_OP_ITEM_STR] =     { PoprGetLen_ItemStr,    PoprDisp_ItemStr },
	[POPUP_OP_UNIT_NAME] =    { PoprGetLen_UnitName,   PoprDisp_UnitName },
	[POPUP_OP_MSG] =          { PoprGetLen_Msg,        PoprDisp_Msg },
	[POPUP_OP_STR] =          { PoprGetLen_Str,        PoprDisp_Str },
	[POPUP_OP_COLOR] =        { PoprGetLen_Color,      PoprDisp_Color },
	[POPUP_OP_ITEM_ICON] =    { PoprGetLen_ItemIcon,   PoprDisp_ItemIcon },
	[POPUP_OP_WTYPE_ICON] =   { PoprGetLen_WtypeIcon,  PoprDisp_WtypeIcon },
	[POPUP_OP_NUM] =          { PoprGetLen_Number,     PoprDisp_Number },
	[POPUP_OP_SOUND] =        { PoprGetLen_Sound,      PoprDisp_Sound },
};
