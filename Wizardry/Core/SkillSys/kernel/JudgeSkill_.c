#include "common-chax.h"
#include "skill-system.h"

extern const u8 _ARM_SkillTester_start[];
extern const u8 _ARM_SkillTester_end[];

void GameInit_CopySkillTesterArm(void)
{
    Assert((_ARM_SkillTester_end - _ARM_SkillTester_start) < ((void *)0x03004150 - (void *)ARM_SkillTester));
    memcpy((void *)ARM_SkillTester, _ARM_SkillTester_start, _ARM_SkillTester_end - _ARM_SkillTester_start);
}
