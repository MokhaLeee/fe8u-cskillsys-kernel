
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const u16 GETilemap_1[];

#define CallARM_FillTileRect BgMap_ApplyTsa


void GetTilemapPalette(struct Unit* unit, int paletteID)
{
  /* Get a tilemap palette based on unit faction.
   */

  u16* palette = NULL;

  switch ( UNIT_FACTION(unit) )
  {
    case FACTION_BLUE:
      palette = gPal_PlayerInterface_Blue;
      break;

    case FACTION_RED:
      palette = gPal_PlayerInterface_Red;
      break;

    case FACTION_GREEN:
      palette = gPal_PlayerInterface_Green;
      break;

    default:
      palette = gPal_PlayerInterface_Blue;
      break;

  }
  ApplyPalette(palette, paletteID);

  return;
}


void TilemapKaitou_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Copy the tilemap and its palette.
   */

  CallARM_FillTileRect(gUiTmScratchB, GETilemap_1, TILEREF(0, GE_TILEMAP_PALETTE));
  GetTilemapPalette(udp->unit, GE_TILEMAP_PALETTE);

  return;
}
