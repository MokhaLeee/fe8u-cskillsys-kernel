#include <common-chax.h>

#include "PortraitHeight.h"

/**
 * For demo build, `HighPortraitFidLut` is defined directly in Portrait dir
 */
// #ifndef CONFIG_DEMO_BUILD

/**
 * But the CI build cannot process PortraitFormatter with no idea :\
 */
#if 1

const u8 HighPortraitFidLut[] = {
	FID_LUTE,
	FID_AMELIA,
	FID_EWAN,
	FID_DOZLA,
	FID_MYRRH,
	0
};
#endif // DEMO_BUILD
