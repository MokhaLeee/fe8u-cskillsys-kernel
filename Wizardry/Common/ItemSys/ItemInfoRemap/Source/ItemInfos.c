#include "common-chax.h"
#include "item-sys.h"

/* LynJump */
char * GetItemNameWithArticle(int item, s8 capitalize)
{
    return GetItemName(item);
}

/* LynJump */
char * GetItemName(int item)
{
    char * result;

#if CHAX
    if (IsDuraItem(item))
        return GetDuraItemName(item);
#endif

    result = GetStringFromIndex(GetItemData(ITEM_INDEX(item))->nameTextId);
    result = StrInsertTact();
    return result;
}

/* LynJump */
int GetItemDescId(int item)
{
#if CHAX
    if (IsDuraItem(item))
        return GetDuraItemDescId(item);
#endif

    return GetItemData(ITEM_INDEX(item))->descTextId;
}

/* LynJump */
int GetItemUseDescId(int item)
{
#if CHAX
    if (IsDuraItem(item))
        return GetDuraItemUseDescId(item);
#endif

    return GetItemData(ITEM_INDEX(item))->useDescTextId;
}

/* LynJump */
int GetItemUses(int item)
{
    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return 0xFF;

#if CHAX
    if (IsDuraItem(item))
        return 1;
#endif

    return ITEM_USES(item);
}

/* LynJump */
int GetItemMaxUses(int item)
{
    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return 0xFF;

#if CHAX
    if (IsDuraItem(item))
        return 1;
#endif

    return GetItemData(ITEM_INDEX(item))->maxUses;
}

/* LynJump */
int GetItemIconId(int item)
{
    if (!item)
        return -1;

#if CHAX
    if (IsDuraItem(item))
        return GetDuraItemIconId(item);
#endif

    return GetItemData(ITEM_INDEX(item))->iconId;
}

/* LynJump */
int GetItemCost(int item)
{
    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return GetItemData(ITEM_INDEX(item))->costPerUse;
    else
        return GetItemData(ITEM_INDEX(item))->costPerUse * GetItemUses(item);
}

/* LynJump */
int GetItemMaxCost(int item)
{
    return GetItemData(ITEM_INDEX(item))->costPerUse * GetItemMaxUses(item);
}

/* LynJump */
void DrawItemMenuLine(struct Text * text, int item, s8 isUsable, u16 * mapOut)
{
    Text_SetParams(text, 0, (isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
    Text_DrawString(text, GetItemName(item));

    PutText(text, mapOut + 2);

    if (!IsDuraItem(item))
    {
        PutNumberOrBlank(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
    }
    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

/* LynJump */
void DrawItemMenuLineLong(struct Text * text, int item, s8 isUsable, u16 * mapOut)
{
    Text_SetParams(text, 0, (isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY));
    Text_DrawString(text, GetItemName(item));

    PutText(text, mapOut + 2);

    if (!IsDuraItem(item))
    {
        PutNumberOrBlank(mapOut + 10, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemUses(item));
        PutNumberOrBlank(mapOut + 13, isUsable ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY, GetItemMaxUses(item));
        PutSpecialChar(mapOut + 11, isUsable ? TEXT_COLOR_SYSTEM_WHITE : TEXT_COLOR_SYSTEM_GRAY, TEXT_SPECIAL_SLASH);
    }
    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

/* LynJump */
void DrawItemMenuLineNoColor(struct Text * text, int item, u16 * mapOut)
{
    Text_SetCursor(text, 0);
    Text_DrawString(text, GetItemName(item));

    PutText(text, mapOut + 2);

    if (!IsDuraItem(item))
    {
        PutNumberOrBlank(mapOut + 11, Text_GetColor(text), GetItemUses(item));
    }
    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

/* LynJump */
void DrawItemStatScreenLine(struct Text * text, int item, int nameColor, u16 * mapOut)
{
    int color;

    ClearText(text);

    color = nameColor;
    Text_SetColor(text, color);

    Text_DrawString(text, GetItemName(item));

    if (!IsDuraItem(item))
    {
        color = (nameColor == TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_GRAY : TEXT_COLOR_SYSTEM_WHITE;
        PutSpecialChar(mapOut + 12, color, TEXT_SPECIAL_SLASH);

        color = (nameColor != TEXT_COLOR_SYSTEM_GRAY) ? TEXT_COLOR_SYSTEM_BLUE : TEXT_COLOR_SYSTEM_GRAY;
        PutNumberOrBlank(mapOut + 11, color, GetItemUses(item));
        PutNumberOrBlank(mapOut + 14, color, GetItemMaxUses(item));
    }
    PutText(text, mapOut + 2);

    DrawIcon(mapOut, GetItemIconId(item), 0x4000);
}

/* LynJump */
u16 GetItemAfterUse(int item)
{
    if (GetItemAttributes(item) & IA_UNBREAKABLE)
        return item;

#if CHAX
    if (IsDuraItem(item))
        return 0;
#endif

    item -= (1 << 8);
    if (item < (1 << 8))
        return 0;

    return item;
}
