
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

const char NameLeftAlignedProc_Name[] = "NameLeftAligned";

const struct ProcInstruction ProcNameLeftAligned[] = {
  PROC_SET_NAME(&NameLeftAlignedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};

struct NameLeftAlignedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle nameText;

};


void NameLeftAligned_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's name against the left edge of allocated space.
   */

  char* nameString;
  struct Unit* unit = udp->unit;

  struct NameLeftAlignedProc* textProc = (struct NameLeftAlignedProc*)ProcFind(ProcNameLeftAligned);
  if (textProc == NULL)
  {
    textProc = (struct NameLeftAlignedProc*)START_PROC(ProcNameLeftAligned, proc);
    Text_InitDB(&textProc->nameText, NAME_WIDTH);
  }

  Text_Clear(&textProc->nameText);

  nameString = GetStringFromIndex(unit->pCharacterData->nameTextId);
  nameString = String_ExpandTactName();

  Text_SetParameters(&textProc->nameText, 0, NAME_COLOR);
  Text_DrawString(&textProc->nameText, nameString);

  Text_Display(&textProc->nameText, gUiTmScratchA + TILEMAP_INDEX(NAME_X, NAME_Y));

  return;
}
