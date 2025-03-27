#include <common-chax.h>
#include <banim-hack.h>
#include <battle-system.h>

STATIC_DECLAR void EfxHPBarLiveRe_DisplayAnimNumber(struct ProcEfxHpBar *proc)
{
	struct Anim *anim = proc->anim_this;
	int round = anim->nextRoundId - 1;
	struct BattleHit *hit = (prBattleHitArray + BattleHitArrayWidth * round);

	/**
	 * Only called when hitted with damage
	 */
	NewEfxAnimNumber(anim, -hit->hpChange);
}

/* External hook */
const struct ProcCmd ProcScr_efxHPBarLiveRework[] = {
	PROC_NAME("efxHPBarLive"),
	PROC_YIELD,
	PROC_CALL(EfxHPBarLiveRe_DisplayAnimNumber),
	PROC_REPEAT(EfxHPBarLiveMain),
	PROC_REPEAT(EfxHpBar_MoveCameraOnEnd),
	PROC_REPEAT(EfxHpBar_WaitCameraMove),
	PROC_END
};
