#include "common-chax.h"
#include "class-types.h"

typedef bool (*ClassTypeFunc_t)(u8 jid);

bool CheckSameClassType(u8 jid1, u8 jid2)
{
	static const u8 jdg_list[] = {
		2, // 00
		0, // 01
		0, // 10
		1, // 11
	};

	static const ClassTypeFunc_t jdg_funcs[] = {
		CheckClassFlier,
		CheckClassCavalry,
		CheckClassArmor,
		CheckClassDragon,
		CheckClassBeast,
		NULL
	};

	const ClassTypeFunc_t *it;

	for (it = jdg_funcs; *it; it++) {
		switch (jdg_list[((*it)(jid1) << 1) | (*it)(jid2)]) {
		case 1:
			return true;

		case 0:
			return false;

		case 2:
		default:
			break;
		}
	}

	/* Both of which are normal jobs */
	return true;
}

bool CheckClassFlier(u8 jid)
{
	const u8 *it;

	for (it = gpKernelClassList_Flier; *it != CLASS_NONE; it++)
		if (*it == jid)
			return true;

	return false;
}

bool CheckClassCavalry(u8 jid)
{
	const u8 *it;

	for (it = gpKernelClassList_Cavalry; *it != CLASS_NONE; it++)
		if (*it == jid)
			return true;

	return false;
}

bool CheckClassArmor(u8 jid)
{
	const u8 *it;

	for (it = gpKernelClassList_Armor; *it != CLASS_NONE; it++)
		if (*it == jid)
			return true;

	return false;
}

bool CheckClassDragon(u8 jid)
{
	const u8 *it;

	for (it = gpKernelClassList_Dragon; *it != CLASS_NONE; it++)
		if (*it == jid)
			return true;

	return false;
}

bool CheckClassBeast(u8 jid)
{
	const u8 *it;

	for (it = gpKernelClassList_Beast; *it != CLASS_NONE; it++)
		if (*it == jid)
			return true;

	return false;
}
