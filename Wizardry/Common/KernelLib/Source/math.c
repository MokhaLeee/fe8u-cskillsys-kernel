#include "common-chax.h"

u32 kdiv(u32 a, u32 b)
{
	u32 ret = 0;
	u32 tmp_a = a;
	u32 tmp_b = b;

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

	Printf("kdiv: numerator=%d, denominator=%d, ret=%d", a, b, ret);
	return ret;
}

u32 kmod(u32 a, u32 b)
{
	u32 ret = 0;
	u32 tmp_a = a;
	u32 tmp_b = b;

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

	Printf("kmod: numerator=%d, denominator=%d, ret=%d", a, b, ret);
	return ret;
}
