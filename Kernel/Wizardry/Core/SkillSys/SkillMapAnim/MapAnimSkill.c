#include "common-chax.h"
#include "battle-system.h"
#include "combat-art.h"
#include "skill-system.h"
#include "efx-skill.h"
#include "icon-rework.h"

#define LOCAL_TRACE 0

STATIC_DECLAR const struct ProcCmd ProcScr_SkillMapAnimDeamon[] = {
	PROC_SLEEP(20),
	PROC_END
};

bool MapAnimRoundAnim_DisplaySkillIcon(ProcPtr parent)
{
	int round, cid, sid_atk, sid_def;
	u32 actor_icon, target_icon;
	u32 left_icon, right_icon;
	struct ProcMapAnimSkillfx *procfx;
	struct MAInfoFrameProc *infoproc;

	infoproc = Proc_Find(ProcScr_MapBattleInfoBox);
	if (!infoproc)
		return false;

	round = GetBattleHitRound(gManimSt.pCurrentRound);

	cid = GetEfxCombatArt(round);
	sid_atk = GetActorEfxSkill(round);
	sid_def = GetTargetEfxSkill(round);

	LTRACEF("cid %#x, sid-atk %#x, sid-def %#x at round %d",
				cid, sid_atk, sid_def, round);

#if defined(CONFIG_USE_DEBUG) && defined(CONFIG_DEBUG_EFXSKILL)
	sid_atk = 1;
	sid_def = 2;
#endif

	actor_icon = 0;
	if (COMBART_VALID(cid))
		actor_icon = COMBART_ICON(cid);
	else if (COMMON_SKILL_VALID(sid_atk))
		actor_icon = SKILL_ICON(sid_atk);

	target_icon = 0;
	if (COMMON_SKILL_VALID(sid_def))
		target_icon = SKILL_ICON(sid_def);

	/* Terminator */
	if (actor_icon == 0 && target_icon == 0)
		return false;

	if (gManimSt.subjectActorId == 1) {
		left_icon  = actor_icon;
		right_icon = target_icon;
	} else {
		right_icon = actor_icon;
		left_icon  = target_icon;
	}

	LTRACEF("left_icon %#x right_icon %#x at round %d",
				left_icon, right_icon, round);

	/* Left */
	if (left_icon != 0) {
		procfx = Proc_Start(ProcScr_MapAnimSkillfx, infoproc);
		procfx->x = infoproc->x;
		procfx->y = infoproc->y;
		procfx->icon_idx = left_icon;
		procfx->pos = POS_L;
	}

	/* Right */
	if (right_icon != 0) {
		procfx = Proc_Start(ProcScr_MapAnimSkillfx, infoproc);
		procfx->x = infoproc->x;
		procfx->y = infoproc->y;
		procfx->icon_idx = right_icon;
		procfx->pos = POS_R;
	}

	Decompress(
		gpImg_MapAnimSKILL,
		OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT);

	if (actor_icon != 0) {
		struct Unit *unit = gManimSt.actor[gManimSt.subjectActorId].unit;

		APProc_Create(
			Obj_MapAnimMISS,
			16 * (SCREEN_TILE_X(unit->xPos)) + 8,
			16 * (SCREEN_TILE_Y(unit->yPos)) + 16,
			TILEREF(BM_OBJCHR_BANIM_EFFECT, 0), 0, 2);
	}

	if (target_icon != 0) {
		struct Unit *unit = gManimSt.actor[gManimSt.targetActorId].unit;

		APProc_Create(
			Obj_MapAnimMISS,
			16 * (SCREEN_TILE_X(unit->xPos)) + 8,
			16 * (SCREEN_TILE_Y(unit->yPos)) + 16,
			TILEREF(BM_OBJCHR_BANIM_EFFECT, 0), 0, 2);
	}

	if (actor_icon != 0)
		PlaySeSpacial(0x3D1, SCREEN_TILE_IX(gManimSt.actor[gManimSt.subjectActorId].unit->xPos));
	else
		PlaySeSpacial(0x3D1, SCREEN_TILE_IX(gManimSt.actor[gManimSt.targetActorId].unit->xPos));

	Proc_StartBlocking(ProcScr_SkillMapAnimDeamon, parent);
	return true;
}
