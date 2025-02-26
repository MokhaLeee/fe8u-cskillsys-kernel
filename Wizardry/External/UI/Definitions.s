.include "fe8.s" 

.macro SET_FUNC name, value
	.global \name
	.type   \name, function
	.set    \name, \value
.endm

.macro SET_DATA name, value
	.global \name
	.type   \name, object
	.set    \name, \value
.endm

SET_DATA sUiFrameImage, 0x085B6470
SET_DATA poinGfx_PlayerInterfaceFontTiles, 0x0808d0ac
SET_DATA poinGfx_PlayerInterfaceNumbers, 0x0808d0b4
SET_DATA poinUnknown_08A1B730, 0x0809A8D8 
SET_DATA poinImg_PrepItemUseScreen, 0x0809c7fc 
SET_DATA poinUnknown_08A1B1FC, 0x08098468
SET_DATA poinImg_PrepPopupWindow, 0x08098cb4
SET_DATA poinUnknown_0880210C, 0x08051cbc
SET_DATA poinUnknown_088021C0, 0x08051cc0
SET_DATA poinUnknown_08802274, 0x08051378
SET_DATA poinUnknown_08802348, 0x08051380
SET_DATA poinUnknown_08802428, 0x0805141c
SET_DATA poinUnknown_08802508, 0x08052170
SET_DATA poinImg_08801C14, 0x08051f68
SET_DATA poinUnknown_08802558, 0x08052178
SET_DATA poinImg_EfxLeftNameBox, 0x08051fc8
SET_DATA poinImg_EfxLeftItemBox, 0x08052028
SET_DATA poinImg_EfxRightNameBox, 0x08052088
SET_DATA poinImg_EfxRightItemBox, 0x08052164
SET_DATA poinImg_ShopGoldBox, 0x080b4e70
SET_DATA poinUnknown_08A1B658, 0x08095eac
SET_DATA poinUnknown_08A1B698, 0x08095e3c
SET_DATA poinUnknown_08A1B7C8, 0x0809a8e0


SET_DATA Img_ShopGoldBox, 0x089ad9f8
SET_DATA gBanimFactionPal, 0x203e114
SET_DATA gBanimFont, 0x02017648
SET_DATA gBanimText, 0x02017660
SET_DATA gBanimValid, 0x203e104
SET_DATA Img_08801C14, 0x8801C14
SET_DATA gUnknown_08802598, 0x8802598
SET_FUNC ApplyUnitSpritePalettes, 0x8026629 


@ division & other libgcc functions
SET_FUNC __aeabi_idiv,    __divsi3
SET_FUNC __aeabi_idivmod, __modsi3
SET_FUNC Div, __divsi3
SET_FUNC Mod, __modsi3

