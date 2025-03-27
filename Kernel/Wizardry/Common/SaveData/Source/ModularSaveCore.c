#include "common-chax.h"
#include "save-data.h"
#include "kernel-lib.h"

const struct EmsChunk *GetEmsChunkByIndex_Sav(int idx)
{
	const struct EmsChunk *chunk_sav;

	for (chunk_sav = gEmsSavChunks; chunk_sav->_identifier_ != EMS_CHUNK_INVALID_OFFSET; chunk_sav++)
		if (chunk_sav->chunk_idx == idx)
			return chunk_sav;

	return NULL;
}

const struct EmsChunk *GetEmsChunkByIndex_Sus(int idx)
{
	const struct EmsChunk *chunk_sus;

	for (chunk_sus = gEmsSusChunks; chunk_sus->_identifier_ != EMS_CHUNK_INVALID_OFFSET; chunk_sus++)
		if (chunk_sus->chunk_idx == idx)
			return chunk_sus;

	return NULL;
}

u32 CalcChunkOffset_Sav(const struct EmsChunk *chunk)
{
	u32 ret = 0;
	const struct EmsChunk *chunk_cur = gEmsSavChunks;

	while (1) {
		if (chunk_cur->_identifier_ == EMS_CHUNK_INVALID_OFFSET) {
			Errorf("Chunk:%p not found, idx=%d", chunk, chunk->chunk_idx);
			hang();
		}

		if (chunk_cur == chunk)
			break;

		ret += chunk_cur->size;
		chunk_cur++;
	}
	return ret;
}

u32 CalcChunkOffset_Sus(const struct EmsChunk *chunk)
{
	u32 ret = 0;
	const struct EmsChunk *chunk_cur = gEmsSusChunks;

	while (1) {
		if (chunk_cur->_identifier_ == EMS_CHUNK_INVALID_OFFSET) {
			Errorf("Chunk:%p not found, idx=%d", chunk, chunk->chunk_idx);
			hang();
		}

		if (chunk_cur == chunk)
			break;

		ret += chunk_cur->size;
		chunk_cur++;
	}
	return ret;
}

LYN_REPLACE_CHECK(WriteSaveBlockInfo);
void WriteSaveBlockInfo(struct SaveBlockInfo *chunk, int index)
{

	chunk->magic16 = SAVEMAGIC16;
	chunk->offset = SramAddrToOffset(GetSaveWriteAddr(index));

	switch (chunk->kind) {
	case SAVEBLOCK_KIND_GAME:
		chunk->size = EMS_SIZE_SAV;
		break;

	case SAVEBLOCK_KIND_SUSPEND:
		chunk->size = EMS_SIZE_SUS;
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

LYN_REPLACE_CHECK(GetSaveWriteAddr);
void *GetSaveWriteAddr(int index)
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


LYN_REPLACE_CHECK(CopyGameSave);
void CopyGameSave(int index_src, int index_dst)
{
	struct SaveBlockInfo chunk;
	size_t real, size;
	u8 *src = GetSaveReadAddr(index_src);
	u8 *dst = GetSaveWriteAddr(index_dst);

	real = 0;
	size = EMS_SIZE_SAV;

	for (; size > real; size -= real) {
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

LYN_REPLACE_CHECK(WriteNewGameSave);
void WriteNewGameSave(int index, int isDifficult, int mode, int isTutorial)
{
	const new_save_hook *it;

	if (mode == 0)
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
	for (it = gpNewSaveHooks; *it != NULL; it++)
		(*it)();

	/* Directlt save game! */
	WriteGameSave(index);
}

LYN_REPLACE_CHECK(WriteGameSave);
void WriteGameSave(int slot)
{
	const struct EmsChunk *cur;
	struct SaveBlockInfo chunk;
	u16 chunk_size = EMS_SIZE_SAV; // SAV
	u8 *dst = GetSaveWriteAddr(slot);
	u32 offset = 0;

	InvalidateSuspendSave(SAVE_ID_SUSPEND);
	gPlaySt.gameSaveSlot = slot;

	for (cur = gEmsSavChunks; cur->_identifier_ != EMS_CHUNK_INVALID_OFFSET; cur++) {
		if (offset > chunk_size) {
			Errorf("Offset overflow: offset=%#x, size=%#x, saver=%p, loader=%p",
					offset, cur->size, cur->save, cur->load);

			hang();
		}
		cur->save(dst + offset, cur->size);
		offset += cur->size;
	}

	/* Terminator */
	MSA_ExternalSaver(dst + offset, EMS_SIZE_SAV - offset);

	chunk.magic32 = SAVEMAGIC32;
	chunk.kind = SAVEBLOCK_KIND_GAME;
	WriteSaveBlockInfo(&chunk, slot);
	WriteLastGameSaveId(slot);
}

LYN_REPLACE_CHECK(ReadGameSave);
void ReadGameSave(int slot)
{
	const struct EmsChunk *cur;
	u8 *src = GetSaveReadAddr(slot);
	u16 chunk_size = EMS_SIZE_SAV; // SAV
	u32 offset = 0;

	if (!(PLAY_FLAG_HARD & gBmSt.gameStateBits))
		InvalidateSuspendSave(SAVE_ID_SUSPEND);

	InitUnits();

	for (cur = gEmsSavChunks; cur->_identifier_ != EMS_CHUNK_INVALID_OFFSET; cur++) {
		if (offset > chunk_size) {
			Errorf("Offset overflow: offset=%#x, size=%#x, saver=%p, loader=%p\n",
					offset, cur->size, cur->save, cur->load);

			hang();
		}
		cur->load(src + offset, cur->size);
		offset += cur->size;
	}

	/* Terminator */
	MSA_ExternalLoader(src + offset, EMS_SIZE_SAV - offset);

	gPlaySt.gameSaveSlot = slot;
	WriteLastGameSaveId(slot);
}

LYN_REPLACE_CHECK(WriteSuspendSave);
void WriteSuspendSave(int slot)
{
	u8 *dst;
	const struct EmsChunk *cur;
	struct SaveBlockInfo chunk;
	u16 chunk_size = EMS_SIZE_SUS; // SUS
	u32 offset = 0;

	if (PLAY_FLAG_TUTORIAL & gPlaySt.chapterStateBits)
		return;

	if (!IsSramWorking())
		return;

#ifdef CONFIG_NO_SUS_IN_AI_PHASE
	if ((gpKernelDesigerConfig->no_suspend_in_aiphase == true)) {
		/* Overwrure this config bit to enable configuration for player runtime */
		if (gPlaySt.config.disableTerrainDisplay == 0) {
			switch (gActionData.suspendPointType) {
			case SUSPEND_POINT_PLAYERIDLE:
				break;

			case SUSPEND_POINT_PHASECHANGE:
				if (gPlaySt.faction == FACTION_BLUE)
					break;

				/* Fall through */

			default:
				return;
			}
		}
	}
#endif

	slot += GetNextSuspendSaveId();
	dst = GetSaveWriteAddr(slot);

	for (cur = gEmsSusChunks; cur->_identifier_ != EMS_CHUNK_INVALID_OFFSET; cur++) {
		if (offset > chunk_size) {
			Errorf("Offset overflow: offset=%#x, size=%#x, saver=%p, loader=%p\n",
					offset, cur->size, cur->save, cur->load);

			hang();
		}
		cur->save(dst + offset, cur->size);
		offset += cur->size;
	}

	/* Terminator */
	MSU_ExternalSaver(dst + offset, EMS_SIZE_SUS - offset);

	chunk.magic32 = SAVEMAGIC32;
	chunk.kind = SAVEBLOCK_KIND_SUSPEND;
	WriteSaveBlockInfo(&chunk, slot);

	gBmSt.just_resumed = false;
	WriteSwappedSuspendSaveId();
}

LYN_REPLACE_CHECK(ReadSuspendSave);
void ReadSuspendSave(int slot)
{
	const struct EmsChunk *cur;
	u8 *src = GetSaveReadAddr(slot + gSuspendSaveIdOffset);
	u16 chunk_size = EMS_SIZE_SUS; // SUS
	u32 offset = 0;

	InitUnits();

	for (cur = gEmsSusChunks; cur->_identifier_ != EMS_CHUNK_INVALID_OFFSET; cur++) {
		if (offset > chunk_size) {
			Errorf("Offset overflow: offset=%#x, size=%#x, saver=%p, loader=%p\n",
					offset, cur->size, cur->save, cur->load);

			hang();
		}
		cur->load(src + offset, cur->size);
		offset += cur->size;
	}

	/* Terminator */
	MSU_ExternalLoader(src + offset, EMS_SIZE_SUS - offset);

	LoadRNStateFromActionStruct();
	SetGameTime(gPlaySt.time_saved);
}

void GameInit_DetectEmsChunks(void)
{
	FORCE_DECLARE int i;
	const struct EmsChunk *cur;
	u32 offset;

	i = 0;

	Print("Dump SAV");
	for (offset = 0, cur = gEmsSavChunks; cur->_identifier_ != EMS_CHUNK_INVALID_OFFSET; i++, cur++) {
		Printf("[%02d]: offset=0x%04X, size=0x%04X, saver=%p, loader=%p",
				i, offset, cur->size, cur->save, cur->load);

		offset += cur->size;
	}

	if (offset > EMS_SIZE_SAV) {
		Errorf("SAV chunk overflowed: max=0x%04X, cur=0x%04X", EMS_SIZE_SAV, offset);
		hang();
	}

	i = 0;

	Print("Dump SUS");
	for (offset = 0, cur = gEmsSusChunks; cur->_identifier_ != EMS_CHUNK_INVALID_OFFSET; i++, cur++) {
		Printf("[%02d]: offset=0x%04X, size=0x%04X, saver=%p, loader=%p",
				i, offset, cur->size, cur->save, cur->load);

		offset += cur->size;
	}

	if (offset > EMS_SIZE_SUS) {
		Errorf("SUS chunk overflowed: max=0x%04X, cur=0x%04X", EMS_SIZE_SUS, offset);
		hang();
	}
}
