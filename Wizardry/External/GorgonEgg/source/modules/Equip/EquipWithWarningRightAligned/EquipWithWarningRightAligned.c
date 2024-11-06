
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipWithWarningRightAlignedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipWithWarningRightAlignedProc_Name[] = "EquipWithWarningRightAligned";


const struct ProcInstruction ProcEquipWithWarningRightAligned[] = {
  PROC_SET_NAME(&EquipWithWarningRightAlignedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipWithWarningRightAligned_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's equipped weapon's name right-aligned within a space, with a low durability warning.
   */

  int padding, color;
  char* equipString;

  struct EquipWithWarningRightAlignedProc* equipProc = (struct EquipWithWarningRightAlignedProc*)ProcFind(ProcEquipWithWarningRightAligned);
  if (equipProc == NULL)
  {
    equipProc = (struct EquipWithWarningRightAlignedProc*)START_PROC(ProcEquipWithWarningRightAligned, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if (udp->weapon != 0)
  {
    equipString = GetItemName(udp->weapon);
    equipString = String_ExpandTactName();

    padding = (EQUIP_WIDTH * 8) - Text_GetStringTextWidth(equipString);

    color = EQUIP_COLOR;

    if ( (GetItemAttributes(udp->weapon) & IA_UNBREAKABLE) == 0 )
    {
      if ( GetItemUses(udp->weapon) == 0 )
        color = EQUIP_WARNING_COLOR;

      else if ( ((GetItemUses(udp->weapon) * 100) / GetItemMaxUses(udp->weapon)) <= EQUIP_WARNING_THRESHOLD )
        color = EQUIP_WARNING_COLOR;
    }

    Text_SetParameters(&equipProc->equipText, padding, color);
    Text_DrawString(&equipProc->equipText, equipString);

    Text_Display(&equipProc->equipText, gUiTmScratchA + TILEMAP_INDEX(EQUIP_X, EQUIP_Y));
  }

  return;
}
