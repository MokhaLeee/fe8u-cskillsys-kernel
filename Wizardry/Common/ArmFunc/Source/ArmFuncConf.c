#include "common-chax.h"
#include "arm-func.h"

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
	{
		.src_start = _ARM_MapTask_CopyStart,
		.src_end   = _ARM_MapTask_CopyEnd,
		.dst_start = ARM_MapTask,
		.dst_end   = ARM_MapTaskEnd,
	},
	{}
};
