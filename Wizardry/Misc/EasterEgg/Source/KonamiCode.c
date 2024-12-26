#include <common-chax.h>

#include "Bonus.h"

// Add a secret Easter egg to the game

#define KERNEL_BONUS_NOTE "\
      Congratulations!\n\
\n\
You found the BONUS\n\
from kernel developers!\n\
Hope you can enjoy this game!\n\
\n\
          Love\n\
          Mokha.Li\n\
\n\
Repo:\n\
https://github.com/FireEmblem\n\
Universe/fe8u-cskillsys\n\
\n\
Contact:\n\
https://github.com/MokhaLeee\n\
\n\
Now you can restart the\n\
game and continue playing!\
"

static const u8 konami_codes[] = {
	DPAD_UP,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	DPAD_LEFT,
	DPAD_RIGHT,
	B_BUTTON,
	A_BUTTON,
};

extern u8 gKonamiComboStep;

/**
 * Replace vanilla Title_IDLE()
 * External hook.
 */
void KernelTitle_IDLE(struct TitleScreenProc *proc)
{
	proc->timer_idle++;
	proc->timer++;
	proc->timer = proc->timer & 0x3F;

	if (proc->timer_idle == 1)
		gKonamiComboStep = 0;

	if (gKeyStatusPtr->newKeys == 0)
		return;

	if (gKeyStatusPtr->newKeys == konami_codes[gKonamiComboStep]) {
		Printf("step=%d, getc:0x%X!", gKonamiComboStep, gKeyStatusPtr->newKeys);

		gKonamiComboStep++;

		if (gKonamiComboStep >= ARRAY_COUNT(konami_codes)) {
			Print("KONAMI BONUS!\n");
			GiveKernelBonus();
			PrintStringToDBG(KERNEL_BONUS_NOTE);
			StartKernelBonusScreen();

			Error("You should now get here");
			hang();
		}

		return;
	}

	if (gKonamiComboStep > 0) {
		Print("Reset KONAMI code");
		gKonamiComboStep = 0;
	}

	/**
	 * Vanilla
	 */
	if (gKeyStatusPtr->newKeys & (A_BUTTON | START_BUTTON)) {
		PlaySoundEffect(0x6D);
		SetNextGameActionId(GAME_ACTION_EVENT_RETURN);
		Proc_Break(proc);
	}
}
