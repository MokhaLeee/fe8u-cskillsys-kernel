#include "gbafe.h"

typedef struct ShopStockEntry ShopStockEntry;
typedef struct ProcShop ProcShop;

// struct ProcShop {
//     /* 00 */ PROC_HEADER;

//     /* 2C */ struct Unit* unit;
//     /* 30 */ u16 shopItems[20];

//     /* 58 */ u16 unk_58;

//     /* 5A */ u8 shopItemCount;
//     /* 5B */ u8 unitItemCount;
//     /* 5C */ u8 curIndex;
//     /* 5D */ u8 unk_5d;
//     /* 5E */ u8 unk_5e;
//     /* 5F */ u8 unk_5f; // maybe top visible item in menu?
//     /* 60 */ u8 unk_60;
//     /* 61 */ u8 shopType;
//     /* 62 */ u8 helpTextActive;

//     /* 64 */ s16 unk_64;
//     /* 66 */ s16 unk_66;
//     /* 68 */ s16 unk_68;
// };

struct ShopStockEntry {
    u8 item;
    u8 stock;
};

//Vanilla functions
extern void sub_8004B88(u16* a, int b, int c);
extern u16 GetItemPurchasePrice(struct Unit* unit, int item);
extern void DisplayUiHand(int x, int y);
extern void StartItemHelpBox(int x, int y, int item);
extern void DisplayShopUiArrows(void);
extern s8 sub_80B56CC(void);
extern void CloseHelpBox(void);
// extern void StartShopDialogue(int baseMsgId, struct ProcShop* proc);
extern void sub_80B5604(void);
extern int sub_80B5698(void);
extern u16 sub_80B568C(void);
extern u16 sub_80B56A8(void);
extern void SetTalkNumber(int number);
extern void sub_8014B88(int, int);
extern void sub_80B4F90(ProcShop*);
// extern void UpdateShopItemCounts(ProcShop*);
extern void DisplayGoldBoxText(u16* tm);
extern s8 IsItemSellable(int);
extern u16 GetItemSellPrice(int item);
extern void DrawUiNumber(u16* out, int color, int number);
extern void Text_InsertString(struct Text*, int xPos, int color, const char* str); //! FE8U = 0x8004481

extern u16 OutOfStockTextBase;

// extern TextHandle gShopItemTexts[6];

extern u8 (*gCurrentShopStocks)[];
extern ShopStockEntry* gShopStockTable[];

extern void InitShopStock();
