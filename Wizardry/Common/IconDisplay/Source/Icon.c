#include "global.h"
#include "icon.h"
#include "hardware.h"

#include "common-chax.h"
#include "icon-rework.h"

const u8 * GetIconGfx(u32 icon)
{
    if (ICON_SHEET(icon) < ICON_SHEET_AMT)
        return IconGetters[ICON_SHEET(icon)](ICON_INDEX(icon));

    return NULL;
}

STATIC_DECLAR int GetIconUsedSlot(const u32 icon)
{
    int i;
    for (i = 0; i < MAX_SIMULTANEOUS_ICONS; i++)
    {
        if (icon == gIconReSts[i])
            return i + 1;
    }

    return 0;
}

STATIC_DECLAR int GetIconNewSlot(const u32 icon)
{
    int i;
    for (i = 0; i < MAX_SIMULTANEOUS_ICONS; i++)
    {
        if ((u16)-1 == gIconReSts[i])
        {
            gIconReSts[i] = icon;
            return i + 1;
        }
    }

    return 0;
}

/* LynJump */
void ResetIconGraphics(void)
{
    CpuFill32(-1, gIconReSts, sizeof(gIconReSts));
}

/* LynJump */
void ClearIconGfx(u32 icon)
{
    int i;
    for (i = 0; i < MAX_SIMULTANEOUS_ICONS; i++)
    {
        if (ICON_SHEET(icon) == ICON_SHEET(gIconReSts[i]) &&
            ICON_INDEX(icon) == ICON_INDEX(gIconReSts[i]))
        {
            gIconReSts[i] = -1;
            break;
        }
    }
}

/* LynJump */
u16 GetIconTileIndex(int icon)
{
    int slot, tile;
    const u8 * src;
    u8 * dst;

    slot = GetIconUsedSlot(icon);
    if (slot != 0)
        return GetIconGfxTileIndex(slot);

    /* Register new icon */
    slot = GetIconNewSlot(icon);
    tile = GetIconGfxTileIndex(slot);

    src = GetIconGfx(icon);
    dst = (void *)(BG_VRAM + tile * CHR_SIZE);

    if (src)
        RegisterDataMove(src, dst, CHR_SIZE * 4);
    else
        RegisterFillTile(0,   dst, CHR_SIZE * 4);

    return tile;
}


/* LynJump */
void LoadIconObjectGraphics(int icon, int chr)
{
    const u8 * src = GetIconGfx(icon);
    u8 * dst = OBJ_VRAM0 + CHR_SIZE * (chr & 0x3FF);

    if (!src)
    {
        RegisterFillTile(0, dst,         CHR_SIZE * 2);
        RegisterFillTile(0, dst + 0x400, CHR_SIZE * 2);
        return;
    }

    RegisterDataMove(src,                dst,         CHR_SIZE * 2);
    RegisterDataMove(src + CHR_SIZE * 2, dst + 0x400, CHR_SIZE * 2);
}
