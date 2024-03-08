#include "global.h"
#include "hardware.h"
#include "icon.h"

const u8 * IconGetterVanilla(const u8 index)
{
    return item_icon_tiles + (index * CHR_SIZE * 4);
}
