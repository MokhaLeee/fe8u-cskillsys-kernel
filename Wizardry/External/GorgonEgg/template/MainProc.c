
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

void GE_Init(struct PlayerInterfaceProc* proc);
void GE_Reset(struct PlayerInterfaceProc* proc);
void GE_Extend(struct PlayerInterfaceProc* proc);
void GE_Update(struct PlayerInterfaceProc* proc);
void GE_Retract(struct PlayerInterfaceProc* proc);
void GE_CheckForUnit(struct PlayerInterfaceProc* proc);

extern const char* GE_Name;


const struct ProcInstruction ProcGE[] = {
    PROC_SET_NAME(&GE_Name),
    PROC_SLEEP(0),

    PROC_CALL_ROUTINE(GE_Init),

    // fallthrough

  PROC_LABEL(GE_PROC_EXTEND),

    PROC_WHILE_EXISTS(gProc_CameraMovement),

    PROC_LOOP_ROUTINE(GE_Reset),
    PROC_LOOP_ROUTINE(GE_Extend),

    // fallthrough

  PROC_LABEL(GE_PROC_UPDATE),

    PROC_LOOP_ROUTINE(GE_Update),

    // fallthrough

  PROC_LABEL(GE_PROC_RETRACT),

    PROC_LOOP_ROUTINE(GE_Retract),
    PROC_GOTO(GE_PROC_EXTEND),

  PROC_LABEL(GE_PROC_CHECK_FOR_UNIT),

    PROC_CALL_ROUTINE(GE_CheckForUnit),
    PROC_GOTO(GE_PROC_UPDATE),

  PROC_END,
};
