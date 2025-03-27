#include "common-chax.h"
#include "save-data.h"

#define SKILL_SYS_MAGIC "FESKILL\0"
#define SKILLSYS_MAGIC_LEN 8

extern u16 gSkillSysMagicOffset;

STATIC_DECLAR int VerifySkillSysSRAM(void)
{
	int i;
	char buf[] = SKILL_SYS_MAGIC;

	ReadSramFast(SramOffsetToAddr(gSkillSysMagicOffset), gGenericBuffer, SKILLSYS_MAGIC_LEN);

	for (i = 0; i < SKILLSYS_MAGIC_LEN; i++)
		if (gGenericBuffer[i] != buf[i])
			return -1;

	return 0;
}

STATIC_DECLAR void WipeSkillSysSRAM(void)
{
	/* Wipe SRAM */
	InitGlobalSaveInfodata();

	WriteAndVerifySramFast(
		SKILL_SYS_MAGIC,
		SramOffsetToAddr(gSkillSysMagicOffset),
		SKILLSYS_MAGIC_LEN);
}

LYN_REPLACE_CHECK(EraseInvalidSaveData);
void EraseInvalidSaveData(void)
{
	int ret = VerifySkillSysSRAM();

	if (ret) {
		Error("SRAM verify failed, start to reset...");
		WipeSkillSysSRAM();

		/* Yeah, restart the game! */
		// SoftReset(0);
	}

	/* Misc judgement */
	Assert(sizeof(struct EmsPackedSavUnit) == SIZE_OF_SAV_UNIT_PACK);
	Assert(sizeof(struct EmsPackedSusUnit) == SIZE_OF_SUS_UNIT_PACK);
}

const u32 size_EmsPackedSusUnit = sizeof(struct EmsPackedSusUnit);
const u32 size_EmsPackedSavUnit = sizeof(struct EmsPackedSavUnit);
