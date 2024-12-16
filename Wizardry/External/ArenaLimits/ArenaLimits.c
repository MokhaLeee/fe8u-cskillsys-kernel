#include "include/gbafe.h"
#include "common-chax.h"

// Arena Limits. Hack by Zeta/Gilgamesh
// Requires FE-CLIB
// Free to use/modify

#define ArenaLimitTrapID 0xEE
#define TEXT_A 0x3
#define TEXT_N 0x1
#define TEXT_X 0x0
#define ARENA_WIN 0x1
#define ARENA_LOSS 0x2
#define ARENA_OK 0x1
#define CANT_ARENA 0x3

void CallMapEventEngine(void* event, u8 wait);
int BWL_AddWinOrLossIdk(int a1, u8 a2, char a3);
int** UnitKill(struct Unit* a1);

struct _ArenaLimitTable
{
	u8 ChapterID;
	u8 LevelLimit;
	u8 TurnLimit;
};

extern struct _ArenaLimitTable ArenaLimitTable[];

extern char* FreeSpaceRAM;

extern char ArenaLevelLimitString[];
extern char ArenaClosedString[];

extern char UnpromotedOnlyString[];
extern char UnpromotedNoLimitString[];

extern void* ArenaLimitEvent;

extern u8 gArenaData;
extern u8 NonLethalArena;
extern u8 MaxLevel;

// adds \0 at the end, returns location of said null terminator (incase you want to expand the string afterwards)
char* StringCopy(char* Source, char* Destination)
{
	int x = 0;
	while (Source[x] != 0x0)
	{
		Destination[x] = Source[x];
		x++;
	}
	Destination[x] = 0x0; // null terminator
	
	return &Destination[x];
}

char* u8ToString(u8 Source, char* Destination)
{
	if (Source < 10)
	{
		Destination[0] = Source + 0x30;
		return &Destination[1];
	}
	else if (Source >= 10 && Source < 99)
	{
		u8 firstDigit = Div(Source, 10);
		u8 secondDigit = Source - (firstDigit * 10);
		
		Destination[0] = firstDigit + 0x30;
		Destination[1] = secondDigit + 0x30;
		return &Destination[2];
	}
	else
	{
		Destination[0] = '?';
		Destination[1] = '?';
		return &Destination[2];
	}
}

u8 CheckArenaLimits(void)
{
	for (int x = 0; ArenaLimitTable[x].ChapterID != 0xFF; x++)
	{
		if (ArenaLimitTable[x].ChapterID == gPlaySt.chapterIndex)
		{
			if (ArenaLimitTable[x].TurnLimit)
			{	
				if (gPlaySt.chapterTurnNumber > ArenaLimitTable[x].TurnLimit)
				{
					char* temp = StringCopy(ArenaClosedString, FreeSpaceRAM);
					temp[0] = TEXT_A;
					temp[1] = TEXT_X;
					CallMapEventEngine(ArenaLimitEvent, 1);
					
					return CANT_ARENA;
				}
			}
			
			if (ArenaLimitTable[x].LevelLimit) // perform level limit check
			{
				u8 TempLevel = gActiveUnit->level;
				if (gActiveUnit->pClassData->attributes & CA_PROMOTED)
					TempLevel += MaxLevel;
				
				if (TempLevel > ArenaLimitTable[x].LevelLimit)
				{
					char* temp = StringCopy(ArenaLevelLimitString, FreeSpaceRAM);
					temp[0] = ' ';
					
					if (ArenaLimitTable[x].LevelLimit > MaxLevel)
						temp = u8ToString(ArenaLimitTable[x].LevelLimit - MaxLevel, &temp[1]);
					else
						temp = u8ToString(ArenaLimitTable[x].LevelLimit, &temp[1]);
					
					temp[0] = TEXT_N;
					temp++;
					
					if (ArenaLimitTable[x].LevelLimit > MaxLevel)
						temp = StringCopy(UnpromotedNoLimitString, temp);
					else
						temp = StringCopy(UnpromotedOnlyString, temp);
					
					temp[0] = TEXT_A;
					temp[1] = TEXT_X;
					
					CallMapEventEngine(ArenaLimitEvent, 1);
					return CANT_ARENA;
				}
			}
			
			return ARENA_OK; // if we didn't hit any limits by now, we're good to arena
			
		}	

	}
	
	return ARENA_OK; // if the chapter isn't in the table, then you're good to use the arena
}

void KillUnitIfNoHealth(struct Unit* unit)
{
	u8 *ArenaVictoryByte = &gArenaData + 0xA;
	if (GetUnitCurrentHp(unit) == 0)
	{
		if (*ArenaVictoryByte == ARENA_LOSS && NonLethalArena)
		{
			unit->curHP = 1;
		}
		else
		{
			int** Something = UnitKill(unit); // something is a pointer to a pointer
			int* Something2 = *Something + 1; // and we want whatever is at *something + 4 bytes
			BWL_AddWinOrLossIdk(*Something2, 0x0, 0x6);
		}

	}
}

u8 DidUnitDie(struct BattleUnit* bUnit)
{
	u8 *ArenaVictoryByte = &gArenaData + 0xA;
	if (GetUnitCurrentHp(&bUnit->unit) == 0)
	{
		if (*ArenaVictoryByte == ARENA_LOSS && NonLethalArena)
		{
			bUnit->unit.curHP = 1;
			return 0;
		}
		else
			return 1;
	}
	else
		return 0;
}