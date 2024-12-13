#include "LTF.h"

// static void StartObject(int xOrigin, int yOrigin, int chrDisp, int isTurn, struct Proc* parent);
// static void SetObjectAllBlend(void);
// static void SetObjectAllFlat(void);
// static void EndAllObjects(void);

// typedef struct Proc* ProcPtr;

#define PROC_UNLOCK_PARENT(aProc) do \
{ \
	struct Proc* __proc = (struct Proc*)(aProc); \
	__proc->proc_flags &= ~2; \
	((struct Proc*)__proc->proc_parent)->proc_lockCnt--; \
} while (0)

extern const u8  gLTFTurnSwitchSheetImg[];
extern const u16 gLTFTurnSwitchSheetPal[];

enum
{
	LTF_TURNSWITCH_GFX_OBJPAL = 2,
	LTF_TURNSWITCH_GFX_OBJCHR = 0x180,
	LTF_TURNSWITCH_GFX_BGPAL = 2,
	LTF_TURNSWITCH_GFX_BGCHR = 0x200,
} turnSwitchVariables;

static const struct ObjData sSpr_Turn =
{
	1, { 0x4000, 0xC000, TILEREF(0, 0) } // h, 64x32
};

static const struct ObjData sSpr_Number =
{
	1, { 0x8000, 0x8000, TILEREF(0, 0) } // v, 16x32
};

struct TurnSwitchFxProc
{
	PROC_HEADER;

	/* 2C */ unsigned blendTimer;
	/* 30 */ unsigned turnDigitCnt;
	/* 34 */ s8 turnDigits[4];
	/* 38 */ struct Vec2u origin;
};

static void TurnSwitchFx_OnInit(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnBlendInLoop(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnBlendInEnd(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnBlendOutInit(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnBlendOutLoop(struct TurnSwitchFxProc* proc);
static void TurnSwitchFx_OnEnd(struct TurnSwitchFxProc* proc);

#define TRIG_MAX 0x1000
#define TRIG_PI 0x80

#define SIN_LOOKUP(value) (gSinLookup[(value) % 0x100])

struct ObjectProc
{
	PROC_HEADER;

	/* 2A */ u16 chrDisp;
	/* 2C */ struct Vec2u origin;

	/* 30 */ unsigned positionTimer;
	/* 34 */ int xDisplayOffset;

	/* 38 */ int isTurn;
};

static void TurnObject_OnInit(struct ObjectProc* proc);
static void TurnObject_PutOnBg(struct ObjectProc* proc);
static void TurnObject_OnDispBlend(struct ObjectProc* proc);
static void TurnObject_OnDispFlat(struct ObjectProc* proc);

static const struct ProcCmd sProc_Object[] =
{
	PROC_NAME("Stan:LTF:TurnObject"),

	PROC_CALL(TurnObject_OnInit),
	PROC_SLEEP(1),

PROC_LABEL(0),
	PROC_CALL(TurnObject_PutOnBg),
	PROC_CALL(TurnObject_OnDispBlend),
	PROC_REPEAT(TurnObject_OnDispBlend),

PROC_LABEL(1),
	PROC_REPEAT(TurnObject_OnDispFlat),

	PROC_END
};

static
void SetObjectBlend(struct Proc* proc)
{
	Proc_Goto(proc, 0);
}

static
void SetObjectAllBlend(void)
{
	Proc_ForEach(sProc_Object, (ProcFunc)SetObjectBlend);
}

static
void SetObjectFlat(struct Proc* proc)
{
	Proc_Goto(proc, 1);
}

static
void SetObjectAllFlat(void)
{
	BG_Fill(gBG1TilemapBuffer, 0);
	BG_EnableSyncByMask(BG1_SYNC_BIT);

	Proc_ForEach(sProc_Object, (ProcFunc)SetObjectFlat);
}

static
void EndAllObjects(void)
{
	Proc_EndEach(sProc_Object);
}

const struct ProcCmd gProc_TurnSwitchFx[] =
{
	PROC_NAME("Stan:LTF:TurnSwitchFx"),

//	PROC_NEW_CHILD(0x0859AF40),

PROC_LABEL(1),
	PROC_CALL(TurnSwitchFx_OnInit),

PROC_LABEL(0),
	PROC_SLEEP(1),

	PROC_REPEAT(TurnSwitchFx_OnBlendInLoop),
	PROC_CALL(TurnSwitchFx_OnBlendInEnd),

	PROC_SLEEP(60),

	PROC_CALL(TurnSwitchFx_OnBlendOutInit),
	PROC_REPEAT(TurnSwitchFx_OnBlendOutLoop),

	PROC_CALL(TurnSwitchFx_OnEnd),

//	PROC_WHILE_EXISTS(0x0859AF40),

	PROC_END
};

static
void StartObject(int xOrigin, int yOrigin, int chrDisp, int isTurn, struct Proc* parent)
{
	struct ObjectProc* proc = Proc_Start(sProc_Object, parent);

	proc->origin.x = xOrigin;
	proc->origin.y = yOrigin;
	proc->chrDisp = chrDisp;
	proc->isTurn = isTurn;
}

static void TurnSwitchFx_OnInit(struct TurnSwitchFxProc* proc)
{
	memset(proc->turnDigits, -1, sizeof(proc->turnDigits));

	unsigned turn = gPlaySt.chapterTurnNumber;
	unsigned digicnt = 0;

	do
	{
		proc->turnDigits[3] = proc->turnDigits[2];
		proc->turnDigits[2] = proc->turnDigits[1];
		proc->turnDigits[1] = proc->turnDigits[0];
		proc->turnDigits[0] = simple_mod(turn, 10);

		digicnt = digicnt+1;
		turn = Div(turn, 10);
	}
	while (turn);

	proc->origin.x = (120 - (72 + digicnt*16)/2) >> 3 << 3;
	proc->origin.y = (80 - 16) >> 3 << 3;

	// gLCDIOBuffer.bgControl[1].priority = gLCDIOBuffer.bgControl[3].priority - 1;

	SetBlendConfig(BLEND_EFFECT_ALPHA, 0, 0x10, 0);
	SetBlendTargetA(0, 1, 0, 0, 0);
	SetBlendTargetB(0, 0, 1, 1, 1);

	Decompress(gLTFTurnSwitchSheetImg, (void *)VRAM + 0x20 * LTF_TURNSWITCH_GFX_BGCHR);
	ApplyPalette(gLTFTurnSwitchSheetPal, LTF_TURNSWITCH_GFX_BGPAL);

	Decompress(gLTFTurnSwitchSheetImg, (void *)VRAM_OBJ + 0x20 * LTF_TURNSWITCH_GFX_OBJCHR);
	ApplyPalette(gLTFTurnSwitchSheetPal, 0x10 + LTF_TURNSWITCH_GFX_OBJPAL);

	StartObject(proc->origin.x, proc->origin.y, 0, TRUE, (struct Proc*) proc);

	SetObjectAllFlat();
	SetObjectAllBlend();

	proc->blendTimer = 0;
	proc->turnDigitCnt = 0;

	// UPDATE SOUNDS

	if (GetCurrentBgmSong() != GetCurrentMapMusicIndex())
		Sound_FadeOutBGM(8);
}

static
void TurnSwitchFx_OnBlendInLoop(struct TurnSwitchFxProc* proc)
{
	const unsigned eva = proc->blendTimer;
	const unsigned evb = 0x10 - proc->blendTimer;

	SetBlendConfig(BLEND_EFFECT_ALPHA, eva, evb, 0);

	if (++proc->blendTimer > 0x10)
		Proc_Break(proc);
}

static void TurnSwitchFx_OnBlendInEnd(struct TurnSwitchFxProc* proc)
{
	SetObjectAllFlat();

	if (proc->turnDigitCnt < 4 && (proc->turnDigits[proc->turnDigitCnt] >= 0 || proc->turnDigitCnt == 0))
	{
		const unsigned turnDigit = proc->turnDigits[proc->turnDigitCnt];

		StartObject(
			proc->origin.x + 72 + 16*proc->turnDigitCnt,
			proc->origin.y,
			8 + 2 * (turnDigit == 0 ? 9 : turnDigit-1),
			FALSE, (struct Proc*) proc);

		proc->turnDigitCnt++;
		proc->blendTimer = 0;

		Proc_Goto((struct Proc*) proc, 0);
	}
}

static void TurnSwitchFx_OnBlendOutInit(struct TurnSwitchFxProc* proc)
{
	proc->blendTimer = 0;
	SetObjectAllBlend();

	PlaySFX(0x73, 0x100, 0, 1);
}

static void TurnSwitchFx_OnBlendOutLoop(struct TurnSwitchFxProc* proc)
{
	const unsigned eva = proc->blendTimer < 0x20 ? 0x10 : 0x10 - (proc->blendTimer-0x20)/2;
	const unsigned evb = proc->blendTimer < 0x20 ? proc->blendTimer/2 : 0x10;

	if (proc->blendTimer == 0x20)
		PROC_UNLOCK_PARENT(proc);

	SetBlendConfig(BLEND_EFFECT_ALPHA, eva, evb, 0);

	if (++proc->blendTimer > 0x40)
		Proc_Break(proc);
}

static void TurnSwitchFx_OnEnd(struct TurnSwitchFxProc* proc)
{
	EndAllObjects();

	SetPrimaryHBlankHandler(NULL);
	ClearBg0Bg1();
}

static
void TurnObject_PutOnBg(struct ObjectProc* proc)
{
	u16* const out = gBG1TilemapBuffer + TILEMAP_INDEX(proc->origin.x/8, proc->origin.y/8);

	const unsigned width  = proc->isTurn ? 8 : 2;
	const unsigned height = 4;

	for (unsigned iy = 0; iy < height; ++iy)
	{
		for (unsigned ix = 0; ix < width; ++ix)
		{
			out[TILEMAP_INDEX(ix, iy)] = TILEREF(
				LTF_TURNSWITCH_GFX_BGCHR + proc->chrDisp + 0x20 * iy + ix,
				LTF_TURNSWITCH_GFX_BGPAL);
		}
	}

	BG_EnableSyncByMask(BG1_SYNC_BIT);
}

static
void TurnObject_OnInit(struct ObjectProc* proc)
{
	proc->positionTimer = 0;
}

static
void TurnObject_PositionUpdate(struct ObjectProc* proc)
{
	static const unsigned OFFSET_MAX = 0x20;

	if (proc->positionTimer < (TRIG_PI/2))
	{
		const unsigned angle = proc->positionTimer += 2;
		proc->xDisplayOffset = OFFSET_MAX - (SIN_LOOKUP(angle) * OFFSET_MAX / TRIG_MAX);
	}
	else
	{
		proc->xDisplayOffset = 0;
	}
}

static
void TurnObject_OnDispBlend(struct ObjectProc* proc)
{
	TurnObject_PositionUpdate(proc);
	BG_SetPosition(1, -proc->xDisplayOffset, 0);
}

static
void TurnObject_OnDispFlat(struct ObjectProc* proc)
{
	TurnObject_PositionUpdate(proc);

	PutSpriteExt(0, 0x1FF & (proc->origin.x + proc->xDisplayOffset), 0x0FF & (proc->origin.y), (u16 *)(proc->isTurn ? &sSpr_Turn : &sSpr_Number), TILEREF(LTF_TURNSWITCH_GFX_OBJCHR + proc->chrDisp, LTF_TURNSWITCH_GFX_OBJPAL));
}
