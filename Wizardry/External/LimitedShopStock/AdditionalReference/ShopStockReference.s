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

@ Shop stuff
SET_FUNC ShopProc_Loop_BuyKeyHandler, 0x80B44A9
SET_FUNC sub_8004B88, 0x8004B89
SET_FUNC DrawShopItemPriceLine, 0x80B5165
SET_FUNC GetItemPurchasePrice, 0x80B5221
SET_FUNC DisplayUiHand, 0x804E79D
SET_FUNC StartItemHelpBox, 0x8088E61
SET_FUNC DisplayShopUiArrows, 0x80B5385
SET_FUNC sub_80B56CC, 0x80B56CD
SET_FUNC CloseHelpBox, 0x8089019
SET_FUNC StartShopDialogue, 0x80B4169
SET_FUNC sub_80B5604, 0x80B5605
SET_FUNC sub_80B5698, 0x80B5699
SET_FUNC sub_80B568C, 0x80B568D
SET_FUNC sub_80B56A8, 0x80B56A9
SET_FUNC SetTalkNumber, 0x8008A19
SET_FUNC HandleShopBuyAction, 0x80B5441
SET_FUNC sub_8014B88, 0x8014B89
SET_FUNC sub_80B4F90, 0x80B4F91
SET_FUNC DisplayGoldBoxText, 0x80B4ED5
SET_FUNC UpdateShopItemCounts, 0x80B42B5
SET_FUNC sub_80B4F90, 0x80B4F91
SET_FUNC GetItemSellPrice, 0x80B5269
SET_FUNC IsItemSellable, 0x80B527D
SET_FUNC DrawShopItemLine, 0x80B51C1
