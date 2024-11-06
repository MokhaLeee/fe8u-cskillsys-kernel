
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipStandardProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipStandardProc_Name[] = "EquipStandard";


const struct ProcInstruction ProcEquipStandard[] = {
  PROC_SET_NAME(&EquipStandardProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a unit's equipped weapon's name centered within a space.
   */

  int padding;
  char* equipString;

  struct EquipStandardProc* equipProc = (struct EquipStandardProc*)ProcFind(ProcEquipStandard);
  if ( equipProc == NULL )
  {
    equipProc = (struct EquipStandardProc*)START_PROC(ProcEquipStandard, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if ( udp->weapon != 0 )
  {
    equipString = GetItemName(udp->weapon);
    equipString = String_ExpandTactName();

    padding = Text_GetStringTextCenteredPos((EQUIP_WIDTH * 8), equipString);

    Text_SetParameters(&equipProc->equipText, padding, EQUIP_COLOR);
    Text_DrawString(&equipProc->equipText, equipString);

    Text_Display(&equipProc->equipText, gUiTmScratchA + TILEMAP_INDEX(EQUIP_X, EQUIP_Y));
  }

  return;
}
