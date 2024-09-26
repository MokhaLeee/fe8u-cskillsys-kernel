#include "common-chax.h"
#include "event-rework.h"

LYN_REPLACE_CHECK(GetUnitStructFromEventParameter);
struct Unit *GetUnitStructFromEventParameter(s16 pid)
{
#if CHAX
	if (pid <= CHAR_EVT_K_MAX)
#else
	if (pid < -3)
#endif
		return NULL;

	switch (pid) {
	case CHAR_EVT_SLOT2:
		pid = (u16)gEventSlots[2];
		break;

	case CHAR_EVT_POSITION_AT_SLOTB:
		if (gBmMapUnit[((u16 *)(gEventSlots + 0xB))[1]][((u16 *)(gEventSlots + 0xB))[0]] != 0)
			return GetUnit(gBmMapUnit[((u16 *)(gEventSlots + 0xB))[1]][((u16 *)(gEventSlots + 0xB))[0]]);
		else
			return NULL;

	case CHAR_EVT_ACTIVE_UNIT:
		return gActiveUnit;

	case CHAR_EVT_PLAYER_LEADER:
		pid = GetPlayerLeaderUnitId();
		break;

#if CHAX
	case CHAR_EVT_UID_AT_SLOT2:
		return GetUnit(gEventSlots[EVT_SLOT_2]);
#endif
	}

	return GetUnitFromCharId(pid);
}
