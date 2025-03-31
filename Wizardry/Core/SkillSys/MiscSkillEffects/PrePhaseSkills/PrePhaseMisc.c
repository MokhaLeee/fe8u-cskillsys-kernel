#include "common-chax.h"
#include "unit-expa.h"

bool PrePhase_ClearMiscUES(ProcPtr proc)
{
    int uid;
    struct Unit *unit;

    for (uid = 1; uid < 0xC0; uid++)
    {
        unit = GetUnit(uid);
        if (UNIT_IS_VALID(unit))
        {
#if defined(SID_GuardBearing) && (COMMON_SKILL_VALID(SID_GuardBearing))
            ClearBitUES(unit, UES_BIT_GUARDBEAR_SKILL_USED);
#endif
#if defined(SID_Tsuzuku) && (COMMON_SKILL_VALID(SID_Tsuzuku))
            ClearBitUES(unit, UES_BIT_TSZUKU_SKILL_USED);
#endif
#if defined(SID_Teleportation) && (COMMON_SKILL_VALID(SID_Teleportation))
            ClearBitUES(unit, UES_BIT_TELEPORTATION_SKILL_USED);
#endif
#if defined(SID_Swarp) && (COMMON_SKILL_VALID(SID_Swarp))
            ClearBitUES(unit, UES_BIT_SWARP_SKILL_USED);
#endif
#if defined(SID_Swap) && (COMMON_SKILL_VALID(SID_Swap))
            ClearBitUES(unit, UES_BIT_SWAP_SKILL_USED);
#endif
#if defined(SID_Turncoat) && (COMMON_SKILL_VALID(SID_Turncoat))
            ClearBitUES(unit, UES_BIT_TURNCOAT);
#endif
#if defined(SID_ShadowFlash) && (COMMON_SKILL_VALID(SID_ShadowFlash))
            ClearBitUES(unit, UES_BIT_SHADOWFLASH_SKILL_USED);
#endif
#if defined(SID_TwinCrests) && (COMMON_SKILL_VALID(SID_TwinCrests))
            ClearBitUES(unit, UES_BIT_TWIN_CRESTS_SKILL_USED);
#endif
#if defined(SID_DeathBlight) && (COMMON_SKILL_VALID(SID_DeathBlight))
            ClearBitUES(unit, UES_BIT_DEATHBLIGHT_SKILL_USED);
#endif

            /* We don't clear this bit as it's meant to be once per map */
            // ClearBitUES(unit, UES_BIT_LOADSTAR_RUSH_SKILL_USED);
            // ClearBitUES(unit, UES_BIT_WYVERN_CRASH_SKILL_USED);
        }
    }
    return false;
}
