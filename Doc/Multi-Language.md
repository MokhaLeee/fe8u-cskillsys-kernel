This repo support multi-language. For now, Chinese is supported, if you want to use Chinese version of kernel, follow these steps before `make`:

- Uncomment `include Fonts/fonts.mk` in [makefile](../makefile):
- Uncomment `CONFIG_LANG_CHINESE` in [configs.h](../Configs/configs.h)
- Uncomment `#include "Fonts/Fonts.event"` in [main.event](../main.event)
- Modify on [TextMain.txt](../Contants/Texts/Source/TextMain.txt), use `texts-cn.txt` rather than `texts.txt`
