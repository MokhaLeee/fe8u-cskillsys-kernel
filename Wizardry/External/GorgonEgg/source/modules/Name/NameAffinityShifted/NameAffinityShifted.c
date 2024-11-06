
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

const char NameAffinityShiftedProc_Name[] = "NameAffinityShifted";

const struct ProcInstruction ProcNameAffinityShifted[] = {
  PROC_SET_NAME(&NameAffinityShiftedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};

struct NameAffinityShiftedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle nameText;

};


void NameAffinityShifted_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's name left-aligned, shifted right if the unit has an affinity.
   */

  char* nameString;
  struct Unit* unit = udp->unit;

  int padding = (unit->pCharacterData->affinity == 0) ? 0 : (2 * 8);

  // Create our text proc if it doesn't already exist.
  struct NameAffinityShiftedProc* textProc = (struct NameAffinityShiftedProc*)ProcFind(ProcNameAffinityShifted);
  if ( textProc == NULL )
  {
    textProc = (struct NameAffinityShiftedProc*)START_PROC(ProcNameAffinityShifted, proc);
    Text_InitDB(&textProc->nameText, NAME_WIDTH);
  }

  Text_Clear(&textProc->nameText);

  nameString = GetStringFromIndex(unit->pCharacterData->nameTextId);
  nameString = String_ExpandTactName();

  Text_SetParameters(&textProc->nameText, padding, NAME_COLOR);
  Text_DrawString(&textProc->nameText, nameString);

  Text_Display(&textProc->nameText, gUiTmScratchA + TILEMAP_INDEX(NAME_X, NAME_Y));

  return;
}
