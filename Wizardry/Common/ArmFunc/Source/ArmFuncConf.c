#include "common-chax.h"
#include "arm-func.h"

extern const u8 _ARM_SkillTester_CopyStart[], _ARM_SkillTester_CopyEnd[];
extern u8 ARM_SkillTester[], ARM_SkillTesterEnd[];

extern const u8 _ARM_UnitList_CopyStart[], _ARM_UnitList_CopyEnd[];
extern u8 ARM_UnitList[], ARM_UnitListEnd[];

extern const u8 _ARM_SkillList_CopyStart[], _ARM_SkillList_CopyEnd[];
extern u8 ARM_SkillList[], ARM_SkillListEnd[];

extern const u8 _ARM_MapFloodCore_CopyStart[], _ARM_MapFloodCore_CopyEnd[];
extern u8 ARM_MapFloodCoreRe[], ARM_MapFloodCoreReEnd[];

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
    {
        .src_start = _ARM_SkillList_CopyStart,
        .src_end   = _ARM_SkillList_CopyEnd,
        .dst_start = ARM_SkillList,
        .dst_end   = ARM_SkillListEnd,
    },
    {
        .src_start = _ARM_MapFloodCore_CopyStart,
        .src_end   = _ARM_MapFloodCore_CopyEnd,
        .dst_start = ARM_MapFloodCoreRe,
        .dst_end   = ARM_MapFloodCoreReEnd,
    },
    {}
};
