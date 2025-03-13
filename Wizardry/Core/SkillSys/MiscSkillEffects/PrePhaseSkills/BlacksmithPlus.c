#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"


bool PrePhase_BlacksmithPlus(ProcPtr proc)
{
#if (defined(SID_BlacksmithPlus) && COMMON_SKILL_VALID(SID_BlacksmithPlus))
    int i;

    for (i = gPlaySt.faction + 1; i <= (gPlaySt.faction + GetFactionUnitAmount(gPlaySt.faction)); ++i)
    {
        struct Unit *unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

        if (SkillTester(unit, SID_BlacksmithPlus))
        {
            int weaponSlot = GetUnitEquippedWeaponSlot(unit);
            u16 weapon = GetUnitEquippedWeapon(unit);

            u8 uses = (weapon >> 8 );      // Extract uses
            u8 itemID = weapon & 0xFF; // Extract itemID

            int itemMaxUses = GetItemMaxUses((int)itemID);
            if (itemMaxUses == uses)
                return false;
            
            uses += 1;
        
            u16 newValue = (uses << 8) | itemID;  // Recombine into a u16 value

            unit->items[weaponSlot] = newValue;
        }
    }
#endif
    return false;
}
