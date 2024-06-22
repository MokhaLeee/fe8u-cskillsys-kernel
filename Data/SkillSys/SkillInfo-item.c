#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "constants/gfx.h"

const struct SkillInfo gSkillInfos_Item[0x100] = {

#if (defined(SID_RuinedBladePlus) && ITEM_SKILL_VALID(SID_RuinedBladePlus))
    [SKILL_INDEX_REAL(SID_RuinedBladePlus)] = {
        .desc = MSG_SKILL_RuinedBladePlus,
        .icon = GFX_SkillIcon_RuinedBlade,
    },
#endif

#if (defined(SID_COMBAT_Grounder) && ITEM_SKILL_VALID(SID_COMBAT_Grounder))
    [SKILL_INDEX_REAL(SID_COMBAT_Grounder)] = {
        .desc = MSG_SKILL_COMBAT_Grounder,
        .icon = GFX_CombatArtIcon_SwordArtAtk,
    },
#endif

#if (defined(SID_COMBAT_BaneOfMonsters) && ITEM_SKILL_VALID(SID_COMBAT_BaneOfMonsters))
    [SKILL_INDEX_REAL(SID_COMBAT_BaneOfMonsters)] = {
        .desc = MSG_SKILL_COMBAT_BaneOfMonsters,
        .icon = GFX_CombatArtIcon_SwordArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Hexblade) && ITEM_SKILL_VALID(SID_COMBAT_Hexblade))
    [SKILL_INDEX_REAL(SID_COMBAT_Hexblade)] = {
        .desc = MSG_SKILL_COMBAT_Hexblade,
        .icon = GFX_CombatArtIcon_SwordArtMag,
    },
#endif

#if (defined(SID_COMBAT_FoudroyantStrike) && ITEM_SKILL_VALID(SID_COMBAT_FoudroyantStrike))
    [SKILL_INDEX_REAL(SID_COMBAT_FoudroyantStrike)] = {
        .desc = MSG_SKILL_COMBAT_FoudroyantStrike,
        .icon = GFX_CombatArtIcon_SwordArtCrit,
    },
#endif

#if (defined(SID_COMBAT_TempestLance) && ITEM_SKILL_VALID(SID_COMBAT_TempestLance))
    [SKILL_INDEX_REAL(SID_COMBAT_TempestLance)] = {
        .desc = MSG_SKILL_COMBAT_TempestLance,
        .icon = GFX_CombatArtIcon_LanceArtAtk,
    },
#endif

#if (defined(SID_COMBAT_SwiftStrikes) && ITEM_SKILL_VALID(SID_COMBAT_SwiftStrikes))
    [SKILL_INDEX_REAL(SID_COMBAT_SwiftStrikes)] = {
        .desc = MSG_SKILL_COMBAT_SwiftStrikes,
        .icon = GFX_CombatArtIcon_LanceArtAtk,
    },
#endif

#if (defined(SID_COMBAT_FrozenLance) && ITEM_SKILL_VALID(SID_COMBAT_FrozenLance))
    [SKILL_INDEX_REAL(SID_COMBAT_FrozenLance)] = {
        .desc = MSG_SKILL_COMBAT_FrozenLance,
        .icon = GFX_CombatArtIcon_LanceArtMag,
    },
#endif

#if (defined(SID_COMBAT_Vengeance) && ITEM_SKILL_VALID(SID_COMBAT_Vengeance))
    [SKILL_INDEX_REAL(SID_COMBAT_Vengeance)] = {
        .desc = MSG_SKILL_COMBAT_Vengeance,
        .icon = GFX_CombatArtIcon_LanceArtCrit,
    },
#endif

#if (defined(SID_COMBAT_HelmSplitter) && ITEM_SKILL_VALID(SID_COMBAT_HelmSplitter))
    [SKILL_INDEX_REAL(SID_COMBAT_HelmSplitter)] = {
        .desc = MSG_SKILL_COMBAT_HelmSplitter,
        .icon = GFX_CombatArtIcon_AxeArtAtk,
    },
#endif

#if (defined(SID_COMBAT_LightningAxe) && ITEM_SKILL_VALID(SID_COMBAT_LightningAxe))
    [SKILL_INDEX_REAL(SID_COMBAT_LightningAxe)] = {
        .desc = MSG_SKILL_COMBAT_LightningAxe,
        .icon = GFX_CombatArtIcon_AxeArtMag,
    },
#endif

#if (defined(SID_COMBAT_WarMasterStrike) && ITEM_SKILL_VALID(SID_COMBAT_WarMasterStrike))
    [SKILL_INDEX_REAL(SID_COMBAT_WarMasterStrike)] = {
        .desc = MSG_SKILL_COMBAT_WarMasterStrike,
        .icon = GFX_CombatArtIcon_AxeArtCrit,
    },
#endif

#if (defined(SID_COMBAT_CurvedShot) && ITEM_SKILL_VALID(SID_COMBAT_CurvedShot))
    [SKILL_INDEX_REAL(SID_COMBAT_CurvedShot)] = {
        .desc = MSG_SKILL_COMBAT_CurvedShot,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Deadeye) && ITEM_SKILL_VALID(SID_COMBAT_Deadeye))
    [SKILL_INDEX_REAL(SID_COMBAT_Deadeye)] = {
        .desc = MSG_SKILL_COMBAT_Deadeye,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif

#if (defined(SID_COMBAT_Encloser) && ITEM_SKILL_VALID(SID_COMBAT_Encloser))
    [SKILL_INDEX_REAL(SID_COMBAT_Encloser)] = {
        .desc = MSG_SKILL_COMBAT_Encloser,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_EncloserPlus) && ITEM_SKILL_VALID(SID_COMBAT_EncloserPlus))
    [SKILL_INDEX_REAL(SID_COMBAT_EncloserPlus)] = {
        .desc = MSG_SKILL_COMBAT_EncloserPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_MonsterBlast) && ITEM_SKILL_VALID(SID_COMBAT_MonsterBlast))
    [SKILL_INDEX_REAL(SID_COMBAT_MonsterBlast)] = {
        .desc = MSG_SKILL_COMBAT_MonsterBlast,
        .icon = GFX_CombatArtIcon_BowArtCrit,
    },
#endif

#if (defined(SID_COMBAT_SchismShot) && ITEM_SKILL_VALID(SID_COMBAT_SchismShot))
    [SKILL_INDEX_REAL(SID_COMBAT_SchismShot)] = {
        .desc = MSG_SKILL_COMBAT_SchismShot,
        .icon = GFX_CombatArtIcon_BowArtCrit,
    },
#endif

#if (defined(SID_COMBAT_BreakShot) && ITEM_SKILL_VALID(SID_COMBAT_BreakShot))
    [SKILL_INDEX_REAL(SID_COMBAT_BreakShot)] = {
        .desc = MSG_SKILL_COMBAT_BreakShot,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_BreakShotPlus) && ITEM_SKILL_VALID(SID_COMBAT_BreakShotPlus))
    [SKILL_INDEX_REAL(SID_COMBAT_BreakShotPlus)] = {
        .desc = MSG_SKILL_COMBAT_BreakShotPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_WaningShot) && ITEM_SKILL_VALID(SID_COMBAT_WaningShot))
    [SKILL_INDEX_REAL(SID_COMBAT_WaningShot)] = {
        .desc = MSG_SKILL_COMBAT_WaningShot,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_WaningShotPlus) && ITEM_SKILL_VALID(SID_COMBAT_WaningShotPlus))
    [SKILL_INDEX_REAL(SID_COMBAT_WaningShotPlus)] = {
        .desc = MSG_SKILL_COMBAT_WaningShotPlus,
        .icon = GFX_CombatArtIcon_BowArtDebuff,
    },
#endif

#if (defined(SID_COMBAT_HuntersVolley) && ITEM_SKILL_VALID(SID_COMBAT_HuntersVolley))
    [SKILL_INDEX_REAL(SID_COMBAT_HuntersVolley)] = {
        .desc = MSG_SKILL_COMBAT_HuntersVolley,
        .icon = GFX_CombatArtIcon_BowArtAtk,
    },
#endif
};
