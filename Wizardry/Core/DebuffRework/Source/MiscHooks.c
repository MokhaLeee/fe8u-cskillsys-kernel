#include "common-chax.h"
#include "debuff.h"

#define LOCAL_TRACE 0

bool UNIT_STONED(struct Unit *unit)
{
	return (UNIT_STATUS_INDEX(unit) == UNIT_STATUS_PETRIFY || UNIT_STATUS_INDEX(unit) == UNIT_STATUS_13);
}

LYN_REPLACE_CHECK(SetUnitStatus);
void SetUnitStatus(struct Unit *unit, int status)
{
	if (status == 0) {
		SetUnitStatusIndex(unit, 0);
		SetUnitStatusDuration(unit, 0);
	} else if (status >= NEW_UNIT_STATUS_MAX) {
		Errorf("Status error: %d", status);
		hang();
	} else {
		int duration = gpDebuffInfos[status].duration;

		if (duration == 0)
			duration = 3;

		LTRACEF("[pid=0x%02X] sttaus=%d, dura=%d", status, duration);

		SetUnitStatusIndex(unit, status);
		SetUnitStatusDuration(unit, duration);
	}
}

LYN_REPLACE_CHECK(SetUnitStatusExt);
void SetUnitStatusExt(struct Unit *unit, int status, int duration)
{
	if (status >= NEW_UNIT_STATUS_MAX) {
		Errorf("Status overflow: %d", status);
		hang();
	}

	if (duration == 0)
		duration = 3;

	SetUnitStatusIndex(unit, status);
	SetUnitStatusDuration(unit, duration);
}

LYN_REPLACE_CHECK(GetUnitStatusName);
char *GetUnitStatusName(struct Unit *unit)
{
	int msg = gpDebuffInfos[GetUnitStatusIndex(unit)].name;

	if (msg == 0)
		msg = 0x52B; /* --[X] */

	return GetStringFromIndex(msg);
}

LYN_REPLACE_CHECK(GetUnitRescueName);
char *GetUnitRescueName(struct Unit *unit)
{
	if (!unit->rescue)
		return GetStringFromIndex(gpDebuffInfos[UNIT_STATUS_NONE].name);

	return GetStringFromIndex(GetUnit(unit->rescue)->pCharacterData->nameTextId);
}

LYN_REPLACE_CHECK(HbPopulate_SSStatus);
void HbPopulate_SSStatus(struct HelpBoxProc *proc)
{
	u8 index = GetUnitStatusIndex(gStatScreen.unit);

	proc->mid = gpDebuffInfos[index].desc;
}

LYN_REPLACE_CHECK(PutUnitMapUiStatus);
void PutUnitMapUiStatus(u16 *buffer, struct Unit *unit)
{
	int tileIdx = 0x16F;
	const u8 *img;

	if (!unit)
		return;

	img = gpDebuffInfos[GetUnitStatusIndex(unit)].img;

	if (img) {
		CpuFastCopy(img, (void *)BG_VRAM + 0x2DE0, 0xA0);

		buffer[0] = tileIdx++;
		buffer[1] = tileIdx++;
		buffer[2] = tileIdx++;
		buffer[3] = tileIdx++;
		buffer[4] = tileIdx++;
		buffer[5] = 0;
		buffer[6] = TILEREF(0x128 + GetUnitStatusDuration(unit), 1);
	}
}
