#include "common-chax.h"
#include "arm-func.h"

extern const u8 _ARM_SkillTester_CopyStart[], _ARM_SkillTester_CopyEnd[];
extern u8 ARM_SkillTester[], ARM_SkillTesterEnd[];

extern const u8 _ARM_UnitList_CopyStart[], _ARM_UnitList_CopyEnd[];
extern u8 ARM_UnitList[], ARM_UnitListEnd[];

struct ArmFunConfig const gKernelArmFunConfig[] = {
    {
        .src_start = _ARM_SkillTester_CopyStart,
        .src_end   = _ARM_SkillTester_CopyEnd,
        .dst_start = ARM_SkillTester,
        .dst_end   = ARM_SkillTesterEnd,
    },
    {
        .src_start = _ARM_UnitList_CopyStart,
        .src_end   = _ARM_UnitList_CopyEnd,
        .dst_start = ARM_UnitList,
        .dst_end   = ARM_UnitListEnd,
    },
    {}
};
