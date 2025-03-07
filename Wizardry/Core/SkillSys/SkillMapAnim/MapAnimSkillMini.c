#include "common-chax.h"
#include "kernel-lib.h"
#include "icon-rework.h"
#include "skill-system.h"

struct ProcSkillMapAnimMini {
	PROC_HEADER;

	int timer;
	int x, y;
	int x_disp, y_disp;
	u16 sid;
};

static void move_camera(struct ProcSkillMapAnimMini *proc)
{
	EnsureCameraOntoPosition(proc, proc->x, proc->y);
}

STATIC_DECLAR void SkillMapAnimMini_Init(struct ProcSkillMapAnimMini *proc)
{
	/* Sprite anim */
	Decompress(
		gpImg_MapAnimSKILL,
		OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT);

	APProc_Create(
		Obj_MapAnimMISS,
		16 * (SCREEN_TILE_X(proc->x)) + 8,
		16 * (SCREEN_TILE_Y(proc->y)) + 16,
		TILEREF(BM_OBJCHR_BANIM_EFFECT, 0), 0, 2);

	PlaySeSpacial(0x3D1, SCREEN_TILE_IX(proc->x));

	/* Icon */
	LoadIconObjectGraphics(SKILL_ICON(proc->sid), MAPANIMFX_CHR_L);
	LoadIconPalette(0, 0x10 + MAPANIMFX_PAL);
	proc->timer = 0;
}

STATIC_DECLAR void SkillMapAnimMini_Loop(struct ProcSkillMapAnimMini *proc)
{
	PutSprite(
		4,
		OAM1_X(SCREEN_TILE_IX(proc->x_disp)),
		OAM0_Y(SCREEN_TILE_IY(proc->y_disp)) + ATTR0_TYPE_BLENDED,
		gObject_16x16,
		OAM2_PAL(MAPANIMFX_PAL) + OAM2_LAYER(0b01) + OAM2_CHR(MAPANIMFX_CHR_L));

	if (++proc->timer == 40)
		Proc_Break(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_SkillMapAnimMini[] = {
	PROC_YIELD,
	PROC_CALL(move_camera),
	PROC_YIELD,
	PROC_CALL(SkillMapAnimMini_Init),
	PROC_YIELD,
	PROC_REPEAT(SkillMapAnimMini_Loop),
	PROC_END
};

void NewSkillMapAnimMini(int x, int y, u16 sid, ProcPtr parent)
{
	struct ProcSkillMapAnimMini *proc;

	if (parent)
		proc = Proc_StartBlocking(ProcScr_SkillMapAnimMini, parent);
	else
		proc = Proc_Start(ProcScr_SkillMapAnimMini, PROC_TREE_3);

	proc->sid = sid;
	proc->x = x;
	proc->y = y;
	proc->x_disp = (x == 0) ? x + 1 : x - 1;
	proc->y_disp = y;
}

bool SkillMapAnimMiniExists(void)
{
	return Proc_Exists(ProcScr_SkillMapAnimMini);
}

/**
 * Mu anim
 */
struct ProcMuSkillAnim {
	PROC_HEADER;

	u16 sid;
	bool mu_gen;
	void (*callback1)(ProcPtr proc);
	void (*callback2)(ProcPtr proc);
};

static void anim_init(struct ProcMuSkillAnim *proc)
{
	struct MuProc *mu;

	proc->mu_gen = false;

	mu = GetUnitMu(gActiveUnit);
	if (!mu) {
		HideUnitSprite(gActiveUnit);
		mu = StartMu(gActiveUnit);
		proc->mu_gen = true;
	}

	SetMuDefaultFacing(mu);
	FreezeSpriteAnim(mu->sprite_anim);
	SetDefaultColorEffects();
	EnsureCameraOntoPosition(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void anim_act(ProcPtr proc)
{
	StartMuActionAnim(GetUnitMu(gActiveUnit));
}

static void skill_anim(struct ProcMuSkillAnim *proc)
{
	NewSkillMapAnimMini(gActiveUnit->xPos, gActiveUnit->yPos, proc->sid, proc);
}

static void _callback1(struct ProcMuSkillAnim *proc)
{
	if (proc->callback1)
		proc->callback1(proc);
}

static void _callback2(struct ProcMuSkillAnim *proc)
{
	if (proc->callback2)
		proc->callback2(proc);
}

// This could be directly called on end, but it is better to release an API for user
STATIC_DECLAR void RemoveMuForActiveUnitExt(void)
{
	struct MuProc *mu = GetUnitMu(gActiveUnit);

	if (mu) {
		EndMu(mu);
		ShowUnitSprite(gActiveUnit);
	}
}

void RemoveMuForActiveUnit(int delay)
{
	CallDelayed(RemoveMuForActiveUnitExt, delay);
}

STATIC_DECLAR const struct ProcCmd ProcScr_MuSkillAnim[] = {
	PROC_CALL(LockGame),
	PROC_CALL(MapAnim_CommonInit),
	PROC_CALL_2(EnsureCameraOntoActiveUnitPosition),
	PROC_CALL(anim_init),
	PROC_YIELD,
	PROC_CALL(anim_act),
	PROC_YIELD,
	PROC_CALL(skill_anim),
	PROC_YIELD,
	PROC_CALL(_callback1),
	PROC_YIELD,
	PROC_CALL(_callback2),
	PROC_YIELD,
	PROC_CALL(UnlockGame),
	PROC_CALL(MapAnim_CommonEnd),
	PROC_YIELD,
	PROC_END
};

void NewMuSkillAnimOnActiveUnit(u16 sid, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc))
{
	struct ProcMuSkillAnim *proc = Proc_Start(ProcScr_MuSkillAnim, PROC_TREE_3);

	proc->sid = sid;
	proc->callback1 = callback1;
	proc->callback2 = callback2;
}

bool MuSkillAnimExists(void)
{
	return Proc_Exists(ProcScr_MuSkillAnim);
}

STATIC_DECLAR const struct ProcCmd ProcScr_MuSkillAnimDeamon[] = {
	PROC_YIELD,
	PROC_WHILE(MuSkillAnimExists),
	PROC_END
};

void NewMuSkillAnimOnActiveUnitWithDeamon(ProcPtr parent, u16 sid, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc))
{
	NewMuSkillAnimOnActiveUnit(sid, callback1, callback2);
	Proc_StartBlocking(ProcScr_MuSkillAnimDeamon, parent);
}

static void event_callcamera(ProcPtr proc)
{
	EnsureCameraOntoPosition(proc, gActiveUnit->xPos, gActiveUnit->yPos);
}

static void event_act(ProcPtr proc)
{
	struct Unit *unit;
	struct MuProc *mu;

	unit = gActiveUnit;

	MapAnim_CommonInit();

	HideUnitSprite(unit);
	mu = StartMu(unit);

	FreezeSpriteAnim(mu->sprite_anim);
	SetMuDefaultFacing(mu);
	SetDefaultColorEffects();
}

static void event_callskill(ProcPtr proc)
{
	StartMuActionAnim(GetUnitMu(gActiveUnit));
	NewSkillMapAnimMini(gActiveUnit->xPos, gActiveUnit->yPos, gEventSlots[EVT_SLOT_B], proc);
}

const EventScr EventScr_MuSkillAnim[] = {
	STAL(1)
	ASMC(event_callcamera)
	STAL(1)
	ASMC(event_act)
	STAL(1)
	ASMC(event_callskill)
	STAL(10)
	NoFade
	ENDA
};
