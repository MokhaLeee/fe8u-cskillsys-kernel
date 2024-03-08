#include "global.h"
#include "rng.h"
#include "agb_sram.h"

#include "common-chax.h"
#include "rn.h"

extern u16 gRNSeeds[3];

extern u16 sRandSeedsC[3], sRandBackup[3];

/* Game init hook */
void InitRandC(void)
{
    int i;
    for (i = 0 ; i < 3; i++)
        sRandSeedsC[i] = NextRN();
}

STATIC_DECLAR void PrepareRandC(int amount)
{
    int i;
    for (i = 0; i < 3; i++)
        sRandBackup[i] = gRNSeeds[i];

    for (i = 0; i < 3; i++)
        gRNSeeds[i] = sRandSeedsC[i];

    for (i = 0; i < amount; i++)
        (void)NextRN();
}

STATIC_DECLAR void RestoreRnState(void)
{
    int i;
    for (i = 0; i < 3; i++)
        gRNSeeds[i] = sRandBackup[i];
}

STATIC_DECLAR int RandNextC(int amount)
{
    u16 ret;
    PrepareRandC(amount);
    ret = NextRN();
    RestoreRnState();
    return ret;
}

STATIC_DECLAR int RandNextC_100(int amount)
{
    return Div(RandNextC(amount) * 100, 0x10000);
}

bool Roll1RandC(int threshold, int amount)
{
    return threshold > RandNextC_100(amount);
}

bool Roll2RandC(int threshold, int amount)
{
    int average, rn1, rn2;
    PrepareRandC(amount);

    rn1 = Div(NextRN() * 100, 0x10000);
    rn2 = Div(NextRN() * 100, 0x10000);
    average = (rn1 + rn2) / 2;

    RestoreRnState();
    return threshold > average;
}

void SaveRandC(u8 * dst, const u32 size)
{
    if (size < sizeof(sRandSeedsC))
    {
        Errorf("ENOMEM: %d", size);
        return;
    }

    WriteAndVerifySramFast(
        sRandSeedsC,
        dst,
        sizeof(sRandSeedsC));
}

void LoadRandC(u8 * src, const u32 size)
{
    if (size < sizeof(sRandSeedsC))
    {
        Errorf("ENOMEM: %#x", size);
        return;
    }

    ReadSramFast(
        src,
        sRandSeedsC,
        sizeof(sRandSeedsC));
}
