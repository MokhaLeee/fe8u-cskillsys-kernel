#include <common-chax.h>
#include <kernel-lib.h>

int WRankToWExp(int wrank)
{
	static const u8 wrank_to_exp_ref[] = {
		[WPN_LEVEL_0] = WPN_EXP_0,
		[WPN_LEVEL_E] = WPN_EXP_E,
		[WPN_LEVEL_D] = WPN_EXP_D,
		[WPN_LEVEL_C] = WPN_EXP_C,
		[WPN_LEVEL_B] = WPN_EXP_B,
		[WPN_LEVEL_A] = WPN_EXP_A,
		[WPN_LEVEL_S] = WPN_EXP_S,
	};

	switch (wrank) {
	case WPN_LEVEL_0 ... WPN_LEVEL_S:
		return wrank_to_exp_ref[wrank];

	default:
		return WPN_EXP_S;
	}
}
