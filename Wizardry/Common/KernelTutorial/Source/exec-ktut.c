#include "common-chax.h"
#include "kernel-tutorial.h"

/**
 * 1. CheckForWaitEvents
 * 2. RunWaitEvents
 */

const EventScr EventScr_KTutorial[] = {
	TUTORIALTEXTBOXSTART
	SVAL(EVT_SLOT_B, -1)
	TEXTSHOW(-1)
	TEXTEND
	REMA

	NoFade
	ENDA
};

LYN_REPLACE_CHECK(CheckTutorialEvent);
bool CheckTutorialEvent(u8 type)
{
	/* Vanilla */
	if ((gPlaySt.tutorial_counter != 0) && (gPlaySt.tutorial_exec_type == type))
		return true;

#if CHAX
	if (type == TUTORIAL_EVT_TYPE_POSTACTION && CanExecKTutorial()) {
		int ktut_flag;

		ktut_flag = GetTriggerKtutorial();
		if (ktut_flag > 0)
			return true;
	}
#endif

	return false;
}

LYN_REPLACE_CHECK(RunTutorialEvent);
bool RunTutorialEvent(u8 type)
{
	int counter, ret = false;

	if ((gPlaySt.tutorial_counter != 0) && (gPlaySt.tutorial_exec_type == type)) {
		counter = gPlaySt.tutorial_counter;
		CallEvent(((u16 **)(GetChapterEventDataPointer(gPlaySt.chapterIndex)->tutorialEvents))[counter - 1], 1);
		gPlaySt.tutorial_counter = 0;
		gPlaySt.tutorial_exec_type = 0;
		ret = true;
	}

#if CHAX
	if (type == TUTORIAL_EVT_TYPE_POSTACTION && CanExecKTutorial()) {
		int ktut_flag;

		ktut_flag = GetTriggerKtutorial();
		if (ktut_flag > 0) {
			PutKtutHistory(ktut_flag);
			if (gpKTutorialMsg[ktut_flag] != 0) {
				gEventSlots[EVT_SLOT_2] = gpKTutorialMsg[ktut_flag];
				CallEvent((const u16 *)EventScr_KTutorial, EV_EXEC_CUTSCENE);
				ret = true;
			}
		}
	}
#endif

	return ret;
}
