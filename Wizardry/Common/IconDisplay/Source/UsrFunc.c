#include "global.h"
#include "hardware.h"
#include "icon.h"

extern u8 const * const pr_item_icon_tiles;

const u8 * IconGetterVanilla(const u8 index)
{
    return pr_item_icon_tiles + (index * CHR_SIZE * 4);
}
