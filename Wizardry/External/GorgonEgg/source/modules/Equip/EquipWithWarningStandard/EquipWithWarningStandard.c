
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipWithWarningStandardProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipWithWarningStandardProc_Name[] = "EquipWithWarningStandard";


const struct ProcInstruction ProcEquipWithWarningStandard[] = {
  PROC_SET_NAME(&EquipWithWarningStandardProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipWithWarningStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's equipped weapon's name centered within a space, with a durability warning.
   */

  int padding, color;
  char* equipString;

  struct EquipWithWarningStandardProc* equipProc = (struct EquipWithWarningStandardProc*)ProcFind(ProcEquipWithWarningStandard);
  if ( equipProc == NULL )
  {
    equipProc = (struct EquipWithWarningStandardProc*)START_PROC(ProcEquipWithWarningStandard, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if ( udp->weapon != 0 )
  {
    equipString = GetItemName(udp->weapon);
    equipString = String_ExpandTactName();

    padding = Text_GetStringTextCenteredPos((EQUIP_WIDTH * 8), equipString);

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
