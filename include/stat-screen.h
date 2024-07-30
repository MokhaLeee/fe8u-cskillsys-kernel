#pragma once

#include "common-chax.h"

#define STAT_BAR_MAX_INDENTIFIER 41

typedef const struct HelpBoxInfo _DECL_INFO;
extern _DECL_INFO * const RTextPageUnit, * const RTextPageSupport;

extern struct {
    u8 talkee;
    u8 toggle : 1;
    u8 _pad_[2];
} gStatScreenStExpa;

void ResetActiveFontPal(void);
int GetTextColorFromGrowth(int growth);
void DisplayHpGrowthValue(void);
void DisplayHpBmValue(void);

#define ModifyTextPal(bank, color)      \
    switch (color) {                    \
    case 0 ... 4:                       \
        bank  = 0;                      \
        break;                          \
    case 5 ... 9:                       \
        bank  = 8;                      \
        color = color - 5;              \
        break;                          \
    case 10 ... 14:                     \
        bank  = 9;                      \
        color = color - 10;             \
        break;                          \
    default:                            \
        bank  = 0;                      \
        color = 0;                      \
    }

void HbPopuplate_Page1TrvTalk(struct HelpBoxProc * proc);
void StartSkillScreenHelp(int pageid, struct Proc * proc);
void HbPopuplate_SkillPageCommon(struct HelpBoxProc * proc);
void HbRedirect_SkillPageCommon(struct HelpBoxProc * proc);
void HbPopuplate_ArtPageCommon(struct HelpBoxProc * proc);
void HbRedirect_ArtPageCommon(struct HelpBoxProc * proc);

void DrawSkillPage_MokhaPlanA(void);
extern _DECL_INFO * const RTextSkillPage_MokhaPlanA;

void DrawSkillPage_MokhaPlanB(void);
extern _DECL_INFO * const RTextSkillPage_MokhaPlanB;
