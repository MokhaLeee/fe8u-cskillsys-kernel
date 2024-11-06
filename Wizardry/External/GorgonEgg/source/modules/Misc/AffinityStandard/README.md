
AffinityStandard

---

This module draws an icon for the selected unit's affinity. Nothing is drawn if the unit has no affinity.

```

Definitions:

  * `AFFINITY_X`: The X coordinate of the icon in pixels
  * `AFFINITY_Y`: The Y coordinate of the icon in pixels
  * `AFFINITY_PAGE`: If the icon rework is installed, this is the sheet index for affinity icons
  * `AFFINITY_BASE_TILE`: The VRAM tile index of the first tile of the affinity icon
  * `AFFINITY_PALETTE`: The PalRAM index of the affinity's palette

Special Definitions:

  * `gAffinityWeaponRankPalette`: This is a C declaration of the affinity/weapon rank palette

Special ASM Definitions:

  * `GetUnitAffinityIcon`: This defines the ROM location of the function that gets a unit's affinity icon
  * `gAffinityWeaponRankPalette`: This defines the ROM location of the palette
