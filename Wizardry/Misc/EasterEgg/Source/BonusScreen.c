#include <common-chax.h>

#include "Bonus.h"

static void BonusScreen_VBlank(void)
{
	INTR_CHECK = INTR_FLAG_VBLANK;

	FlushLCDControl();
	FlushBackgrounds();
	FlushTiles();
	m4aSoundVSync();
	m4aSoundMain();
}

noreturn void StartKernelBonusScreen(void)
{
	int i = 0;

	SetInterrupt_LCDVBlank(BonusScreen_VBlank);

	SetupDebugFontForBG(BG_2, 0);
	BG_SetPosition(BG_2, 0, 0);

	SetDispEnable(0, 0, 1, 0, 0);
	SetWinEnable(0, 0, 0);
	SetBlendNone();

	while (1) {
		UpdateKeyStatus(gKeyStatusPtr);
		sub_8003ABC(gKeyStatusPtr->heldKeys, gKeyStatusPtr->newKeys);
		VBlankIntrWait();
		SoftResetIfKeyComboPressed();

		if (++i > 128) {
			if (gKeyStatusPtr->heldKeys & (L_BUTTON | R_BUTTON | A_BUTTON | B_BUTTON))
				SoftReset(0);

			if (gKeyStatusPtr->heldKeys & (A_BUTTON | B_BUTTON | SELECT_BUTTON | START_BUTTON))
				SoftReset(0);
		}
	}
}
