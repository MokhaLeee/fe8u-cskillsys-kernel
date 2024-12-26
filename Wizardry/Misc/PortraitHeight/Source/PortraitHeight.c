#include <common-chax.h>

#include "PortraitHeight.h"

LYN_REPLACE_CHECK(ShouldFaceBeRaised);
bool ShouldFaceBeRaised(int fid)
{
	const u8 *it;

	for (it = gpHighPortraitFidLut; *it; it++)
		if (*it == fid)
			return true;

	return false;
}
