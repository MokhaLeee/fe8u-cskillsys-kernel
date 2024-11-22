#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "jester_headers/macros.h"
#include "jester_headers/soundtrack-ids.h"
#include "jester_headers/maps.h"
#include "jester_headers/flags.h"
#include "jester_headers/miscellaenous.h"
#include "jester_headers/event-call.h"
#include "EAstdlib.h"

const EventScr EventScrWM_Ch1_ENDING[] = {
    EVBIT_MODIFY(0x1)
    WM_SPAWNLORD(WM_MU_0, CHARACTER_EIRIKA, WM_NODE_BorderMulan)
    WM_CENTERCAMONLORD(WM_MU_0)
    WM_LOADLOCATION2(WM_NODE_BorderMulan)
    WmEvtSetNodeStateNot2(WM_NODE_BorderMulan) // ENOSUPP in EAstdlib
    WM_SETCAM(0, 12)
    WM_FADEOUT(0)
    WM_TEXTDECORATE // WaitFade
    EVBIT_MODIFY(0x0)
    STAL(20)
    WM_CREATENEXTDESTINATION
    WM_WAITFORFX
    STAL(40)
    WM_DRAWPATH(WM_PATH_00)
    STAL(70)
    MUSC(BGM_TREASURED_MEMORIES)
    WM_SHOWTEXTWINDOW(40, 0x0001)
    WM_WAITFORTEXT
    WM_TEXTSTART
    WM_TEXT(Chapter_01_WM, 0)
    TEXTEND
    STAL(20)
    WM_MOVESPRITETO(WM_MU_0, 0x0000, WM_NODE_BorderMulan, WM_NODE_CastleFrelia, 200, 40)
    TEXTCONT
    TEXTEND
    WM_WAITFORSPRITES(WM_MU_ANY)
    WM_REMOVETEXT
    STAL(2)
    STAL(20)
    SKIPWN
    ENDA
};

const EventScr EventScrWM_Ch2_SET_NODE[] = {
    EVBIT_MODIFY(0x1)
    WM_SPAWNLORD(WM_MU_0, CHARACTER_EIRIKA, WM_NODE_CastleFrelia)
    WM_CENTERCAMONLORD(WM_MU_0)
    WM_FADEOUT(0)
    WM_TEXTDECORATE // WaitFade
    EVBIT_MODIFY(0x0)
    STAL(20)
    WM_LOADLOCATION2(WM_NODE_Ide)
    WM_SETDESTINATION(WM_NODE_Ide)
    //WM_CREATENEXTDESTINATION
    WM_WAITFORFX
    STAL(40)
    WM_DRAWPATH(WM_PATH_01)
    STAL(70)
    MUSC(0x4)
    SVAL(EVT_SLOT_2, EventScrWM_Ch2_BeginningTutorial)
    CALL(EventScr_CallOnTutorialMode)
    WM_MAKELORDVISIBLE(WM_MU_0)
    EVBIT_MODIFY(0x1)
    CALL(EventScr_RemoveBGIfNeeded)
    ENDA
};

const EventScr EventScrWM_Ch2_TRAVEL_TO_NODE[] = {
    STAL(6)
    WM_MOVECAM(-1, -1, 0, 32, 45, 0)
    WM_WAITFORCAM
    CALL(EventScr_WM_FadeCommon)
    WM_TEXTSTART
    WM_SHOWPORTRAIT(0, 0x0052, 0x0534, 0) // HAYDEN
    STAL(6)
    WM_TEXT(Chapter_02_WM, 0)
    TEXTEND
    WM_CLEARPORTRAIT(0, 0x0634, 0)
    STAL(60)
    WM_SHOWPORTRAIT(0, 0x002C, 0x0534, 0) // TANA
    STAL(6)
    STAL(46)
    WM_SHOWPORTRAIT(1, 0x0007, 0x02BC, 0) // MOULDER
    STAL(6)
    TEXTCONT
    TEXTEND
    WM_CLEARPORTRAIT(0, 0x0634, 0)
    STAL(46)
    WM_CLEARPORTRAIT(1, 0x01BC, 0)
    STAL(60)
    WM_SHOWPORTRAIT(0, 0x0002, 0x02BC, 0) //EIRIKA
    STAL(6)
    TEXTCONT
    TEXTEND
    STAL(46)
    WM_CLEARPORTRAIT(0, 0x01BC, 0)
    STAL(46)
    WM_SHOWDRAWNMAP(0, -8, 0x3)
    STAL(120)
    WM_MOVECAM2(0, -8, 0, 48, 70, 0)
    STAL(85)
    WM_PLACEDOT(0, 0, WM_NODE_Renvall2, 1)
    TEXTCONT
    TEXTEND
    STAL(30)
    WM_FXCLEAR1(-0x1)
    WM_FXCLEAR2(-0x1)
    WmEvtRemoveBigMap // WM_WAITFORFXCLEAR1
    WmEvtWaitBigMapRemove // WM_WAITFORFXCLEAR2
    TEXTCONT
    TEXTEND
    WM_MOVESPRITETO(WM_MU_0, 0x0002, WM_NODE_CastleFrelia, WM_NODE_Ide, 200, 30)
    TEXTCONT
    TEXTEND
    WM_WAITFORSPRITES(WM_MU_ANY)
    WM_REMOVETEXT
    STAL(2)
    STAL(20)
    FADI(16)
    ENDA
};

const EventScr EventScrWM_Ch3_SET_NODE[] = {
    EVBIT_MODIFY(0x1)
    WM_SPAWNLORD(WM_MU_0, CHARACTER_EIRIKA, WM_NODE_Ide)
    WM_CENTERCAMONLORD(WM_MU_0)
    WM_FADEOUT(0)
    WM_TEXTDECORATE // WaitFade
    EVBIT_MODIFY(0x0)
    STAL(20)
    WM_LOADLOCATION2(WM_NODE_BorgoRidge)
    WM_SETDESTINATION(WM_NODE_BorgoRidge)
    //WM_CREATENEXTDESTINATION
    WM_WAITFORFX
    STAL(40)
    WM_DRAWPATH(WM_PATH_02)
    STAL(70)
    MUSC(0x4)
    SVAL(EVT_SLOT_2, EventScrWM_Ch3_BeginningTutorial)
    CALL(EventScr_CallOnTutorialMode)
    WM_MAKELORDVISIBLE(WM_MU_0)
    EVBIT_MODIFY(0x1)
    CALL(EventScr_RemoveBGIfNeeded)
    ENDA
};

const EventScr EventScrWM_Ch3_TRAVEL_TO_NODE[] = {
    STAL(6)
    WM_MOVECAM(-1, -1, 0, 72, 60, 0)
    WM_WAITFORCAM
    CALL(EventScr_WM_FadeCommon)
    WM_TEXTSTART
    PUTSPRITE(WM_MU_3, CLASS_EIRIKA_LORD, WM_FACTION_BLUE, WM_NODE_Ide)
    WM_MAKELORDVISIBLE(WM_MU_3)
    WM_MAKELORDDISAPPEAR(WM_MU_0)
    PUTSPRITE(WM_MU_2, CLASS_THIEF, WM_FACTION_GREEN, WM_NODE_Ide)
    WM_MOVESPRITETO(WM_MU_2, 0x0003, WM_NODE_Ide, WM_NODE_BorgoRidge, -4, 0)
    WM_WAITFORSPRITES(WM_MU_2)
    WM_REMSPRITE(WM_MU_2)
    WM_SHOWPORTRAIT(0, 0x0002, 0x0534, 0) // Eirika
    STAL(6)
    STAL(46)
    WM_TEXT(Chapter_03_WM, 0)
    TEXTEND
    WM_MAKELORDVISIBLE(WM_MU_0)
    WM_REMSPRITE(WM_MU_3)
    WM_CLEARPORTRAIT(0, 0x0634, 0)
    STAL(46)
    PUTSPRITE(WM_MU_2, CLASS_BRIGAND, WM_FACTION_RED, WM_NODE_BorgoRidge)
    WM_FADEINSPRITE(WM_MU_2, 60)
    WM_SHOWPORTRAIT(0, 0x0033, 0x02BC, 0) // Bazba
    STAL(6)
    STAL(46)
    TEXTCONT
    TEXTEND
    WM_CLEARPORTRAIT(0, 0x01BC, 0)
    STAL(46)
    WM_FADEOUTSPRITE(WM_MU_2, 60)
    WM_MOVESPRITETO(WM_MU_0, 0x0000, WM_NODE_Ide, WM_NODE_BorgoRidge, -4, 0)
    WM_WAITFORSPRITES(WM_MU_ANY)
    WM_WAITFORSPRITELOAD
    WM_REMSPRITE(WM_MU_2)
    TEXTCONT
    TEXTEND
    WM_REMOVETEXT
    STAL(2)
    STAL(20)
    FADI(16)
    ENDA
};