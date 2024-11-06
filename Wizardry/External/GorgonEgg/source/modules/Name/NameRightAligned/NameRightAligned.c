
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

const char NameRightAlignedProc_Name[] = "NameRightAligned";

const struct ProcInstruction ProcNameRightAligned[] = {
  PROC_SET_NAME(&NameRightAlignedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};

struct NameRightAlignedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle nameText;

};


void NameRightAligned_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's name against the right edge of allocated space.
   */
  int padding;
  char* nameString;
  struct Unit* unit = udp->unit;

  struct NameRightAlignedProc* textProc = (struct NameRightAlignedProc*)ProcFind(ProcNameRightAligned);
  if (textProc == NULL)
  {
    textProc = (struct NameRightAlignedProc*)START_PROC(ProcNameRightAligned, proc);
    Text_InitDB(&textProc->nameText, NAME_WIDTH);
  }

  Text_Clear(&textProc->nameText);

  nameString = GetStringFromIndex(unit->pCharacterData->nameTextId);
  nameString = String_ExpandTactName();

  padding = (NAME_WIDTH * 8) - Text_GetStringTextWidth(nameString);

  Text_SetParameters(&textProc->nameText, padding, NAME_COLOR);
  Text_DrawString(&textProc->nameText, nameString);

  Text_Display(&textProc->nameText, gUiTmScratchA + TILEMAP_INDEX(NAME_X, NAME_Y));

  return;
}
