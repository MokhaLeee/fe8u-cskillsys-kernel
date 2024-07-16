#include "common-chax.h"
#include "stat-screen.h"
#include "strmag.h"
#include "lvup.h"
#include "bwl.h"
#include "debuff.h"
#include "kernel-glyph.h"
#include "constants/texts.h"


extern u8 gStatScreenStatsGrowthsToggle; 
void ToggleStatScreenStatsGrowthsDisplay(void) 
{ 
    if (gStatScreenStatsGrowthsToggle) 
    { 
        gStatScreenStatsGrowthsToggle = false; 
    } 
    else 
    { 
        gStatScreenStatsGrowthsToggle = true; 
    } 
} 

void StatScreen_OnIdle(struct Proc* proc)
{
    struct Unit* unit;

    if (gKeyStatusPtr->newKeys & B_BUTTON)
    {
        gLCDControlBuffer.dispcnt.bg0_on = TRUE;
        gLCDControlBuffer.dispcnt.bg1_on = FALSE;
        gLCDControlBuffer.dispcnt.bg2_on = TRUE;
        gLCDControlBuffer.dispcnt.bg3_on = TRUE;
        gLCDControlBuffer.dispcnt.obj_on = TRUE;

        SetBlendConfig(3, 0, 0, 0x10);

        SetBlendTargetA(0, 0, 0, 0, 0);
        SetBlendBackdropA(1);

        // TODO: ResetBackdropColor macro?
        gPaletteBuffer[PAL_BACKDROP_OFFSET] = 0;
        EnablePaletteSync();

        Proc_Break(proc);

        PlaySoundEffect(0x6B); // TODO: song ids
    }

    else if (gKeyStatusPtr->repeatedKeys & DPAD_LEFT)
    {
        /* 
        The decomp does the following, but this use of % is not working properly when I build here.
        When switching pages it brings me to (numberOfPages + 1), crashing the game.
        gStatScreen.page = (gStatScreen.page + gStatScreen.pageAmt - 1) % gStatScreen.pageAmt;
        */
        if (!gStatScreen.page) 
        { 
            gStatScreen.page = gStatScreen.pageAmt - 1; 
        }
        else 
        {
            gStatScreen.page--; 
        }
        StartPageSlide(DPAD_LEFT, gStatScreen.page, proc);
        return;
    }

    else if (gKeyStatusPtr->repeatedKeys & DPAD_RIGHT)
    {
        gStatScreen.page++; 
        if (gStatScreen.page >= gStatScreen.pageAmt) 
        { 
            gStatScreen.page = 0; 
        }
        StartPageSlide(DPAD_RIGHT, gStatScreen.page, proc);
    }

    else if (gKeyStatusPtr->repeatedKeys & DPAD_UP)
    {
        unit = FindNextUnit(gStatScreen.unit, -1);
        StartUnitSlide(unit, -1, proc);
    }

    else if (gKeyStatusPtr->repeatedKeys & DPAD_DOWN)
    {
        unit = FindNextUnit(gStatScreen.unit, +1);
        StartUnitSlide(unit, +1, proc);
    }

    else if ((gKeyStatusPtr->repeatedKeys & A_BUTTON) && (gStatScreen.unit->rescue))
    {
        unit = GetUnit(gStatScreen.unit->rescue);
        StartUnitSlide(unit, (gStatScreen.unit->state & US_RESCUING) ? +1 : -1, proc);
    }

    else if (gKeyStatusPtr->newKeys & R_BUTTON)
    {
        Proc_Goto(proc, 0); // TODO: label name
        StartStatScreenHelp(gStatScreen.page, proc);
    }
    
    else if (gKeyStatusPtr->newKeys & SELECT_BUTTON)
    {
        if (!gStatScreen.page) 
        { // stats page only 
            Proc_Goto(proc, 0); 
            ToggleStatScreenStatsGrowthsDisplay(); 
            StatScreen_Display(proc);
        } 
    }
}

