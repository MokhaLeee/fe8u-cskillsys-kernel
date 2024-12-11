#include <common-chax.h>
#include <strmag.h>
#include <status-getter.h>

#define UNIT_STATUS_MAX 128

int GetUnitMaxStatusHp(struct Unit *unit)
{
	int status = KUNIT_MHP_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].hp;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusPow(struct Unit *unit)
{
	int status = UNIT_POW_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].pow;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusMag(struct Unit *unit)
{
	int status = GetUnitMaxMagic(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].mag;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusSkl(struct Unit *unit)
{
	int status = UNIT_SKL_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].mag;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusSpd(struct Unit *unit)
{
	int status = UNIT_SPD_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].spd;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusLck(struct Unit *unit)
{
	int status = UNIT_LCK_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].lck;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusDef(struct Unit *unit)
{
	int status = UNIT_DEF_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].def;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusRes(struct Unit *unit)
{
	int status = UNIT_RES_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].res;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusMov(struct Unit *unit)
{
	int status = UNIT_MOV_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].mov;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}

int GetUnitMaxStatusCon(struct Unit *unit)
{
	int status = UNIT_CON_MAX(unit);
	u8 pid = UNIT_CHAR_ID(unit);

	status += gpUnitMaxStatusConfigTable[pid].con;
	LIMIT_AREA(status, 0, UNIT_STATUS_MAX);
	return status;
}
