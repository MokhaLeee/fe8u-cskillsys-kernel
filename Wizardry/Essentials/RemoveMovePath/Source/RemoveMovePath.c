#include "common-chax.h"
#include "map-movement.h"

/* LynJump */
void PlayerPhase_DisplayUnitMovement(void)
{
    if (gConfigRemoveMovePath == false)
        GetMovementScriptFromPath();
    else
        GenerateBestMovementScript(
            gBmSt.playerCursor.x,
            gBmSt.playerCursor.y,
            gWorkingMovementScript);

    UnitApplyWorkingMovementScript(gActiveUnit, gActiveUnit->xPos, gActiveUnit->yPos);
    MU_StartMoveScript_Auto(gWorkingMovementScript);
}

/* LynJump */
void DrawUpdatedPathArrow(void)
{
    if (gConfigRemoveMovePath == false)
    {
        UpdatePathArrowWithCursor();
        DrawPathArrow();
    }
}
