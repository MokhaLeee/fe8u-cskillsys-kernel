#include "common-chax.h"
#include "weapon-range.h"
#include "status-getter.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "constants/skills.h"

#include "ai-hack.h"

#define LOCAL_TRACE 0

STATIC_DECLAR bool AiTryTeleportationExt(void)
{
	struct Vec2 vec;

	if (GetUnitEquippedWeaponSlot(gActiveUnit) < 0)
		return false;

#if defined(SID_Teleportation) && (COMMON_SKILL_VALID(SID_Teleportation))
	if (!SkillTester(gActiveUnit, SID_Teleportation) || ((GetUnitCurrentHp(gActiveUnit) * 100) < (GetUnitMaxHp(gActiveUnit) * SKILL_EFF0(SID_Teleportation))))
#else
	if (1)
#endif
		return false;

	if (GetTeleportationRandomPosition(gActiveUnit, &vec) == true) {
		AiSetDecision(gActiveUnit->xPos, gActiveUnit->yPos, CONFIG_AI_ACTION_EXPA_Teleportation, 0, 0, vec.x, vec.y);
		return true;
	}
	return false;
}

/* External hook */
bool Ai2Decide_TryTeleportation(void)
{
	/**
	 * This function may just hook at AI-2 decided
	 */
	if (gAiScriptKind != AI_SCRIPT_AI2)
		return false;

	if (CheckBitUES(gActiveUnit, UES_BIT_TSZUKU_SKILL_USED))
		return false;

	switch (gActiveUnit->ai2) {
	case 0:
	case 1:
	case 2:
	case 9:
	case 10:
	case 11:
		break;

	default:
		return false;
	}

	if (AiTryTeleportationExt()) {
		LTRACEF("[uid=%x] Ai1 Teleportation to x=%d, y=%d",
					gActiveUnit->index & 0xFF, gAiDecision.xTarget, gAiDecision.yTarget);

		SetBitUES(gActiveUnit, UES_BIT_TSZUKU_SKILL_USED);
		return true;
	}
	return false;
}
