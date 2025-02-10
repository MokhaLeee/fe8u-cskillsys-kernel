#pragma once

#include "common-chax.h"
#include "list-verify.h"

#define STAT_BAR_MAX_INDENTIFIER 41

typedef const struct HelpBoxInfo _DECL_INFO;
extern _DECL_INFO *const RTextPageSupport;

extern struct {
	u8 talkee;
	u8 unitpage_max;

	u8 toggle_timer : 7;
	u8 toggle : 1;

	u8 _pad_[1];
} gStatScreenStExpa;

#define STATSCREEN_TOGGLE_DURATION 20

#define ModifyTextPal(bank, color) \
{ \
	switch (color) { \
	case 0 ... 4: \
		bank  = 0; \
		break; \
	case 5 ... 9: \
		bank  = 8; \
		color = color - 5; \
		break; \
	case 10 ... 14: \
		bank  = 9; \
		color = color - 10; \
		break; \
	default: \
		bank  = 0; \
		color = 0; \
	} \
}

/* unit page */
int GetUnitBattleAmt(struct Unit *unit);
u8 SortMax(const u8 *buf, int size);
u8 GetTalkee(struct Unit *unit);
void InstallExpandedTextPal(void);
void ResetActiveFontPal(void);
int GetTextColorFromGrowth(int growth);
void HbPopuplate_Page1TrvTalk(struct HelpBoxProc *proc);
void PutDrawTextRework(struct Text *text, u16 *tm, int color, int x, int tile_width, char const *str);
void DrawStatWithBarReworkExt(int num, int x, int y, u16 *tm, int base, int total, int max, int max_ref);
void DrawStatWithBarRework(int num, int x, int y, u16 *tm1, u16 *tm2, int base, int total, int max);
void StartUnitScreenHelp(int pageid, struct Proc *proc);
void ToggleUnitLeftPage(bool toggle);

void DisplayPage_WithBWL(void);
void DisplayPage_WithLeadership(void);

extern _DECL_INFO *const RTextPageUnit_WithBWL;
extern _DECL_INFO *const RTextPageUnit_WithLeadership;

/* skill page */
void StartSkillScreenHelp(int pageid, struct Proc *proc);
void HbPopuplate_SkillPageCommon(struct HelpBoxProc *proc);
void HbRedirect_SkillPageCommon(struct HelpBoxProc *proc);
void HbPopuplate_ArtPageCommon(struct HelpBoxProc *proc);
void HbRedirect_ArtPageCommon(struct HelpBoxProc *proc);
void HbRedirect_ArtPageOnlyAlly(struct HelpBoxProc *proc);

void DrawSkillPage_MokhaPlanA(void);
void DrawSkillPage_MokhaPlanB(void);

extern _DECL_INFO *const RTextSkillPage_MokhaPlanA;
extern _DECL_INFO *const RTextSkillPage_MokhaPlanB;

/**
 * Item page
 */
enum { CHAX_ITEM_PAGE_AMT = UNIT_ITEM_COUNT };

struct ItemPageEnt {
	u16 item;
	u8 slot, color;
};

struct ItemPageList {
	struct UnitListHeader header;
	struct ItemPageEnt ent[CHAX_ITEM_PAGE_AMT];
};

extern EWRAM_OVERLAY(0) struct ItemPageList gItemPageList;

struct ItemPageList *GetUnitItemPageList(struct Unit *unit);

extern u8 const *const gpTSA_ItemPageSubfix;
extern u8 const *const gpTSA_ItemEquipLine;
extern struct SSTextDispInfo const *const gpPage1TextInfo;
