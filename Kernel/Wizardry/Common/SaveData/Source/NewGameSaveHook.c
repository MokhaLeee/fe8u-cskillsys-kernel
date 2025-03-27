#include "common-chax.h"
#include "save-data.h"

void NewGameSaveGenericHook(void)
{
	u8 *dst = GetSaveWriteAddr(gPlaySt.gameSaveSlot);

	SetGameTime(0);
	InitUnits();
	ClearSupplyItems();
	ResetPermanentFlags();
	InvalidateSuspendSave(SAVE_ID_SUSPEND);
	SetBonusContentClaimFlags(0);

	/* ClearWorldMapStuff */
	ClearWorldMapStuff(dst + CalcChunkOffset_Sav(GetEmsChunkByIndex_Sav(EMS_CHUNK_WMDATA)));

	/* ClearPidChStatsSaveData */
	CpuFill16(0, gPidStatsData, sizeof(gPidStatsData));
	CpuFill16(0, gChapterStats, sizeof(gChapterStats));

	/* dungeon */
	CpuFill16(0, gDungeonState.dungeon, sizeof(gDungeonState.dungeon));
}
