#pragma once

#include "common-chax.h"

struct MovMapFillStateRe {

    u8 _pad_vanilla_[0xC]; // gMovMapFillState

    /* 0C */ u8 flag;
    /* 0D */ u8 _pad_[3];
};

enum MovMapFillStateRe_flags {
    FMOVSTRE_PASS = 1 << 0,
};

// #define MovMapFillStateRe ((struct MovMapFillStateRe *)&gMovMapFillState)
#define MovMapFillStateRe ((struct MovMapFillStateRe *)0x03004E60)
