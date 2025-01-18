#include "common-chax.h"
#include "combat-art.h"

extern struct CombatArtStatus gCombatArtStatus;

/* Judge is combat-art effective */
int GetCombatArtInForce(struct Unit *unit)
{
	if (gCombatArtStatus.uid != unit->index)
		return 0;

	if (!COMBART_VALID(gCombatArtStatus.cid))
		return 0;

	return gCombatArtStatus.cid;
}

void RegisterCombatArtStatus(struct Unit *unit, u8 cid)
{
	gCombatArtStatus.uid = unit->index;
	gCombatArtStatus.cid = cid;
}

void RegisterCombatArtTargetPos(u8 x, u8 y)
{
	gCombatArtStatus.x = x;
	gCombatArtStatus.y = y;
}

void ResetCombatArtStatus(void)
{
	CpuFill16(0, &gCombatArtStatus, sizeof(gCombatArtStatus));
}

void SaveCombatArtStatus(u8 *dst, const u32 size)
{
	if (size < sizeof(gCombatArtStatus)) {
		Errorf("ENOMEM: %#x", size);
		hang();
	}

	WriteAndVerifySramFast(
		&gCombatArtStatus,
		dst,
		sizeof(gCombatArtStatus));
}

void LoadCombatArtStatus(u8 *src, const u32 size)
{
	if (size < sizeof(gCombatArtStatus)) {
		Errorf("ENOMEM: %#x", size);
		hang();
	}

	ReadSramFast(
		src,
		&gCombatArtStatus,
		sizeof(gCombatArtStatus));
}
