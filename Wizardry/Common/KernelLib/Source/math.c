#include "common-chax.h"

#define LOCAL_TRACE 0

u32 k_udiv(u32 a, u32 b)
{
	u32 ret = 0;
	u32 tmp_a = a;
	u64 tmp_b = b;

	if (b == 0)
		return 0;

	while (tmp_b <= a)
		tmp_b = tmp_b << 1;

	while (tmp_b > b) {
		tmp_b = tmp_b >> 1;
		ret = ret << 1;

		if (tmp_a >= tmp_b) {
			tmp_a = tmp_a - tmp_b;
			ret = ret | 1;
		}
	}

	LTRACEF("k_udiv: numerator=%d, denominator=%d, ret=%d", a, b, ret);
	return ret;
}

u32 k_umod(u32 a, u32 b)
{
	u32 ret = 0;
	u32 tmp_a = a;
	u64 tmp_b = b;

	if (b == 0)
		return 0;

	while (tmp_b <= a)
		tmp_b = tmp_b << 1;

	while (tmp_b > b) {
		tmp_b = tmp_b >> 1;

		if (tmp_a >= tmp_b)
			tmp_a = tmp_a - tmp_b;
	}

	ret = tmp_a;

	LTRACEF("k_umod: numerator=%d, denominator=%d, ret=%d", a, b, ret);
	return ret;
}
