#include "common-chax.h"
#include "rn.h"

extern u16 sRandSeedsC[4];

STATIC_DECLAR u16 KernelRandRoll(u16 seeds[])
{
	u16 rn = (seeds[1] << 11) + (seeds[0] >> 5);

	seeds[2] *= 2;

	if (seeds[1] & 0x8000)
		seeds[2]++;

	rn ^= seeds[2];

	seeds[2] = seeds[1];
	seeds[1] = seeds[0];
	seeds[0] = rn;

	return rn;
}

/* Game init hook */
void InitRandC(void)
{
	int i;

	for (i = 0 ; i < 3; i++)
		sRandSeedsC[i] = NextRN();
}

STATIC_DECLAR int RandNextC(void)
{
	return KernelRandRoll(sRandSeedsC);
}

STATIC_DECLAR int RandNextC_100(void)
{
	return Div(RandNextC() * 100, 0x10000);
}

bool Roll1RandC(int threshold)
{
	return threshold > RandNextC_100();
}

bool Roll2RandC(int threshold)
{
	int average = (RandNextC_100() + RandNextC_100()) / 2;

	return (threshold > average);
}

void SaveRandC(u8 *dst, const u32 size)
{
	if (size < sizeof(sRandSeedsC)) {
		Errorf("ENOMEM: %d", size);
		hang();
	}

	sRandSeedsC[3] = Checksum16(sRandSeedsC, 6);

	WriteAndVerifySramFast(
		sRandSeedsC,
		dst,
		sizeof(sRandSeedsC));
}

void LoadRandC(u8 *src, const u32 size)
{
	if (size < sizeof(sRandSeedsC)) {
		Errorf("ENOMEM: %#x", size);
		hang();
	}

	ReadSramFast(
		src,
		sRandSeedsC,
		sizeof(sRandSeedsC));

	if (sRandSeedsC[3] != Checksum16(sRandSeedsC, 6))
		InitRandC();
}
