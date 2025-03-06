#include "common-chax.h"
#include "battle-system.h"

/**
 * Fit to patch: DSFE STYLE AVOID
 */
extern const u8 DsStyleAvoidIdentifier[];

static bool check_array(const u8 *data, const u8 *ref_array, int size)
{
	int i;

	for (i = 0; i < size; i++)
		if (data[i] != ref_array[i])
			return false;

	return true;
}

STATIC_DECLAR bool CheckDsStyleAvoidDefault(void)
{
	static const u8 ref_array[] = {
		0x40, 0x00, 0x11, 0x1C, 0x57, 0x31, 0x09, 0x78,
		0x09, 0x06, 0x09, 0x16, 0x09, 0x18, 0x19, 0x20,
		0x10, 0x56,
	};

	return check_array(DsStyleAvoidIdentifier, ref_array, ARRAY_COUNT(ref_array));
}

STATIC_DECLAR bool CheckDsStyleAvoidFix1(void)
{
	static const u8 ref_array[] = {
		0x11, 0x1C, 0x57, 0x31, 0x09, 0x78, 0x09, 0x06,
		0x09, 0x16, 0x09, 0x18, 0x19, 0x20, 0x10, 0x56,
		0x40, 0x08,
	};

	return check_array(DsStyleAvoidIdentifier, ref_array, ARRAY_COUNT(ref_array));
}

STATIC_DECLAR bool CheckDsStyleAvoidFix2(void)
{
	static const u8 ref_array[] = {
		0x00, 0x00, 0x11, 0x1C, 0x57, 0x31, 0x09, 0x78,
		0x09, 0x06, 0x09, 0x16, 0x09, 0x18, 0x19, 0x20,
		0x10, 0x56,
	};

	return check_array(DsStyleAvoidIdentifier, ref_array, ARRAY_COUNT(ref_array));
}

int GetBaseAvoid_WithDsStylePatch(struct BattleUnit *bu)
{
	if (CheckDsStyleAvoidDefault()) {
		// default: speed * 2 + luck
		return (bu->battleSpeed * 2 + bu->unit.lck);
	}

	if (CheckDsStyleAvoidFix1()) {
		// fix1: speed + luck / 2
		return (bu->battleSpeed + (bu->unit.lck / 2));
	}

	if (CheckDsStyleAvoidFix2()) {
		// fix2: speed + luck
		return (bu->battleSpeed + bu->unit.lck);
	}

	// default: speed * 2 + luck
	return (bu->battleSpeed * 2 + bu->unit.lck);
}
