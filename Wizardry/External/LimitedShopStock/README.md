## Limited Shop Stock
This hack allows you to limit how many of each item a player may purchase.  
These limits apply globally to every shop in a chapter, and may be set by the chapter.

### Requirements for Installation
- Expanded Modular Save (To save current shop suppy).

### Installation Instructions
- Include `Installer.event` in your buildfile.

- Assign a textID to `OutOfStockTextBase`. Some defaults are listed in `DefaultText.txt`.

- In the Expanded Modular Save folder, add an entry under `gSuspendSaveChunks` for the shop stock save/load routines.
    ```
    // Shop supply
    DeclSaveChunk(<StartAddress>, StockedItemLimit, SU_SaveShopStock, SU_LoadShopStock)
    ```
    Change `<StartAddress>` to a value appropriate for your own Expanded Modular Save configuration.  
    `StockedItemLimit` is defined in `Installer.event`.

### Usage Instructions
To create a list of limited items, use the `StockEntry` macro under a set label to add entres and `StockListEnd` to terminate the list.
```
ALIGN 4
Ch2ShopStock:
StockEntry(SlimSword, 2)
StockEntry(IronLance, 3)
StockListEnd //TERMINATOR
```

To assign a list of limited items to a chapter, use the `SetChapterShopStock` macro.
```
SetChapterShopStock(2, Ch2ShopStock)
```

To load a chapter's shop list, use the `InitShopStock` function.  
This can be called as an ASMC in a chapter's starting event or be set to run at the start of a chapter via a hook.

### Credits
Thanks to the people who work on the [FE8U decomp](https://github.com/FireEmblemUniverse/fireemblem8u) for making this possible.
