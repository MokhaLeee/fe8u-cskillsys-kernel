#include "common-chax.h"
#include "bmsave.h"
#include "sram-layout.h"
#include "bmcontainer.h"
#include "bmmind.h"
#include "eventinfo.h"

#include "save-data.h"

typedef void (* new_save_hook)(void);
extern const new_save_hook gNewSaveHooks[];

const struct EmsChunk * GetEmsChunkByIndex_Sav(int idx)
{
    const struct EmsChunk * chunk_sav;

    for (chunk_sav = gEmsSavChunks; chunk_sav->offset != EMS_CHUNK_INVALID_OFFSET; chunk_sav++)
        if (chunk_sav->chunk_idx == idx)
            return chunk_sav;

    return NULL;
}

const struct EmsChunk * GetEmsChunkByIndex_Sus(int idx)
{
    const struct EmsChunk * chunk_sus;

    for (chunk_sus = gEmsSusChunks; chunk_sus->offset != EMS_CHUNK_INVALID_OFFSET; chunk_sus++)
        if (chunk_sus->chunk_idx == idx)
            return chunk_sus;

    return NULL;
}

/* LynJump! */
void WriteSaveBlockInfo(struct SaveBlockInfo * chunk, int index)
{

    chunk->magic16 = SAVEMAGIC16;
    chunk->offset = SramAddrToOffset(GetSaveWriteAddr(index));

    switch (chunk->kind)
    {
    case SAVEBLOCK_KIND_GAME:
        chunk->size = gEmsSizes[0];
        break;

    case SAVEBLOCK_KIND_SUSPEND:
        chunk->size = gEmsSizes[1];
        break;

    case SAVEBLOCK_KIND_ARENA:
        chunk->size = 0;
        break;

    case SAVEBLOCK_KIND_XMAP:
        chunk->size = 0;
        break;

    case (u8)SAVEBLOCK_KIND_INVALID:
        chunk->size = 0;
        chunk->offset = 0;
        chunk->magic16 = 0;
        break;

    default:
        return;
    }

    PopulateSaveBlockChecksum(chunk);
    WriteAndVerifySramFast(chunk, &gSram->saveBlockInfo[index], sizeof(struct SaveBlockInfo));
}

/* LynJump! */
void * GetSaveWriteAddr(int index)
{
    switch (index) {
        case SAVE_ID_GAME0:
        case SAVE_ID_GAME1:
        case SAVE_ID_GAME2:
        case SAVE_ID_SUSPEND:
        case SAVE_ID_SUSPEND_ALT:
            return CART_SRAM + gEmsOffsets[index];

        case SAVE_ID_ARENA:
        case SAVE_ID_XMAP:
        default:
            return NULL;
    }
}

/* LynJump! */
int SramChecksum32(void * sram_src, int size)
{
    size_t _size = size;
    if (_size > sizeof(gGenericBuffer))
        _size = sizeof(gGenericBuffer);

    ReadSramFast(sram_src, gGenericBuffer, _size);
    return ComputeChecksum32((const u32 *)gGenericBuffer, _size);
}

/* LynJump! */
void CopyGameSave(int index_src, int index_dst)
{
    struct SaveBlockInfo chunk;
    size_t real, size;
    u8 * src = GetSaveReadAddr(index_src);
    u8 * dst = GetSaveWriteAddr(index_dst);

    real = 0;
    size = EMS_SIZE_SAV;

    for (; size > real; size -= real)
    {
        real = size;
        if (real > sizeof(gGenericBuffer))
            real = sizeof(gGenericBuffer);

        ReadSramFast(src, gGenericBuffer, real);
        WriteAndVerifySramFast(gGenericBuffer, dst, real);

        src += real;
        dst += real;

        if (real >= size)
            break;

        size = size - real;
    }

    chunk.magic32 = SAVEMAGIC32;
    chunk.kind = SAVEBLOCK_KIND_GAME;
    WriteSaveBlockInfo(&chunk, index_dst);
}

/* LynJump! */
void WriteNewGameSave(int index, int isDifficult, int mode, int isTutorial)
{
    const new_save_hook * it;

    if (0 == mode)
        mode = gPlaySt.chapterModeIndex;

    if (isTutorial < 0)
        isTutorial = gPlaySt.config.controller;

    InitPlayConfig(isDifficult, isTutorial);
    
    gPlaySt.unk_2C_1 = 0;
    gPlaySt.unk_2C_04 = 0;
    CpuFill16(0, &gPlaySt.unk_30, sizeof(gPlaySt.unk_30));
    gPlaySt.unk_2C_0D = 0;
    gPlaySt.chapterModeIndex = mode;
    gPlaySt.unk_2B_00 = 1;
    gPlaySt.playerName[0] = '\0';
    gPlaySt.chapterIndex = 0;
    gPlaySt.playthroughIdentifier = GetNewPlaythroughId();
    gPlaySt.gameSaveSlot = index;
    gPlaySt.unk_2C_2 = GetGlobalCompletionCount();

    /* External hooks */
    for (it = gNewSaveHooks; *it != NULL; it++)
        (*it)();

    /* Directlt save game! */
    WriteGameSave(index);
}

/* LynJump! */
void WriteGameSave(int slot)
{
    const struct EmsChunk * cur;
    struct SaveBlockInfo chunk;
    u16 chunk_size = gEmsSizes[0]; // SAV
    u8 * dst = GetSaveWriteAddr(slot);

    InvalidateSuspendSave(SAVE_ID_SUSPEND);
    gPlaySt.gameSaveSlot = slot;

    for (cur = gEmsSavChunks; cur->offset != EMS_CHUNK_INVALID_OFFSET; cur++)
    {
        if (cur->offset > chunk_size)
        {
            Errorf("Offset overflow: offset=%#x, size=%#x, saver=%p, loader=%p\n"
                    cur->offset, cur->size, cur->save, cur->load);

            abort();
        }
        cur->save(dst + cur->offset, cur->size);
    }
    chunk.magic32 = SAVEMAGIC32;
    chunk.kind = SAVEBLOCK_KIND_GAME;
    WriteSaveBlockInfo(&chunk, slot);
    WriteLastGameSaveId(slot);
}

/* LynJump! */
void ReadGameSave(int slot)
{
    const struct EmsChunk * cur;
    u8 * src = GetSaveReadAddr(slot);
    u16 chunk_size = gEmsSizes[0]; // SAV

    if (!(PLAY_FLAG_HARD & gBmSt.gameStateBits))
        InvalidateSuspendSave(SAVE_ID_SUSPEND);

    InitUnits();

    for (cur = gEmsSavChunks; cur->offset != EMS_CHUNK_INVALID_OFFSET; cur++)
    {
        if (cur->offset > chunk_size)
        {
            Errorf("Offset overflow: offset=%#x, size=%#x, saver=%p, loader=%p\n"
                    cur->offset, cur->size, cur->save, cur->load);

            abort();
        }
        cur->load(src + cur->offset, cur->size);
    }
    gPlaySt.gameSaveSlot = slot;
    WriteLastGameSaveId(slot);
}

/* LynJump! */
void WriteSuspendSave(int slot)
{
    u8 * dst;
    const struct EmsChunk * cur;
    struct SaveBlockInfo chunk;
    u16 chunk_size = gEmsSizes[1]; // SUS

    if (PLAY_FLAG_TUTORIAL & gPlaySt.chapterStateBits)
        return;

    if (!IsSramWorking())
        return;

    slot += GetNextSuspendSaveId();
    dst = GetSaveWriteAddr(slot);
        
    for (cur = gEmsSavChunks; cur->offset != EMS_CHUNK_INVALID_OFFSET; cur++)
    {
        if (cur->offset > chunk_size)
        {
            Errorf("Offset overflow: offset=%#x, size=%#x, saver=%p, loader=%p\n"
                    cur->offset, cur->size, cur->save, cur->load);

            abort();
        }
        cur->save(dst + cur->offset, cur->size);
    }

    chunk.magic32 = SAVEMAGIC32;
    chunk.kind = SAVEBLOCK_KIND_SUSPEND;
    WriteSaveBlockInfo(&chunk, slot);

    gBmSt.just_resumed = false;
    WriteSwappedSuspendSaveId();
}

/* LynJump! */
void ReadSuspendSave(int slot)
{
    const struct EmsChunk * cur;
    u8 * src = GetSaveReadAddr(slot + gSuspendSaveIdOffset);
    u16 chunk_size = gEmsSizes[1]; // SUS

    InitUnits();

    for (cur = gEmsSavChunks; cur->offset != EMS_CHUNK_INVALID_OFFSET; cur++)
    {
        if (cur->offset > chunk_size)
        {
            Errorf("Offset overflow: offset=%#x, size=%#x, saver=%p, loader=%p\n"
                    cur->offset, cur->size, cur->save, cur->load);

            abort();
        }
        cur->load(src + cur->offset, cur->size);
    }

    LoadRNStateFromActionStruct();
    SetGameTime(gPlaySt.time_saved);
}
