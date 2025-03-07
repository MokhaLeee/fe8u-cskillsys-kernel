#include <common-chax.h>
#include <icon-rework.h>
#include <popup-reowrk.h>
#include <kernel-lib.h>

/**
 * Efx popup
 */

STATIC_DECLAR void EfxPopR_Start(struct PopupProc *proc)
{
	int width, width_popupbox, x_bg1;
	struct Anim *anim;
	struct Text *text = &gBanimText[0];
	struct ProcPopupHanlder *parent = proc->proc_parent;

	proc->xTileParam = -1;
	proc->yTileParam = -1;
	proc->textColorId = TEXT_COLOR_SYSTEM_WHITE;
	proc->iconId = -1;
	proc->iconX = 0;
	proc->soundId = 0;

	ResetIconGraphics_();
	LZ77UnCompVram(Img_EkrPopup, (void *)BG_VRAM + 0x2000);
	LZ77UnCompWram(Tsa_EkrPopup, (void *)gEkrTsaBuffer);
	InitTextFont(&gBanimFont, (void *)BG_VRAM + 0x2100, 0x108, 1);
	SetTextDrawNoClear();
	// CpuFastCopy(Pal_EkrPopup, PAL_BG(0x1), 0x20);

	/**
	 * Parse
	 */
	proc->xGfxSize = width = ParsePopupInstAndGetLen(proc);

	/**
	 * Window frame
	 */
	width_popupbox = DIV_ROUND_CLOSEST(width + 7, 8);
	MakeBattlePopupTileMapFromTSA(gBG1TilemapBuffer, width_popupbox);

	/**
	 * Text
	 */

	InitText(text, width_popupbox);
	Text_SetColor(text, proc->textColorId);
	Text_SetCursor(text, 2);
	LZ77UnCompVram(gUnknown_08803BD0, (void *)BG_VRAM + 0x2100);
	GeneratePopupText(proc->pDefinition, *text);

	/**
	 * Draw BG1
	 */
	x_bg1 = (0xE0 - (width_popupbox + 2) * 8) / 2;
	BG_SetPosition(BG_1,  -x_bg1, -0x30);
	BG_EnableSyncByMask(BG1_SYNC_BIT);

	/**
	 * Icon anim
	 */
	if (proc->iconId != 0xFFFF)
		LoadIconObjectGraphics(proc->iconId, proc->iconObjTileId);

	parent->anim = anim = AnimCreate(AnimScr_EkrPopup, 0x96);
	anim->oam2Base = OAM2_PAL(0x2) + OAM2_LAYER(0x1) + OAM2_CHR(0x0800 / 0x20);
	anim->xPosition = x_bg1 + 0x12 + proc->iconX;
	anim->yPosition = 0x38;

	/**
	 * Sfx
	 */
	if (proc->soundId != 0)
		EfxPlaySE(proc->soundId, 0x80);

	ApplyPalette(Pal_EkrPopup, 1);
	EnablePaletteSync();
	SetDefaultColorEffects();
	SetWinEnable(0, 0, 0);
}

STATIC_DECLAR void EfxPopR_Loop(struct PopupProc *proc)
{
	struct ProcPopupHanlder *parent = proc->proc_parent;

	if (--proc->clock <= 0) {
		AnimDelete(parent->anim);
		SpellFx_ClearBG1();
		Proc_Break(proc);
	}
}

STATIC_DECLAR const struct ProcCmd ProcScr_EfxPopupRework[] = {
	PROC_YIELD,
	PROC_CALL(EfxPopR_Start),
	PROC_YIELD,
	PROC_REPEAT(EfxPopR_Loop),
	PROC_END
};

STATIC_DECLAR void NewEfxPopup(const struct PopupInstruction *inst, int time, ProcPtr parent)
{
	struct PopupProc *proc = Proc_StartBlocking(ProcScr_EfxPopupRework, parent);

	proc->pDefinition = inst;
	proc->clock = time;
	proc->iconObjTileId = 0x40;
	proc->iconPalId = 0x12;
}

/**
 * Ekr popup
 */
STATIC_DECLAR void EkrPopR_Init(struct ProcPopupHanlder *proc)
{
	Sound_SetSEVolume(0x80);
}

STATIC_DECLAR void EkrPopR_Loop(struct ProcPopupHanlder *proc)
{
	const struct PopupConfig *config = proc->config++;

	if (!config || !config->setup || !config->inst) {
		Proc_Goto(proc, 99);
		return;
	}

	if (config->setup()) {
		NewEfxPopup(config->inst, config->duration, proc);
		Proc_Break(proc);
	}
}

STATIC_DECLAR void EkrPopR_End(struct ProcPopupHanlder *proc)
{
	gEkrPopupEnded = true;
	gpProcEkrPopup = NULL;
	Sound_SetSEVolume(0x100);
}

STATIC_DECLAR const struct ProcCmd ProcScr_EkrPopupRework[] = {
	PROC_YIELD,
	PROC_SLEEP(16),
	PROC_CALL(EkrPopR_Init),

PROC_LABEL(0),
	PROC_REPEAT(EkrPopR_Loop),
	PROC_YIELD,
	PROC_GOTO(0),

PROC_LABEL(99),
	PROC_CALL(EkrPopR_End),
	PROC_END
};

LYN_REPLACE_CHECK(NewEkrPopup);
void NewEkrPopup(void)
{
	struct ProcPopupHanlder *proc = Proc_Start(ProcScr_EkrPopupRework, PROC_TREE_3);

	gpProcEkrPopup = (ProcPtr)proc;

	if (gEkrDistanceType == EKR_DISTANCE_PROMOTION)
		proc->config = gpPromotPopupConfigs;
	else
		proc->config = gpBattlePopupConfigs;
}
