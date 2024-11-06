
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

u8* GetGenericMiniMugGfx(int portraitID);
void LoadGenericMiniMugPalette(int portraitID, int paletteID);

#ifndef MINIMUG_WIDTH
  #define MINIMUG_WIDTH  4
  #endif // MINIMUG_WIDTH

#ifndef MINIMUG_HEIGHT
  #define MINIMUG_HEIGHT 4
  #endif // MINIMUG_HEIGHT

#define MINIMUG_GRAPHICS_SIZE (MINIMUG_WIDTH * MINIMUG_HEIGHT * CHR_SIZE)


void DecompressMinimugGraphics(int portraitID, int baseTile, int paletteID)
{
  /* Copies the minimug's graphics and palette using a buffer.
   *
   * This uses RegisterTileGraphics to copy the graphics instead
   * of writing directly to VRAM. This is essentially a C implementation
   * of Huichelaar's minimug palette fix (https://github.com/Huichelaar/FE8U_MinimugPalette)
   */

  void* graphicsDest = (void*)(((int)((baseTile * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  if ( IS_GENERIC_MINIMUG(portraitID) )
  {
    u8* graphics = GetGenericMiniMugGfx(portraitID);

    RegisterTileGraphics(graphics, graphicsDest, MINIMUG_GRAPHICS_SIZE);
    LoadGenericMiniMugPalette(portraitID, paletteID);
  }
  else
  {
    const struct PortraitData* info = GetPortraitData(portraitID);

    Decompress(info->pMiniPortraitGraphics, (void*)(gMinimugTempBuffer));
    RegisterTileGraphics(gMinimugTempBuffer, graphicsDest, MINIMUG_GRAPHICS_SIZE);
    ApplyPalette(info->pPortraitPalette, paletteID);
  }

  return;
}
