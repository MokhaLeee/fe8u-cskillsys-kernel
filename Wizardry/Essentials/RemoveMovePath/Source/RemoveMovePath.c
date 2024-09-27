#include "common-chax.h"
#include "kernel-lib.h"
#include "map-movement.h"

LYN_REPLACE_CHECK(PlayerPhase_DisplayUnitMovement);
void PlayerPhase_DisplayUnitMovement(void)
{
	if (gpKernelDesigerConfig->remove_move_path == false)
		GetMovementScriptFromPath();
	else
		GenerateBestMovementScript(
			gBmSt.playerCursor.x,
			gBmSt.playerCursor.y,
			gWorkingMovementScript);

	UnitApplyWorkingMovementScript(gActiveUnit, gActiveUnit->xPos, gActiveUnit->yPos);
	SetAutoMuMoveScript(gWorkingMovementScript);
}

LYN_REPLACE_CHECK(DrawUpdatedPathArrow);
void DrawUpdatedPathArrow(void)
{
	if (gpKernelDesigerConfig->remove_move_path == false) {
		UpdatePathArrowWithCursor();
		DrawPathArrow();
	}
}
