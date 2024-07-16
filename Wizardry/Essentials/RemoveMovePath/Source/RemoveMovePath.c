#include "common-chax.h"
#include "kernel-lib.h"
#include "map-movement.h"

/* LynJump */
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

/* LynJump */
void DrawUpdatedPathArrow(void)
{
    if (gpKernelDesigerConfig->remove_move_path == false)
    {
        UpdatePathArrowWithCursor();
        DrawPathArrow();
    }
}
