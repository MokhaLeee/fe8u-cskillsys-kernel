#include "common-chax.h"
#include "skill-system.h"
#include "arm-func.h"
#include "class-types.h"
#include "weapon-range.h"
#include "kernel-lib.h"
#include "map-movement.h"
#include "constants/skills.h"

STATIC_DECLAR void PreGenerateMovementMap(int default_mov)
{
	FORCE_DECLARE bool Aerobatics_activated;
	FORCE_DECLARE bool FlierFormation_activated;
	FORCE_DECLARE bool SoaringWings_activated;
	FORCE_DECLARE bool self_flier;

	struct Unit *unit;
	int i, ix, iy;

	static const struct Vec1 *vec_ref[4] = {
		NULL,
		gVecs_1x1,
		gVecs_2x2,
		gVecs_3x3,
	};

	static const u8 vec_cnts[4] = {
		0,
		ARRAY_COUNT_RANGE1x1,
		ARRAY_COUNT_RANGE2x2,
		ARRAY_COUNT_RANGE3x3
	};

	KernelMoveMapFlags = 0;

	/* Fasten calc! */
	if (!gMovMapFillState.hasUnit)
		return;

	unit = GetUnit(gMovMapFillState.unitId);

	self_flier = CheckClassFlier(UNIT_CLASS_ID(unit));
	Aerobatics_activated = false;
	FlierFormation_activated = false;
	SoaringWings_activated = false;

#if (defined(SID_Pass) && COMMON_SKILL_VALID(SID_Pass))
	if (SkillTester(unit, SID_Pass) && ((GetUnitCurrentHp(unit) * 4) >= GetUnitMaxHp(unit)))
		KernelMoveMapFlags |= FMOVSTRE_PASS;
#endif

#if (defined(SID_FlierFormation) && COMMON_SKILL_VALID(SID_FlierFormation))
	FlierFormation_activated = SkillTester(unit, SID_FlierFormation);
#endif

#if (defined(SID_Aerobatics) && COMMON_SKILL_VALID(SID_Aerobatics))
	Aerobatics_activated = SkillTester(unit, SID_Aerobatics);
#endif

#if (defined(SID_SoaringWings) && COMMON_SKILL_VALID(SID_SoaringWings))
	SoaringWings_activated = SkillTester(unit, SID_SoaringWings);
#endif

	/**
	 * Here we use generic buffer as a external cost buffer
	 */
	BmMapInit(gGenericBuffer, &KernelExtMoveBarrierMap, gBmMapSize.x, gBmMapSize.y);
	BmMapFill(KernelExtMoveBarrierMap, 0);

	BmMapInit(gGenericBuffer + 0x800, &KernelExtMovePioneerMap, gBmMapSize.x, gBmMapSize.y);
	BmMapFill(KernelExtMovePioneerMap, 0);

	for (iy = 0; iy < gBmMapSize.y; ++iy)
		for (ix = 0; ix < gBmMapSize.x; ++ix) {
			int barrier_range = 0;
			int pioneer_range = 0;
			struct Unit *_unit = GetUnitAtPosition(ix, iy);

			if (!UnitOnMapAvaliable(_unit))
				continue;

			/**
			 * 1. check for barrier map
			 */
			if (!AreUnitsAllied(unit->index, _unit->index) && !(KernelMoveMapFlags & FMOVSTRE_PASS)) { /* Pass skill can ignore barriers */
#if (defined(SID_Obstruct) && COMMON_SKILL_VALID(SID_Obstruct))
				if (SkillTester(_unit, SID_Obstruct)) {
					if ((GetUnitCurrentHp(_unit) * 100) >= (GetUnitMaxHp(_unit) * SKILL_EFF0(SID_Obstruct))) {
						int __barrier = SKILL_EFF1(SID_Obstruct);

						if (barrier_range < __barrier)
							barrier_range = __barrier;
					}
				}
#endif

#if (defined(SID_DetailedReport) && COMMON_SKILL_VALID(SID_DetailedReport))
				if (SkillTester(_unit, SID_DetailedReport)) {
					if ((GetUnitCurrentHp(_unit) * 100) >= (GetUnitMaxHp(_unit) * SKILL_EFF0(SID_DetailedReport))) {
						if (GetUnitMaxRange(unit) >= SKILL_EFF1(SID_DetailedReport)) {
							int __barrier = SKILL_EFF2(SID_DetailedReport);

							if (barrier_range < __barrier)
								barrier_range = __barrier;
						}
					}
				}
#endif
			}

			if (barrier_range > 3)
				barrier_range = 3;

			switch (barrier_range) {
			case 0:
			default:
				break;

			case 1:
			case 2:
			case 3:
				for (i = 0; i < vec_cnts[barrier_range]; i++) {
					int _x = ix + vec_ref[barrier_range][i].x;
					int _y = iy + vec_ref[barrier_range][i].y;

					if (IsPositionValid(_x, _y))
						KernelExtMoveBarrierMap[_y][_x] = -1;
				}

				KernelExtMoveBarrierMap[iy][ix] = -1;
				KernelMoveMapFlags |= FMOVSTRE_BARRIER;
				break;
			}

			/**
			 * 1. check for pioneer map
			 */
			if (AreUnitsAllied(unit->index, _unit->index)) {
				bool ally_flier = CheckClassFlier(UNIT_CLASS_ID(_unit));

				/**
				 * A limitation on balancing:
				 * Self bonus can only apply to allies in move range
				 */
				if (RECT_DISTANCE(unit->xPos, unit->yPos, _unit->xPos, _unit->yPos) <= (default_mov + 1)) {
					if (FlierFormation_activated && ally_flier) {
#if (defined(SID_FlierGuidance) && COMMON_SKILL_VALID(SID_FlierGuidance))
						int __bonus = SKILL_EFF0(SID_FlierFormation);

						if (pioneer_range < __bonus)
							pioneer_range = __bonus;
#endif
					}

					if (Aerobatics_activated && !ally_flier) {
#if (defined(SID_Aerobatics) && COMMON_SKILL_VALID(SID_Aerobatics))
						int __bonus = SKILL_EFF0(SID_Aerobatics);

						if (pioneer_range < __bonus)
							pioneer_range = __bonus;
#endif
					}

					if (SoaringWings_activated) {
#if (defined(SID_SoaringWings) && COMMON_SKILL_VALID(SID_SoaringWings))
						int __bonus = SKILL_EFF0(SID_SoaringWings);

						if (pioneer_range < __bonus)
							pioneer_range = __bonus;
#endif
					}
				}

#if (defined(SID_FlierGuidance) && COMMON_SKILL_VALID(SID_FlierGuidance))
				if (self_flier && SkillTester(_unit, SID_FlierGuidance)) {
					int __bonus = SKILL_EFF0(SID_FlierGuidance);

					if (pioneer_range < __bonus)
						pioneer_range = __bonus;
				}
#endif

#if (defined(SID_Guidance) && COMMON_SKILL_VALID(SID_Guidance))
				if (!self_flier && SkillTester(_unit, SID_Guidance)) {
					int __bonus = SKILL_EFF0(SID_Guidance);

					if (pioneer_range < __bonus)
						pioneer_range = __bonus;
				}
#endif

#if (defined(SID_SoaringGuidance) && COMMON_SKILL_VALID(SID_SoaringGuidance))
				if (SkillTester(_unit, SID_SoaringGuidance)) {
					int __bonus = SKILL_EFF0(SID_SoaringGuidance);

					if (pioneer_range < __bonus)
						pioneer_range = __bonus;
				}
#endif
			}

			if (pioneer_range > 3)
				pioneer_range = 3;

			switch (pioneer_range) {
			case 0:
			default:
				break;

			case 1:
			case 2:
			case 3:
				for (i = 0; i < vec_cnts[pioneer_range]; i++) {
					int _x = ix + vec_ref[pioneer_range][i].x;
					int _y = iy + vec_ref[pioneer_range][i].y;

					if (IsPositionValid(_x, _y) && gWorkingTerrainMoveCosts[gBmMapTerrain[_y][_x]] != (u8)-1)
						KernelExtMovePioneerMap[_y][_x] = 1;
				}

				if (gWorkingTerrainMoveCosts[gBmMapTerrain[iy][ix]] != (u8)-1)
					KernelExtMovePioneerMap[iy][ix] = 1;

				KernelMoveMapFlags |= FMOVSTRE_PIONEER;
				break;
			}
		}
}

LYN_REPLACE_CHECK(GenerateMovementMap);
void GenerateMovementMap(int x, int y, int movement, int uid)
{
	u8 **working_map = gWorkingBmMap;

	gMovMapFillState.dst = gMovMapFillStPool1;
	gMovMapFillState.src = gMovMapFillStPool2;

	gMovMapFillState.movement = movement;

	if (uid == 0)
		gMovMapFillState.hasUnit = false;
	else {
		gMovMapFillState.hasUnit = true;
		gMovMapFillState.unitId = uid;
	}

	gMovMapFillState.maxMovementValue = MAP_MOVEMENT_MAX;

	BmMapFill(gWorkingBmMap, -1);

	gMovMapFillState.dst->xPos = x;
	gMovMapFillState.dst->yPos = y;
	gMovMapFillState.dst->connexion = 5;
	gMovMapFillState.dst->leastMoveCost = 0;

	gWorkingBmMap[y][x] = 0;

	gMovMapFillState.dst++;
	gMovMapFillState.dst->connexion = 4;

#if CHAX
	WARN_GENERIC_BUF_USED;
	PreGenerateMovementMap(movement);
	SetWorkingBmMap(working_map);
#endif

	CallARM_FillMovementMap();
	WARN_GENERIC_BUF_RELEASED;
}

void GameInit_RedirectMapFloodFunc(void)
{
	gUnknown_03003128 = _MapFloodCoreRe;
}
