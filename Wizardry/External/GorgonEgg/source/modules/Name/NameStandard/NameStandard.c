
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct NameStandardProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle nameText;

};


const char NameStandardProc_Name[] = "NameStandard";


const struct ProcInstruction ProcNameStandard[] = {
  PROC_SET_NAME(&NameStandardProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void NameStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's name similarly to how vanilla does.
   *
   * This places the name centered within the given space.
   */
  int padding;
  char* nameString;
  struct Unit* unit = udp->unit;

  struct NameStandardProc* textProc = (struct NameStandardProc*)ProcFind(ProcNameStandard);
  if (textProc == NULL)
  {
    textProc = (struct NameStandardProc*)START_PROC(ProcNameStandard, proc);
    Text_InitDB(&textProc->nameText, NAME_WIDTH);
  }

  Text_Clear(&textProc->nameText);

  nameString = GetStringFromIndex(unit->pCharacterData->nameTextId);
  nameString = String_ExpandTactName();

  padding = Text_GetStringTextCenteredPos((NAME_WIDTH * 8), nameString);

  Text_SetParameters(&textProc->nameText, padding, NAME_COLOR);
  Text_DrawString(&textProc->nameText, nameString);

  Text_Display(&textProc->nameText, gUiTmScratchA + TILEMAP_INDEX(NAME_X, NAME_Y));

  return;
}
