#include "global.h"
#include "bmsave.h"
#include "agb_sram.h"
#include "hardware.h"

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

/* LynJump */
void EraseInvalidSaveData(void)
{
    int ret;
    ret = VerifySkillSysSRAM();
    if (ret)
    {
        Error("SRAM verify failed, start to reset...");
        WipeSkillSysSRAM();

        /* Yeah, restart the game! */
        // SoftReset(0);
    }
}
