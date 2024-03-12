#pragma once

#define SPELL_ASSOC_DATA(_item, _count, _efx, _pcmd, _stat, _facing, _color)   \
{                                                                               \
    .item           = _item,                                                    \
    .count          = _count,                                                   \
    .efx            = _efx,                                                    \
    .pcmd_manim     = _pcmd,                                                    \
    .stat           = _stat,                                                    \
    .facing         = _facing,                                                  \
    .flash_color    = _color,                                                   \
}

#define SPELL_ASSOC_DATA_WPN(_item, _efx)  \
    SPELL_ASSOC_DATA(_item, 2, _efx, NULL, true, MA_FACING_OPPONENT, SPELL_ASSOC_MCOLOR_NORMAL)

#define SPELL_ASSOC_DATA_WPN_MAGIC(_item, _efx, flash_color)   \
    SPELL_ASSOC_DATA(_item, 2, _efx, NULL, true, MA_FACING_OPPONENT, flash_color)

#define SPELL_ASSOC_DATA_WPN_DEFAULT(_item) SPELL_ASSOC_DATA_WPN(_item, -1)
#define SPELL_ASSOC_DATA_ITEM(_item) SPELL_ASSOC_DATA_WPN(_item, 0x32)
