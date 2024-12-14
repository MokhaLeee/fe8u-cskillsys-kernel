#include "FE-CLib/include/gbafe.h"
#include "common-chax.h" 

s8 AreAllegiancesAllied(int factionA, int factionB) {
	factionA &= 0xFF; factionB &= 0xFF;
	if (factionA  == factionB) return TRUE; // if same units, then they're obviously allied
	factionA = (factionA >> 6);
	factionB =  (factionB >> 6);
	if ((GetChapterDefinition(gPlaySt.chapterIndex)->) >> (factionA * 4 + factionB) & 1) return TRUE; else return FALSE;
}

void SwitchGameplayPhase() {
	switch(gPlaySt.faction) {
		case FACTION_BLUE:
		gPlaySt.faction = FACTION_RED;
		break;
		case FACTION_RED:
		gPlaySt.faction = FACTION_GREEN;
		break;
		case FACTION_GREEN:
		gPlaySt.faction = FACTION_PURPLE;
		break;
		case FACTION_PURPLE:
		gChapterData.currentPhase = FACTION_BLUE;
		if(gPlaySt.chapterTurnNumber < 999) gPlaySt.chapterTurnNumber++;
		ProcessSupportGains();
		break;
	}
}

int GetCurrentMapMusicIndex() {
	int PlayerPhaseMusicIndex, EnemyPhaseMusicIndex, OtherPhaseMusicIndex, NeutralPhaseMusicIndex; 
	if(CheckEventId(4)) PlayerPhaseMusicIndex = 6; else PlayerPhaseMusicIndex = 0;
	if(CheckEventId(4)) EnemyPhaseMusicIndex = 7; else EnemyPhaseMusicIndex = 1;
	if(CheckEventId(4)) OtherPhaseMusicIndex = 5; else OtherPhaseMusicIndex = 2;
	if(CheckEventId(4)) NeutralPhaseMusicIndex = 4; else NeutralPhaseMusicIndex = 3;
	switch(gPlaySt.faction) {
		case FACTION_RED:
			return GetChapterDefinition(gPlaySt.chapterIndex)->mapBgmIds[EnemyPhaseMusicIndex];
		case FACTION_GREEN:
			return GetChapterDefinition(gPlaySt.chapterIndex)->mapBgmIds[OtherPhaseMusicIndex];
		case FACTION_PURPLE:
			return GetChapterDefinition(gPlaySt.chapterIndex)->mapBgmIds[NeutralPhaseMusicIndex];
		case FACTION_BLUE: 
			if(CheckEventId(4)) // Victory Music doesn't play if flag 4 is on?
				return GetChapterDefinition(gPlaySt.chapterIndex)->mapBgmIds[PlayerPhaseMusicIndex];
			else { // Should normal player phase music be played or victory music?
				if(GetBattleMapType() == 2 || GetChapterDefinition(gPlaySt.chapterIndex)->victorySongEnemyThreshold != 0) {
					if (CountUnitInFactionWithoutAttributes(FACTION_RED, US_UNAVAILABLE) < GetChapterDefinition(gChapterData.chapterIndex)->victorySongEnemyThreshold || CountUnitInFactionWithoutAttributes(FACTION_RED, US_UNAVAILABLE) == 0) return 0x10;
					
				}
				return GetChapterDefinition(gPlaySt.chapterIndex)->mapBgmIds[PlayerPhaseMusicIndex];
			}
	}
}