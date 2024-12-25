#include "common-chax.h"
#include "jester_headers/custom-structs.h"

#ifdef CONFIG_BEXP

extern u16 sBEXP[CONFIG_BEXP];

void SaveBEXP(void* target, u16 size) {
    Assert(size >= sizeof(sBEXP));
    WriteAndVerifySramFast(sBEXP, target, size);
}

void LoadBEXP(void* source, u16 size) {
    Assert(size >= sizeof(sBEXP));
    ReadSramFast(source, sBEXP, size);
}

#endif