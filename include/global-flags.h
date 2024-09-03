#pragma once

#include "debug-kit.h"
#include "gbafe.h"

extern struct GlobalFlags {
    u32 skill_activated_transform : 1;
} MapGlobalFlag;

static inline struct GlobalFlags GetGlobalFlags(struct Unit * unit)
{
    return MapGlobalFlag;
}