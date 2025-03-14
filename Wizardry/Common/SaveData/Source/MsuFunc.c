#include "common-chax.h"
#include "debug-kit.h"
#include "save-data.h"
#include "strmag.h"
#include "debuff.h"
#include "battle-system.h"

void MSU_SavePlaySt(u8 *dst, const u32 size)
{
	gPlaySt.time_saved = GetGameClock();
	WriteAndVerifySramFast(&gPlaySt, dst, size);
}

void MSU_LoadPlaySt(u8 *src, const u32 size)
{
	ReadSramFast(src, &gPlaySt, size);
	SetGameTime(gPlaySt.time_saved);
}

void MSU_SaveAction(u8 *dst, const u32 size)
{
	StoreRNStateToActionStruct();
	WriteAndVerifySramFast(&gActionData, dst, size);
}

void MSU_LoadAction(u8 *src, const u32 size)
{
	ReadSramFast(src, &gActionData, size);
	LoadRNStateFromActionStruct();
}

void MSU_SaveMenuMask(u8 *dst, const u32 size)
{
	u8 list[MENU_OVERRIDE_MAX];

	GetForceDisabledMenuItems(list);
	WriteAndVerifySramFast(list, dst, size);
}

void MSU_LoadMenuMask(u8 *src, const u32 size)
{
	u8 list[MENU_OVERRIDE_MAX];

	ReadSramFast(src, list, size);
	SetForceDisabledMenuItems(list);
}

void MSU_SaveWorldMap(u8 *dst, const u32 size)
{
	WriteWorldMapStuff(dst, &gGMData);
}

void MSU_LoadWorldMap(u8 *src, const u32 size)
{
	ReadWorldMapStuff(src, &gGMData);
}

void MSU_SaveDungeon(u8 *dst, const u32 size)
{
	struct Dungeon dungeon[2];

	SaveDungeonRecords(dungeon);
	WriteAndVerifySramFast(dungeon, dst, size);
}

void MSU_LoadDungeon(u8 *src, const u32 size)
{
	struct Dungeon dungeon[2];

	ReadSramFast(src, dungeon, size);
	LoadDungeonRecords(dungeon);
}

void MSU_SaveDungeon2(u8 *dst, const u32 size)
{
	struct Dungeon dungeon;

	SaveDungeonState(&dungeon);
	WriteAndVerifySramFast(&dungeon, dst, size);
}

void MSU_LoadDungeon2(u8 *src, const u32 size)
{
	struct Dungeon dungeon;

	ReadSramFast(src, &dungeon, size);
	LoadDungeonState(&dungeon);
}

void MSU_SaveEvtCounter(u8 *dst, const u32 size)
{
	int val = GetEventSlotCounter();

	WriteAndVerifySramFast(&val, dst, size);
}

void MSU_LoadEvtCounter(u8 *src, const u32 size)
{
	int val;

	ReadSramFast(src, &val, size);
	SetEventSlotCounter(val);
}

void MSU_SaveBonusClaim(void) {}

void MSU_LoadBonusClaimWIP(void)
{
	u32 buf;
	const struct EmsChunk *chunk = GetEmsChunkByIndex_Sav(EMS_CHUNK_BONUSCLAIMDATA);
	void *src = GetSaveReadAddr(gPlaySt.gameSaveSlot);

	ReadSramFast(src + CalcChunkOffset_Sav(chunk), &buf, sizeof(buf)); /* read from save data */
	SetBonusContentClaimFlags(buf);
}

STATIC_DECLAR void NewPackSuspandUnit(struct Unit *src, struct EmsPackedSusUnit *dst)
{
	u32 i;
	struct Unit tmp_unit;

	if (!dst)
		return;

	if (!src || !UNIT_IS_VALID(src)) {
		ClearUnit(&tmp_unit);
		src = &tmp_unit;
		dst->jid = 0;
		dst->pid = 0;
	} else {
		dst->jid = UNIT_CLASS_ID(src);
		dst->pid = UNIT_CHAR_ID(src);
	}

	dst->max_hp = src->maxHP;
	dst->pow = src->pow;
	dst->mag = UNIT_MAG(src);
	dst->skl = src->skl;
	dst->spd = src->spd;
	dst->lck = src->lck;
	dst->def = src->def;
	dst->res = src->res;
	dst->mov = src->movBonus;
	dst->con = src->conBonus;
	dst->level = src->level;
	dst->exp = src->exp;
	dst->xPos = src->xPos;
	dst->yPos = src->yPos;

	for (i = 0; i < 8; i++)
		dst->ranks[i] = src->ranks[i];

	for (i = 0; i < 0x5; i++)
		dst->items[i] = src->items[i];

	dst->state = src->state;

#if !CHAX
	dst->status = src->statusIndex;
	dst->duration = src->statusDuration;
#else
	dst->status = UNIT_STATUS_INDEX(src);
	dst->duration = UNIT_STATUS_DURATION(src);
#endif

	if (UNIT_FACTION(src) == FACTION_BLUE) {
		dst->pad.ally.rescue = src->rescue;
		dst->pad.ally.ballista = src->ballistaIndex;
		dst->pad.ally.torch = src->torchDuration;
		dst->pad.ally.barrier = src->barrierDuration;

		for (i = 0; i < ARRAY_COUNT(dst->pad.ally.skills); i++)
			dst->pad.ally.skills[i] = src->supports[i];

		dst->pad.ally.support_gain = src->supportBits;
		dst->pad.ally.cur_hp = src->curHP;
	} else {
		for (i = 0; i < ARRAY_COUNT(dst->pad.ai.skills); i++)
			dst->pad.ai.skills[i] = src->supports[i];

		dst->pad.ai.ai1 = src->ai1;
		dst->pad.ai.ai1_cur = src->ai_a_pc;
		dst->pad.ai.ai2 = src->ai2;
		dst->pad.ai.ai2_cur = src->ai_b_pc;
		dst->pad.ai.ai_flag = src->aiFlags;
		dst->pad.ai.ai_config = src->ai_config;
		dst->pad.ai.cur_hp = src->curHP;
	}
	dst->_u3A = src->_u3A;
	dst->_u3B = src->_u3B;
}

STATIC_DECLAR void NewUnpackSuspandUnit(struct EmsPackedSusUnit *src, struct Unit *dst)
{
	u32 i;

	if (!src || !dst)
		return;

	dst->pCharacterData = GetCharacterData(src->pid);
	dst->pClassData = GetClassData(src->jid);
	dst->maxHP = src->max_hp;
	dst->pow = src->pow;
	UNIT_MAG(dst) = src->mag;
	dst->skl = src->skl;
	dst->spd = src->spd;
	dst->lck = src->lck;
	dst->def = src->def;
	dst->res = src->res;
	dst->movBonus = src->mov;
	dst->conBonus = src->con;
	dst->level = src->level;
	dst->exp = src->exp;
	dst->xPos = src->xPos;
	dst->yPos = src->yPos;

	for (i = 0; i < 8; i++)
		dst->ranks[i] = src->ranks[i];

#if !CHAX
	dst->statusIndex = src->status;
	dst->statusDuration = src->duration;
#else
	UNIT_STATUS_INDEX(dst) = src->status;
	UNIT_STATUS_DURATION(dst) = src->duration;
#endif

	if (UNIT_FACTION(dst) == FACTION_BLUE) {
		for (i = 0; i < ARRAY_COUNT(src->pad.ally.skills); i++)
			dst->supports[i] = src->pad.ally.skills[i];

		dst->rescue = src->pad.ally.rescue;
		dst->ballistaIndex = src->pad.ally.ballista;
		dst->torchDuration = src->pad.ally.torch;
		dst->barrierDuration = src->pad.ally.barrier;

		dst->supportBits = src->pad.ally.support_gain;
		dst->curHP = src->pad.ally.cur_hp;
	} else {
		for (i = 0; i < ARRAY_COUNT(src->pad.ai.skills); i++)
			dst->supports[i] = src->pad.ai.skills[i];

		dst->ai1 = src->pad.ai.ai1;
		dst->ai_a_pc = src->pad.ai.ai1_cur;
		dst->ai2 = src->pad.ai.ai2;
		dst->ai_b_pc = src->pad.ai.ai2_cur;
		dst->aiFlags = src->pad.ai.ai_flag;
		dst->ai_config = src->pad.ai.ai_config;
		dst->curHP = src->pad.ai.cur_hp;
	}

	dst->state = src->state;

	dst->_u3A = src->_u3A;
	dst->_u3B = src->_u3B;

	for (i = 0; i < 0x5; i++)
		dst->items[i] = src->items[i];

	if (dst->exp > 99)
		dst->exp = -1;

	if (dst->xPos == 0x3F)
		dst->xPos = -1;

	if (dst->yPos == 0x3F)
		dst->yPos = -1;
}

void MSU_SaveBlueUnits(u8 *dst, const u32 size)
{
	int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

	for (i = 1; i <= amt; i++) {
		struct EmsPackedSusUnit pack;

		NewPackSuspandUnit(GetUnit(i + FACTION_BLUE), &pack);
		WriteAndVerifySramFast(&pack, dst, SIZE_OF_SUS_UNIT_PACK);
		dst += SIZE_OF_SUS_UNIT_PACK;
	}
}

void MSU_LoadBlueUnits(u8 *src, const u32 size)
{
	int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

	for (i = 1; i <= amt; i++) {
		struct EmsPackedSusUnit pack;

		ReadSramFast(src, &pack, SIZE_OF_SUS_UNIT_PACK);
		NewUnpackSuspandUnit(&pack, GetUnit(i + FACTION_BLUE));
		src += SIZE_OF_SUS_UNIT_PACK;
	}
}

void MSU_SaveRedUnits(u8 *dst, const u32 size)
{
	int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

	for (i = 1; i <= amt; i++) {
		struct EmsPackedSusUnit pack;

		NewPackSuspandUnit(GetUnit(i + FACTION_RED), &pack);
		WriteAndVerifySramFast(&pack, dst, SIZE_OF_SUS_UNIT_PACK);
		dst += SIZE_OF_SUS_UNIT_PACK;
	}
}

void MSU_LoadRedUnits(u8 *src, const u32 size)
{
	int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

	for (i = 1; i <= amt; i++) {
		struct EmsPackedSusUnit pack;

		ReadSramFast(src, &pack, SIZE_OF_SUS_UNIT_PACK);
		NewUnpackSuspandUnit(&pack, GetUnit(i + FACTION_RED));
		src += SIZE_OF_SUS_UNIT_PACK;
	}
}

void MSU_SaveGreenUnits(u8 *dst, const u32 size)
{
	int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

	for (i = 1; i <= amt; i++) {
		struct EmsPackedSusUnit pack;

		NewPackSuspandUnit(GetUnit(i + FACTION_GREEN), &pack);
		WriteAndVerifySramFast(&pack, dst, SIZE_OF_SUS_UNIT_PACK);
		dst += SIZE_OF_SUS_UNIT_PACK;
	}
}

void MSU_LoadGreenUnits(u8 *src, const u32 size)
{
	int i, amt = size / SIZE_OF_SUS_UNIT_PACK;

	for (i = 1; i <= amt; i++) {
		struct EmsPackedSusUnit pack;

		ReadSramFast(src, &pack, SIZE_OF_SUS_UNIT_PACK);
		NewUnpackSuspandUnit(&pack, GetUnit(i + FACTION_GREEN));
		src += SIZE_OF_SUS_UNIT_PACK;
	}
}

void MSU_SaveRedUnitExtSkills(u8 *dst, const u32 size)
{
	int i, amt = size / 4;

	for (i = 1; i <= amt; i++) {
		struct Unit *unit = GetUnit(i + FACTION_RED);

		WriteAndVerifySramFast(&unit->supports[3], dst, 4);
		dst += 4;
	}
}

void MSU_LoadRedUnitExtSkills(u8 *src, const u32 size)
{
	int i, amt = size / 4;

	for (i = 1; i <= amt; i++) {
		struct Unit *unit = GetUnit(i + FACTION_RED);

		ReadSramFast(src, &unit->supports[3], 4);
		src += 4;
	}
}

void MSU_SaveGreenUnitExtSkills(u8 *dst, const u32 size)
{
	int i, amt = size / 4;

	for (i = 1; i <= amt; i++) {
		struct Unit *unit = GetUnit(i + FACTION_GREEN);

		WriteAndVerifySramFast(&unit->supports[3], dst, 4);
		dst += 4;
	}
}

void MSU_LoadGreenUnitExtSkills(u8 *src, const u32 size)
{
	int i, amt = size / 4;

	for (i = 1; i <= amt; i++) {
		struct Unit *unit = GetUnit(i + FACTION_GREEN);

		ReadSramFast(src, &unit->supports[3], 4);
		src += 4;
	}
}

void MSU_SaveSelectionPos(u8 *dst, const u32 size)
{
	Assert(size >= (sizeof(gActiveUnitMoveOrigin) + sizeof(gBattleTargetPositionBackup)));

	WriteAndVerifySramFast(&gActiveUnitMoveOrigin, dst, sizeof(gActiveUnitMoveOrigin));

	dst += sizeof(gActiveUnitMoveOrigin);
	WriteAndVerifySramFast(&gBattleTargetPositionBackup, dst, sizeof(gBattleTargetPositionBackup));
}

void MSU_LoadSelectionPos(u8 *src, const u32 size)
{
	Assert(size >= (sizeof(gActiveUnitMoveOrigin) + sizeof(gBattleTargetPositionBackup)));

	ReadSramFast(src, &gActiveUnitMoveOrigin, sizeof(gActiveUnitMoveOrigin));
	src += sizeof(gActiveUnitMoveOrigin);

	ReadSramFast(src, &gBattleTargetPositionBackup, sizeof(gBattleTargetPositionBackup));
}
