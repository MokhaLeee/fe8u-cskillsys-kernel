#include "common-chax.h"

extern const u8 HighPortraitFidLut[];

LYN_REPLACE_CHECK(ShouldFaceBeRaised);
bool ShouldFaceBeRaised(int fid)
{
    const u8 * it;
    for (it = HighPortraitFidLut; *it; it++)
        if (*it == fid)
            return true;

    return false;
}
