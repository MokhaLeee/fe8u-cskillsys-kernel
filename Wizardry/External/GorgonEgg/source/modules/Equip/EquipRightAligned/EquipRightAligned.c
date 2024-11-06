
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipRightAlignedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipRightAlignedProc_Name[] = "EquipRightAligned";


const struct ProcInstruction ProcEquipRightAligned[] = {
  PROC_SET_NAME(&EquipRightAlignedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipRightAligned_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's equipped weapon's name right-aligned within a space.
   */

  int padding;
  char* equipString;

  struct EquipRightAlignedProc* equipProc = (struct EquipRightAlignedProc*)ProcFind(ProcEquipRightAligned);
  if (equipProc == NULL)
  {
    equipProc = (struct EquipRightAlignedProc*)START_PROC(ProcEquipRightAligned, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if (udp->weapon != 0)
  {
    equipString = GetItemName(udp->weapon);
    equipString = String_ExpandTactName();

    padding = (EQUIP_WIDTH * 8) - Text_GetStringTextWidth(equipString);

    Text_SetParameters(&equipProc->equipText, padding, EQUIP_COLOR);
    Text_DrawString(&equipProc->equipText, equipString);

    Text_Display(&equipProc->equipText, gUiTmScratchA + TILEMAP_INDEX(EQUIP_X, EQUIP_Y));
  }

  return;
}
