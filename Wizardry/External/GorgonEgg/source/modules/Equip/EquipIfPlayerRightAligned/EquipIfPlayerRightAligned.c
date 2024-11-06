
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipIfPlayerRightAlignedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipIfPlayerRightAlignedProc_Name[] = "EquipIfPlayerRightAligned";


const struct ProcInstruction ProcEquipIfPlayerRightAligned[] = {
  PROC_SET_NAME(&EquipIfPlayerRightAlignedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipIfPlayerRightAligned_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's equipped weapon's name centered within a space.
   */

  int padding;
  char* equipString;

  struct EquipIfPlayerRightAlignedProc* equipProc = (struct EquipIfPlayerRightAlignedProc*)ProcFind(ProcEquipIfPlayerRightAligned);
  if ( equipProc == NULL )
  {
    equipProc = (struct EquipIfPlayerRightAlignedProc*)START_PROC(ProcEquipIfPlayerRightAligned, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if ( (UNIT_FACTION(udp->unit) == FACTION_BLUE) && (udp->weapon != 0) )
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
