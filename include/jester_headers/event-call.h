#pragma once

#include "global.h"
#include "proc.h"
#include "event.h"
#include "uimenu.h"
#include "bmtrap.h"

extern u8 gUdefCnt;
extern struct UnitDefinition gUdefs[];
extern CONST_DATA u8 gAutoUdefJids[2][4];

// extern CONST_DATA EventScr EventScr_WM_FadeCommon[];
// extern CONST_DATA EventScr EventScrWM_Prologue_Beginning[];
// extern CONST_DATA EventScr EventScrWM_Prologue_ChapterIntro[];
// extern CONST_DATA EventScr EventScrWM_Ch1_Beginning[];
// extern CONST_DATA EventScr EventScrWM_Ch1_ChapterIntro[];
// extern CONST_DATA EventScr EventScrWM_Ch2_BeginningTutorial[];
// extern CONST_DATA EventScr EventScrWM_Ch2_Beginning[];
// extern CONST_DATA EventScr EventScrWM_Ch2_ChapterIntro[];

extern const EventScr EventScrWM_Ch1_ENDING[];
extern const EventScr EventScrWM_Ch2_SET_NODE[];
extern const EventScr EventScrWM_Ch2_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch3_SET_NODE[];
extern const EventScr EventScrWM_Ch3_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch4_SET_NODE[];
extern const EventScr EventScrWM_Ch4_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch5_SET_NODE[];
extern const EventScr EventScrWM_Ch5_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch6_SET_NODE[];
extern const EventScr EventScrWM_Ch6_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch7_SET_NODE[];
extern const EventScr EventScrWM_Ch7_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch8_SET_NODE[];
extern const EventScr EventScrWM_Ch8_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch9_SET_NODE[];
extern const EventScr EventScrWM_Ch9_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch10_SET_NODE[];
extern const EventScr EventScrWM_Ch10_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch11_SET_NODE[];
extern const EventScr EventScrWM_Ch11_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch12_SET_NODE[];
extern const EventScr EventScrWM_Ch12_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch13_SET_NODE[];
extern const EventScr EventScrWM_Ch13_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch14_SET_NODE[];
extern const EventScr EventScrWM_Ch14_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch15_SET_NODE[];
extern const EventScr EventScrWM_Ch15_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch16_SET_NODE[];
extern const EventScr EventScrWM_Ch16_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch17_SET_NODE[];
extern const EventScr EventScrWM_Ch17_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch18_SET_NODE[];
extern const EventScr EventScrWM_Ch18_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch19_SET_NODE[];
extern const EventScr EventScrWM_Ch29_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch20_SET_NODE[];
extern const EventScr EventScrWM_Ch20_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch21_SET_NODE[];
extern const EventScr EventScrWM_Ch21_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch22_SET_NODE[];
extern const EventScr EventScrWM_Ch22_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch23_SET_NODE[];
extern const EventScr EventScrWM_Ch23_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch24_SET_NODE[];
extern const EventScr EventScrWM_Ch24_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch25_SET_NODE[];
extern const EventScr EventScrWM_Ch25_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch26_SET_NODE[];
extern const EventScr EventScrWM_Ch26_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch27_SET_NODE[];
extern const EventScr EventScrWM_Ch27_TRAVEL_TO_NODE[];
extern const EventScr EventScrWM_Ch28_SET_NODE[];
extern const EventScr EventScrWM_Ch28_TRAVEL_TO_NODE[];


extern u8 Event19_Checks(struct EventEngineProc * proc);