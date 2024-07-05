#include "common-chax.h"
#include "arm-func.h"

extern const u8 _ARM_SkillTester_CopyStart[], _ARM_SkillTester_CopyEnd[];
extern u8 ARM_SkillTester[], ARM_SkillTesterEnd[];

struct ArmFunConfig const gKernelArmFunConfig[] = {
    {
        .src_start = _ARM_SkillTester_CopyStart,
        .src_end   = _ARM_SkillTester_CopyEnd,
        .dst_start = ARM_SkillTester,
        .dst_end   = ARM_SkillTesterEnd,
    },
    {}
};
