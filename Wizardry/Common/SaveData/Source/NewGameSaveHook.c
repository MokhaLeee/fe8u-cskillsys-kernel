#include "common-chax.h"
#include "save-data.h"

void NewGameSaveGenericHook(void)
{
    const struct EmsChunk * chunk;
    int slot = gPlaySt.gameSaveSlot;
    u8 * dst = GetSaveWriteAddr(slot);

    SetGameTime(0);
    InitUnits();
    ClearSupplyItems();
    ResetPermanentFlags();
    InvalidateSuspendSave(SAVE_ID_SUSPEND);
    SetBonusContentClaimFlags(0);

    ClearWorldMapStuff(dst + GetEmsChunkByIndex_Sav(EMS_CHUNK_WMDATA)->offset);

    if (CHAX)
    {
        /* ClearPidChStatsSaveData */
        CpuFill16(0, gPidStatsData, sizeof(gPidStatsData));
        CpuFill16(0, gChapterStats, sizeof(gChapterStats));

        chunk = GetEmsChunkByIndex_Sav(EMS_CHUNK_BWLENTRIES);
        chunk->save(dst + chunk->offset, chunk->size);

        chunk = GetEmsChunkByIndex_Sav(EMS_CHUNK_WINDATA);
        chunk->save(dst + chunk->offset, chunk->size);
    }
    
    if (CHAX)
    {
        /* dungeon */
        CpuFill16(0, gDungeonState.dungeon, sizeof(gDungeonState.dungeon));
        chunk = GetEmsChunkByIndex_Sav(EMS_CHUNK_DUNGEON);
        chunk->save(dst + chunk->offset, chunk->size);
    }
}
