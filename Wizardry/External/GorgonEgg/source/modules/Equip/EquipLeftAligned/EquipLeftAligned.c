
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipLeftAlignedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipLeftAlignedProc_Name[] = "EquipLeftAligned";


const struct ProcInstruction ProcEquipLeftAligned[] = {
  PROC_SET_NAME(&EquipLeftAlignedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipLeftAligned_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's equipped weapon's name left-aligned within a space.
   */

  char* equipString;

  struct EquipLeftAlignedProc* equipProc = (struct EquipLeftAlignedProc*)ProcFind(ProcEquipLeftAligned);
  if ( equipProc == NULL )
  {
    equipProc = (struct EquipLeftAlignedProc*)START_PROC(ProcEquipLeftAligned, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if ( udp->weapon != 0 )
  {
    equipString = GetItemName(udp->weapon);
    equipString = String_ExpandTactName();

    Text_SetParameters(&equipProc->equipText, 0, EQUIP_COLOR);
    Text_DrawString(&equipProc->equipText, equipString);

    Text_Display(&equipProc->equipText, gUiTmScratchA + TILEMAP_INDEX(EQUIP_X, EQUIP_Y));
  }

  return;
}
