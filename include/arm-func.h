#pragma once

#include "common-chax.h"

struct ArmFunConfig {
    u8 * dst_start, * dst_end;
    const u8 * src_start, * src_end;
};

extern struct ArmFunConfig const gKernelArmFunConfig[];
extern struct ArmFunConfig const * const gpKernelArmFunConfig;
