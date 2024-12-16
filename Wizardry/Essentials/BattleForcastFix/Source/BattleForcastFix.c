#include <common-chax.h>

/**
 * Called in target-selection on cancel,
 * Quite a dangrous bug on vanilla.
 */
LYN_REPLACE_CHECK(sub_8022E38);
void sub_8022E38(void)
{
#if CHAX
	ResetText();
#endif

	EnsureCameraOntoPosition(NULL, gActiveUnit->xPos, gActiveUnit->yPos);
}
