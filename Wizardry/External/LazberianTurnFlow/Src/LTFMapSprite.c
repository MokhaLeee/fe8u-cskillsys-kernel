#include "gbafe.h"

extern struct KeyStatusBuffer sKeyStatusBuffer;

// NOTE: REPLACES VANILLA FUNCTION
unsigned GetUnitBattleMapSpritePaletteIndex(struct Unit* unit)
{
	if (unit->state & 0x8000000)
		return 0xB; // Link Arena palette

	if (!(sKeyStatusBuffer.heldKeys & R_BUTTON) && (unit->state & US_UNSELECTABLE))
		return 0xF;

	return GetUnitSpritePalette(unit);
}

void LTFMapSprite_UpdateAllMapSpritePalettes(void)
{
	for (unsigned index = 1; index < 0x100; ++index)
	{
		struct Unit* unit = GetUnit(index);

		if (!unit || !unit->pCharacterData)
			continue;

		if (!unit->pMapSpriteHandle)
			continue;

		struct SMSHandle* handle = unit->pMapSpriteHandle;

		handle->oam2Base &= ~(0xF000);
		handle->oam2Base |= GetUnitBattleMapSpritePaletteIndex(unit) << 12;
	}
}

void LTFMapSprite_UpdateAll(void)
{
	if ((sKeyStatusBuffer.heldKeys ^ sKeyStatusBuffer.prevKeys) & (R_BUTTON))
		LTFMapSprite_UpdateAllMapSpritePalettes();

	PutUnitSpritesOam();
}
