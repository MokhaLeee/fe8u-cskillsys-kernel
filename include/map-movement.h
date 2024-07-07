#pragma once

#include "common-chax.h"

extern u32 KernelMoveMapFlags;

extern void (* _MapFloodCoreRe)(void);

enum MovMapFillStateRe_flags {
    FMOVSTRE_PASS = 1 << 0,
    FMOVSTRE_OBSTRUCT = 1 << 1,
};

extern u8 ** KernelExtMoveCostMap;
