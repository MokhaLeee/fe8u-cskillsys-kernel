#include "common-chax.h"
#include "skill-system.h"

extern const u8 JudgeSkillArmToCopy_Start[];
extern const u8 JudgeSkillArmToCopy_End[];

void GameInit_CopySkillTesterArm(void)
{
    Assert((JudgeSkillArmToCopy_End - JudgeSkillArmToCopy_Start) < ((void *)0x03004150 - (void *)_SkillTester_ARM));
    memcpy((void *)_SkillTester_ARM, JudgeSkillArmToCopy_Start, JudgeSkillArmToCopy_End - JudgeSkillArmToCopy_Start);
}
