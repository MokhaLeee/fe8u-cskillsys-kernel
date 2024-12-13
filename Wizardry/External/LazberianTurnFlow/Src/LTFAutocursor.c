#include "LTF.h"

void GetPlayerStartCursorPosition(int* xOut, int* yOut);

struct Unit* LTF_GetAutocursorUnit(unsigned int phase)
{
	for (unsigned index = (phase + 1); index < (phase + 0x40); ++index)
	{
		struct Unit* unit = GetUnit(index);

		if (!UNIT_IS_VALID(unit))
			continue;

		if (unit->statusIndex == UNIT_STATUS_BERSERK)
			continue;

		if (LTF_IsUnitReadyToMove(unit))
			return unit;
	}

	return NULL;
}

void LTF_InitCursorMemory(void)
{
	struct Unit* unit = LTF_GetAutocursorUnit(FACTION_BLUE);

	if (unit)
	{
		gPlaySt.xCursor = unit->xPos;
		gPlaySt.yCursor = unit->yPos;
	}
	else
	{
		gPlaySt.xCursor = gBmMapSize.x / 2;
		gPlaySt.yCursor = gBmMapSize.y / 2;
	}
}

// NOTE: REPLACES VANILLA FUNCTION
LYN_REPLACE_CHECK(GetPlayerStartCursorPosition);
void GetPlayerStartCursorPosition(int* xOut, int* yOut)
{
	if (!gPlaySt.config.autoCursor) // autocursorOption == 1 => autocursor OFF
	{
		struct Unit* unit = LTF_GetAutocursorUnit(gPlaySt.faction);

		if (unit)
		{
			*xOut = unit->xPos;
			*yOut = unit->yPos;

			return;
		}
	}

	*xOut = gPlaySt.xCursor;
	*yOut = gPlaySt.yCursor;
}
