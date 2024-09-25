#include "common-chax.h"

extern u8 const * const pr_item_icon_tiles;
extern u8 const * const * const gpAffinIcons;
extern u8 const * const * const gpMountIcons;
extern u8 const * const * const gpWTypeIcons;

const u8 *IconGetterVanilla(const u8 index)
{
    return pr_item_icon_tiles + (index * CHR_SIZE * 4);
}

const u8 *GetAffinIcon(const u8 index)
{
    return gpAffinIcons[index];
}

const u8 *GetMountIcon(const u8 index)
{
    return gpMountIcons[index];
}

const u8 *GetWTypeIcon(const u8 index)
{
    return gpWTypeIcons[index];
}
