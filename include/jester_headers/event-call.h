#pragma once

#include "global.h"
#include "proc.h"
#include "event.h"
#include "uimenu.h"
#include "bmtrap.h"

extern u8 gUdefCnt;
extern struct UnitDefinition gUdefs[];
extern CONST_DATA u8 gAutoUdefJids[2][4];

extern CONST_DATA EventScr EventScr_WM_FadeCommon[];
extern CONST_DATA EventScr EventScrWM_Prologue_Beginning[];
extern CONST_DATA EventScr EventScrWM_Prologue_ChapterIntro[];
extern CONST_DATA EventScr EventScrWM_Ch1_Beginning[];
extern CONST_DATA EventScr EventScrWM_Ch1_ChapterIntro[];
extern CONST_DATA EventScr EventScrWM_Ch2_BeginningTutorial[];
extern CONST_DATA EventScr EventScrWM_Ch2_Beginning[];
extern CONST_DATA EventScr EventScrWM_Ch2_ChapterIntro[];

extern const EventScr EventScrWM_Ch1_ENDING[];
extern const EventScr EventScrWM_Ch2_SET_NODE[];
extern const EventScr EventScrWM_Ch2_TRAVEL_TO_NODE[];

extern u8 Event19_Checks(struct EventEngineProc * proc);