#include "common-chax.h"
#include "skill-system.h"

#define LOCAL_TRACE 0

void ResetSkillDbgList(void)
{
	memset(gSkillDbgList, 0, sizeof(gSkillDbgList));
}

STATIC_DECLAR struct SkillDbgListEnt *FindSkillDbgListChunk(struct Unit *unit, int sid)
{
	int i;
	int pid = UNIT_CHAR_ID(unit);

	for (i = 0; i < SKILL_DBG_LIST_LEN; i++) {
		struct SkillDbgListEnt *it = gSkillDbgList + i;

		if (it->pid == pid && it->sid == sid)
			return it;
	}
	return NULL;
}

STATIC_DECLAR struct SkillDbgListEnt *FindFreeSkillDbgListChunk(void)
{
	int i;

	for (i = 0; i < SKILL_DBG_LIST_LEN; i++) {
		struct SkillDbgListEnt *it = gSkillDbgList + i;

		if (it->pid == 0 || it->sid == 0)
			return it;
	}
	return NULL;
}

void AddSkillDbgList(struct Unit *unit, int sid)
{
	struct SkillDbgListEnt *it;
	int pid = UNIT_CHAR_ID(unit);

	it = FindSkillDbgListChunk(unit, sid);
	if (it != NULL) {
		Errorf("Already exists: pid=0x%02X, sid=0x%04X", pid, sid);
		return;
	}

	it = FindFreeSkillDbgListChunk();
	if (it == NULL) {
		Error("No free chunk!");
		return;
	}

	it->pid = pid;
	it->sid = sid;

	ResetSkillLists();
	LTRACEF("Add chunk %d: pid=0x%02X, sid=0x%04X", it - gSkillDbgList, pid, sid);
}

void AddSkillDbgListByPid(int pid, int sid)
{
	struct Unit *unit = GetUnitStructFromEventParameter(pid);

	if (unit)
		AddSkillDbgList(unit, sid);
}

void RemoveSkillDbgList(struct Unit *unit, int sid)
{
	struct SkillDbgListEnt *it = FindSkillDbgListChunk(unit, sid);

	if (it == NULL) {
		Errorf("No such chunk: pid=0x%02X, sid=0x%04X", UNIT_CHAR_ID(unit), sid);
		return;
	}

	it->pid = 0;
	it->sid = 0;
	ResetSkillLists();
}

void FlushUnitSkillDbgList(struct Unit *unit)
{
	int i;
	int pid = UNIT_CHAR_ID(unit);

	for (i = 0; i < SKILL_DBG_LIST_LEN; i++) {
		struct SkillDbgListEnt *it = gSkillDbgList + i;

		if (it->pid == pid) {
			it->pid = 0;
			it->sid = 0;
		}
	}
	ResetSkillLists();
}

void EMS_SaveSkillDbgList(u8 *dst, const u32 size)
{
	Assert(size >= sizeof(gSkillDbgList));

	WriteAndVerifySramFast(gSkillDbgList, dst, sizeof(gSkillDbgList));
}

void EMS_LoadSkillDbgList(u8 *src, const u32 size)
{
	Assert(size >= sizeof(gSkillDbgList));

	ReadSramFast(src, gSkillDbgList, sizeof(gSkillDbgList));
}

void AppendSkillListViaDebugList(struct Unit *unit, struct SkillList *list, u8 *ref)
{
	int i;
	int pid = UNIT_CHAR_ID(unit);

	for (i = 0; i < SKILL_DBG_LIST_LEN; i++) {
		struct SkillDbgListEnt *it = gSkillDbgList + i;

		LTRACEF("chunk=%d, pid=0x%02X, sid=0x%04X", i, it->pid, it->sid);

		if (it->pid != pid)
			continue;

		if (it->sid == 0) {
			Errorf("Invalid chunk %d: pid=0x%02X", i, pid);
			continue;
		}

		if (ref[it->sid])
			continue;

		if (SkillListOverflow(list)) {
			Error("list overflowed");
			return;
		}

		ref[it->sid] = 1;
		list->sid[list->amt++] = it->sid;
	}
}
