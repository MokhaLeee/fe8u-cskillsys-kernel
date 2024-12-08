#include "MapAuraFxUpdate.h"

enum {
	// Constants

	FX_TILE_ROOT = 0x280,
	FX_PALETTE_ROOT = 5,
};

struct MapAuraFxProc
{
	PROC_HEADER;

	/* 2C */ unsigned blend;
	/* 30 */ int vSpeed;
	/* 32 */ int elapsedTime;
};

static void MapAuraFx_OnInit(struct MapAuraFxProc* proc);
static void MapAuraFx_OnLoop(struct MapAuraFxProc* proc);
static void MapAuraFx_OnEnd(struct MapAuraFxProc* proc);

static const struct ProcCmd sProc_MapAuraFx[] = {
	PROC_NAME("Map Aura Fx"),

	PROC_SET_END_CB(MapAuraFx_OnEnd),
	PROC_CALL(MapAuraFx_OnInit),

	PROC_REPEAT(MapAuraFx_OnLoop),
	PROC_YIELD,

	PROC_END
};

struct MapAuraFxUnitProc
{
	PROC_HEADER;

	/* 2C */ struct Unit* pUnit;
};

static void MapAuraFx_Unit_OnInit(struct MapAuraFxUnitProc* proc);
static void MapAuraFx_Unit_OnLoop(struct MapAuraFxUnitProc* proc);
static void MapAuraFx_Unit_OnEnd(struct MapAuraFxUnitProc* proc);

static const struct ProcCmd sProc_MapAuraFx_Unit[] = {
	PROC_NAME("Map Aura Fx Unit"),

	PROC_SLEEP(0),

	PROC_CALL(MapAuraFx_Unit_OnInit),
	PROC_SET_END_CB(MapAuraFx_Unit_OnEnd),

	PROC_REPEAT(MapAuraFx_Unit_OnLoop),

	PROC_END
};

static void UnpackGraphics(void)
{
	static const void* const gfx = (const void* const) (0x08A032AC);
	static const u16*  const pal = (const u16*  const) (0x08A03334);
	static const u16*  const tsa = (const u16*  const) (0x08A03354);

	static const unsigned TILEMAP_BASE = (FX_PALETTE_ROOT << 12) | FX_TILE_ROOT;

	Decompress(gfx, (void*) (VRAM + FX_TILE_ROOT * 0x20));
	CopyToPaletteBuffer(pal, FX_PALETTE_ROOT * 0x20, 0x20);

	CallARM_FillTileRect(gBG2TilemapBuffer, tsa, TILEMAP_BASE);

	// This is probably not the most efficient way of doing this but whatever

	for (unsigned ix = 1; ix < 16; ++ix)
		TileMap_CopyRect(gBG2TilemapBuffer, BG_LOCATED_TILE(gBG2TilemapBuffer, ix*2, 0), 2, 4);

	for (unsigned iy = 1; iy < 8; ++iy)
		TileMap_CopyRect(gBG2TilemapBuffer, BG_LOCATED_TILE(gBG2TilemapBuffer, 0, iy*4), 32, 4);

    EnablePaletteSync();
	BG_EnableSync(2);
}

static void InitSpecialEffects(void)
{
    //DISPCNT works when 00 9F. This code should do exactly that.
	gLCDControlBuffer.dispcnt.win0_on = FALSE;
	gLCDControlBuffer.dispcnt.win1_on = FALSE;
	gLCDControlBuffer.dispcnt.objWin_on = TRUE;

    //idk the exact values for WININ tbh but this works
    //3C 20 works
    gLCDControlBuffer.wincnt.win0_enableBg0 = FALSE;
    gLCDControlBuffer.wincnt.win0_enableBg1 = FALSE;
    gLCDControlBuffer.wincnt.win0_enableBg2 = TRUE;
    gLCDControlBuffer.wincnt.win0_enableBg3 = TRUE;
    gLCDControlBuffer.wincnt.win0_enableObj = TRUE;
    gLCDControlBuffer.wincnt.win0_enableBlend = TRUE;

    gLCDControlBuffer.wincnt.win1_enableBg0 = FALSE;
    gLCDControlBuffer.wincnt.win1_enableBg1 = FALSE;
    gLCDControlBuffer.wincnt.win1_enableBg2 = FALSE;
    gLCDControlBuffer.wincnt.win1_enableBg3 = FALSE;
    gLCDControlBuffer.wincnt.win1_enableObj = FALSE;
    gLCDControlBuffer.wincnt.win1_enableBlend = TRUE;

    //WINOUT must be configured this way
    //1B 3F
	gLCDControlBuffer.wincnt.wout_enableBg0 = TRUE;
	gLCDControlBuffer.wincnt.wout_enableBg1 = TRUE;
	gLCDControlBuffer.wincnt.wout_enableBg2 = FALSE;
	gLCDControlBuffer.wincnt.wout_enableBg3 = TRUE;
	gLCDControlBuffer.wincnt.wout_enableObj = TRUE;
	gLCDControlBuffer.wincnt.wout_enableBlend = FALSE;

	gLCDControlBuffer.wincnt.wobj_enableBg0 = TRUE;
	gLCDControlBuffer.wincnt.wobj_enableBg1 = TRUE;
	gLCDControlBuffer.wincnt.wobj_enableBg2 = TRUE;
	gLCDControlBuffer.wincnt.wobj_enableBg3 = TRUE;
	gLCDControlBuffer.wincnt.wobj_enableObj = TRUE;
	gLCDControlBuffer.wincnt.wobj_enableBlend = TRUE;

    //bgControl[2] = 00 0E 00 00
	gLCDControlBuffer.bg2cnt.priority = 0;
    gLCDControlBuffer.bg2cnt.screenBaseBlock = 0;
    
    //BLDCNT 44 30 00 00
    //blendCoeffA = 10; B = 10
	SetBlendTargetA(FALSE, FALSE, TRUE,  FALSE, FALSE);
	SetBlendTargetB(FALSE, FALSE, FALSE, FALSE, TRUE);
}

static void MapAuraFx_OnInit(struct MapAuraFxProc* proc)
{
	UnpackGraphics();
	InitSpecialEffects();

	proc->vSpeed = 8;
	proc->blend  = 4;
	proc->elapsedTime  = 0;

	// Calling this once to perhaps avoid flickering?
	// (I didn't notice any flickering without it but just in case)
	MapAuraFx_OnLoop(proc);
	proc->elapsedTime  = 0;
}

static void MapAuraFx_OnLoop(struct MapAuraFxProc* proc)
{
    proc->elapsedTime++;
    if(proc->elapsedTime > 32) {Proc_Break(proc);}
    unsigned newBlend = proc->blend + 1;
    if(newBlend >= 16) {newBlend = (32 - newBlend);}
    else if(newBlend < 8) {newBlend = proc->blend + 2;}
    else {newBlend = 16;}
    proc->blend = newBlend;
	BG_SetPosition(2, 0, (proc->vSpeed * GetGameClock() / 32) % 32);
	SetBlendConfig(1, proc->blend, 16, 0);
}

static void MapAuraFx_OnEnd(struct MapAuraFxProc* proc)
{
	gLCDControlBuffer.dispcnt.win0_on = FALSE;
	gLCDControlBuffer.dispcnt.win1_on = FALSE;
	gLCDControlBuffer.dispcnt.objWin_on = FALSE;

	BG_Fill(gBG2TilemapBuffer, 0);
	BG_EnableSync(2);
    Proc_EndEach(sProc_MapAuraFx_Unit);
}

static void MapAuraFx_Unit_OnInit(struct MapAuraFxUnitProc* proc)
{
	// Hide SMS, as we are going to display it manually
	HideUnitSprite(proc->pUnit);

	// Display it once
	MapAuraFx_Unit_OnLoop(proc);
}

static void MapAuraFx_Unit_OnLoop(struct MapAuraFxUnitProc* proc)
{
	// TODO: add to libgbafe
	static void(*SMS_DisplayWindowBlended)(int, int, int, int, struct Unit*)
		= (void(*)(int node, int x, int y, int info, struct Unit* unit))(0x08028014+1);

	// Display semi-transparent map sprite + obj window map sprite

	SMS_DisplayWindowBlended(
		11,
		proc->pUnit->xPos * 16 - gBmSt.camera.x,
		proc->pUnit->yPos * 16 - gBmSt.camera.y,
		(proc->pUnit->pMapSpriteHandle->oam2Base &~ 0x3FF) | (2 << 10), // priority 2
		proc->pUnit
	);
}

static void MapAuraFx_Unit_OnEnd(struct MapAuraFxUnitProc* proc)
{
	// Restore SMS
	ShowUnitSprite(proc->pUnit);
}

/*
 * ====================
 * = PUBLIC FUNCTIONS =
 * ====================
 */

void StartMapAuraFx(void)
{
	Proc_Start(sProc_MapAuraFx, ROOT_PROC_3);
}

void EndMapAuraFx(void)
{
	Proc_EndEach(sProc_MapAuraFx);
}

int IsMapAuraFxActive(void)
{
	return Proc_Find(sProc_MapAuraFx) ? TRUE : FALSE;
}

void SetMapAuraFxSpeed(int speed)
{
	struct MapAuraFxProc* proc = (struct MapAuraFxProc*) Proc_Find(sProc_MapAuraFx);

	if (proc)
	{
		proc->vSpeed = speed;
	}
}

void SetMapAuraFxBlend(unsigned blend)
{
	struct MapAuraFxProc* proc = (struct MapAuraFxProc*) Proc_Find(sProc_MapAuraFx);

	if (proc)
	{
		proc->blend = blend;
	}
}

void SetMapAuraFxPalette(const u16 palette[])
{
	struct MapAuraFxProc* proc = (struct MapAuraFxProc*) Proc_Find(sProc_MapAuraFx);

	if (proc)
	{
		CopyToPaletteBuffer(palette, FX_PALETTE_ROOT * 0x20, 0x20);
        EnablePaletteSync();
	}
}

void AddMapAuraFxUnit(struct Unit* unit)
{
	if (UNIT_IS_VALID(unit) && unit->pMapSpriteHandle)
	{
		struct MapAuraFxProc* parent = (struct MapAuraFxProc*) Proc_Find(sProc_MapAuraFx);

		if (parent)
		{
			struct MapAuraFxUnitProc* unitProc =
				(struct MapAuraFxUnitProc*) Proc_Start(sProc_MapAuraFx_Unit, parent);

			unitProc->pUnit = unit;
			parent->elapsedTime  = 0;
		}
	}
}

#if MAP_AURA_FX_EXAMPLE

// A trivial example
// Use as menu command switch in/out callbacks

int CommandSwitchIn(void)
{
	StartMapAuraFx();

	for (unsigned i = 1; i < 10; ++i)
	{
		AddMapAuraFxUnit(GetUnit(i));
	}

	return ME_NONE;
}

int CommandSwitchOut(void)
{
	EndMapAuraFx();

	return ME_NONE;
}

#endif

