#include "common-chax.h"
#include "kernel-lib.h"
#include "kernel-tutorial.h"

/* In GameInit */
static void ClearTrigKtutFlags(void)
{
	memset(sKTutorialBits, 0, sizeof(sKTutorialBits));
}

void ClearKtutorialFlags(void)
{
	ClearTrigKtutFlags();
	memset(sKTutorialBitsHistory, 0, sizeof(sKTutorialBitsHistory));
}

/* In SaveData */
void EMS_SaveKTutorialFlags(u8 *dst, const u32 size)
{
	Assert(size >= (sizeof(sKTutorialBitsHistory) + sizeof(sKTutorialBits)));

	WriteAndVerifySramFast(
		sKTutorialBits,
		dst,
		sizeof(sKTutorialBits));

	WriteAndVerifySramFast(
		sKTutorialBitsHistory,
		dst + sizeof(sKTutorialBits),
		sizeof(sKTutorialBitsHistory));
}

/* In SaveData */
void EMS_LoadKTutorialFlags(u8 *src, const u32 size)
{
	Assert(size >= (sizeof(sKTutorialBitsHistory) + sizeof(sKTutorialBits)));

	ReadSramFast(
		src,
		sKTutorialBits,
		sizeof(sKTutorialBits));

	ReadSramFast(
		src + sizeof(sKTutorialBits),
		sKTutorialBitsHistory,
		sizeof(sKTutorialBitsHistory));
}

/* API */
bool CanExecKTutorial(void)
{
	if (gpKernelDesigerConfig->kernel_tutorial_level == KTUT_LEVEL_NEVER)
		return false;

	if (gpKernelDesigerConfig->kernel_tutorial_level == KTUT_LEVEL_MIDDLE) {
		/* Only in tutorial mode */
		if (!TUTORIAL_MODE())
			return false;
	}

	if (gpKernelDesigerConfig->kernel_tutorial_level == KTUT_LEVEL_HIGH)
		return true;

	return true;
}

void TriggerKtutorial(int flag)
{
	_BIT_SET(sKTutorialBits, flag);
}

int GetTriggerKtutorial(void)
{
	int i;

	for (i = 1; i < KTUTORIAL_MAX; i++)
		if (_BIT_CHK(sKTutorialBits, i) && !_BIT_CHK(sKTutorialBitsHistory, i))
			return i;

	return -1;
}

bool CheckKtutFlagTriggered(int flag)
{
	return _BIT_CHK(sKTutorialBitsHistory, flag);
}

void PutKtutHistory(int flag)
{
	/**
	 * If we triggered more than one k-tut,
	 * We can just trigger one tutorial, but other will be negleted.
	 * Here we clear all trigger flags.
	 */
	ClearTrigKtutFlags();

	if (flag < KTUTORIAL_MAX)
		_BIT_SET(sKTutorialBitsHistory, flag);
}
