
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipWithWarningLeftAlignedProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipWithWarningLeftAlignedProc_Name[] = "EquipWithWarningLeftAligned";


const struct ProcInstruction ProcEquipWithWarningLeftAligned[] = {
  PROC_SET_NAME(&EquipWithWarningLeftAlignedProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipWithWarningLeftAligned_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's equipped weapon's name left-aligned within a space, with a low durability warning.
   */

  int color;
  char* equipString;

  struct EquipWithWarningLeftAlignedProc* equipProc = (struct EquipWithWarningLeftAlignedProc*)ProcFind(ProcEquipWithWarningLeftAligned);
  if ( equipProc == NULL )
  {
    equipProc = (struct EquipWithWarningLeftAlignedProc*)START_PROC(ProcEquipWithWarningLeftAligned, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if ( udp->weapon != 0 )
  {
    equipString = GetItemName(udp->weapon);
    equipString = String_ExpandTactName();

    color = EQUIP_COLOR;

    if ( (GetItemAttributes(udp->weapon) & IA_UNBREAKABLE) == 0 )
    {
      if ( GetItemUses(udp->weapon) == 0 )
        color = EQUIP_WARNING_COLOR;

      else if ( ((GetItemUses(udp->weapon) * 100) / GetItemMaxUses(udp->weapon)) <= EQUIP_WARNING_THRESHOLD )
        color = EQUIP_WARNING_COLOR;
    }

    Text_SetParameters(&equipProc->equipText, 0, color);
    Text_DrawString(&equipProc->equipText, equipString);

    Text_Display(&equipProc->equipText, gUiTmScratchA + TILEMAP_INDEX(EQUIP_X, EQUIP_Y));
  }

  return;
}
