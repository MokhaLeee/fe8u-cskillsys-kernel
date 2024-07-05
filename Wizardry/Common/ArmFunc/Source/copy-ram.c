#include "common-chax.h"
#include "arm-func.h"

void GameInit_CopyArmFuncs(void)
{
    const struct ArmFunConfig * it;
    for (it = gpKernelArmFunConfig; ; it++)
    {
        if (!it->dst_start || !it->dst_end || !it->src_start || !it->src_end)
            break;

        Assert((it->dst_end - it->dst_start) > (it->src_end - it->src_start));
        memcpy(it->dst_start, it->src_start, it->src_end - it->src_start);
    }
}
