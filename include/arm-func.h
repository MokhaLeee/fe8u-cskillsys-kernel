#pragma once

#include "common-chax.h"

struct ArmFunConfig {
	u8 *dst_start, *dst_end;
	const u8 *src_start, *src_end;
};

extern struct ArmFunConfig const gKernelArmFunConfig[];
extern struct ArmFunConfig const *const gpKernelArmFunConfig;

extern const u8 _ARM_SkillTester_CopyStart[], _ARM_SkillTester_CopyEnd[];
extern const u8 _ARM_UnitList_CopyStart[], _ARM_UnitList_CopyEnd[];
extern const u8 _ARM_SkillList_CopyStart[], _ARM_SkillList_CopyEnd[];
extern const u8 _ARM_MapFloodCore_CopyStart[], _ARM_MapFloodCore_CopyEnd[];
extern const u8 _ARM_MapTask_CopyStart[], _ARM_MapTask_CopyEnd[];

extern u8 ARM_SkillTester[], ARM_SkillTesterEnd[];
extern u8 ARM_UnitList[], ARM_UnitListEnd[];
extern u8 ARM_SkillList[], ARM_SkillListEnd[];
extern u8 ARM_MapFloodCoreRe[], ARM_MapFloodCoreReEnd[];
extern u8 ARM_MapTask[], ARM_MapTaskEnd[];
