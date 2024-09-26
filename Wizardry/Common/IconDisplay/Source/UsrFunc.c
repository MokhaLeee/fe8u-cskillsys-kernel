#include "common-chax.h"
#include "icon-rework.h"

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
