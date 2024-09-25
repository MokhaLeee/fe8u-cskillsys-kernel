#include "common-chax.h"
#include "arm-func.h"

void GameInit_CopyArmFuncs(void)
{
	FORCE_DECLARE int i;
	const struct ArmFunConfig *it;

	for (i = 0, it = gpKernelArmFunConfig; ; i++, it++) {
		if (!it->dst_start || !it->dst_end || !it->src_start || !it->src_end)
			break;

		Printf("[%02d] src: %07X-%07X, dst: %07X-%07X",
					i, it->src_start, it->src_end, it->dst_start, it->dst_end);

		Assert((it->dst_end - it->dst_start) >= (it->src_end - it->src_start));
		memcpy(it->dst_start, it->src_start, it->src_end - it->src_start);
	}
}
