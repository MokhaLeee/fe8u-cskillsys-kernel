
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction ProcGE[];

struct EquipIfPlayerStandardProc {
  /* 00 */ PROC_FIELDS;
  /* 2C */ struct TextHandle equipText;

};


const char EquipIfPlayerStandardProc_Name[] = "EquipIfPlayerStandard";


const struct ProcInstruction ProcEquipIfPlayerStandard[] = {
  PROC_SET_NAME(&EquipIfPlayerStandardProc_Name),
  PROC_SLEEP(0),

  PROC_WHILE_EXISTS(ProcGE),

  PROC_END,
};


void EquipIfPlayerStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws a player unit's equipped weapon's name centered within a space.
   */

  int padding;
  char* equipString;

  struct EquipIfPlayerStandardProc* equipProc = (struct EquipIfPlayerStandardProc*)ProcFind(ProcEquipIfPlayerStandard);
  if ( equipProc == NULL )
  {
    equipProc = (struct EquipIfPlayerStandardProc*)START_PROC(ProcEquipIfPlayerStandard, proc);
    Text_InitDB(&equipProc->equipText, EQUIP_WIDTH);
  }

  Text_Clear(&equipProc->equipText);

  if ( (UNIT_FACTION(udp->unit) == FACTION_BLUE) && (udp->weapon != 0) )
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
