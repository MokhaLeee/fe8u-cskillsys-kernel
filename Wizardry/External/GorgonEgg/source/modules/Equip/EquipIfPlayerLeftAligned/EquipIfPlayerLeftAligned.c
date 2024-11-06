
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipIfPlayerLeftAlignedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipIfPlayerLeftAlignedProc_Name[] = "EquipIfPlayerLeftAligned";


const struct ProcInstruction ProcEquipIfPlayerLeftAligned[] = {
  PROC_SET_NAME(&EquipIfPlayerLeftAlignedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipIfPlayerLeftAligned_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's equipped weapon's name left-aligned within a space.
   */

  char* equipString;

  struct EquipIfPlayerLeftAlignedProc* equipProc = (struct EquipIfPlayerLeftAlignedProc*)ProcFind(ProcEquipIfPlayerLeftAligned);
  if ( equipProc == NULL )
  {
    equipProc = (struct EquipIfPlayerLeftAlignedProc*)START_PROC(ProcEquipIfPlayerLeftAligned, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if ( (UNIT_FACTION(udp->unit) == FACTION_BLUE) && (udp->weapon != 0) )
  {
    equipString = GetItemName(udp->weapon);
    equipString = String_ExpandTactName();

    Text_SetParameters(&equipProc->equipText, 0, EQUIP_COLOR);
    Text_DrawString(&equipProc->equipText, equipString);

    Text_Display(&equipProc->equipText, gUiTmScratchA + TILEMAP_INDEX(EQUIP_X, EQUIP_Y));
  }

  return;
}
