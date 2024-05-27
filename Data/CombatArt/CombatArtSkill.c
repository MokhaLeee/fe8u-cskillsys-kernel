#include "common-chax.h"
#include "combat-art.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

const u8 gCombatArtSkillTable[0x100] = {
#if defined(SID_COMBAT_Grounder) && (SID_COMBAT_Grounder < MAX_SKILL_NUM)
    [SID_COMBAT_Grounder] = CID_Grounder,
#endif

#if defined(SID_COMBAT_BaneOfMonsters) && (SID_COMBAT_BaneOfMonsters < MAX_SKILL_NUM)
    [SID_COMBAT_BaneOfMonsters] = CID_BaneOfMonsters,
#endif

#if defined(SID_COMBAT_Hexblade) && (SID_COMBAT_Hexblade < MAX_SKILL_NUM)
    [SID_COMBAT_Hexblade] = CID_Hexblade,
#endif

#if defined(SID_COMBAT_FoudroyantStrike) && (SID_COMBAT_FoudroyantStrike < MAX_SKILL_NUM)
    [SID_COMBAT_FoudroyantStrike] = CID_FoudroyantStrike,
#endif

#if defined(SID_COMBAT_TempestLance) && (SID_COMBAT_TempestLance < MAX_SKILL_NUM)
    [SID_COMBAT_TempestLance] = CID_TempestLance,
#endif

#if defined(SID_COMBAT_SwiftStrikes) && (SID_COMBAT_SwiftStrikes < MAX_SKILL_NUM)
    [SID_COMBAT_SwiftStrikes] = CID_SwiftStrikes,
#endif

#if defined(SID_COMBAT_FrozenLance) && (SID_COMBAT_FrozenLance < MAX_SKILL_NUM)
    [SID_COMBAT_FrozenLance] = CID_FrozenLance,
#endif

#if defined(SID_COMBAT_Vengeance) && (SID_COMBAT_Vengeance < MAX_SKILL_NUM)
    [SID_COMBAT_Vengeance] = CID_Vengeance,
#endif

#if defined(SID_COMBAT_HelmSplitter) && (SID_COMBAT_HelmSplitter < MAX_SKILL_NUM)
    [SID_COMBAT_HelmSplitter] = CID_HelmSplitter,
#endif

#if defined(SID_COMBAT_LightningAxe) && (SID_COMBAT_LightningAxe < MAX_SKILL_NUM)
    [SID_COMBAT_LightningAxe] = CID_LightningAxe,
#endif

#if defined(SID_COMBAT_WarMasterStrike) && (SID_COMBAT_WarMasterStrike < MAX_SKILL_NUM)
    [SID_COMBAT_WarMasterStrike] = CID_WarMasterStrike,
#endif

#if defined(SID_COMBAT_CurvedShot) && (SID_COMBAT_CurvedShot < MAX_SKILL_NUM)
    [SID_COMBAT_CurvedShot] = CID_CurvedShot,
#endif

#if defined(SID_COMBAT_Deadeye) && (SID_COMBAT_Deadeye < MAX_SKILL_NUM)
    [SID_COMBAT_Deadeye] = CID_Deadeye,
#endif

#if defined(SID_COMBAT_Encloser) && (SID_COMBAT_Encloser < MAX_SKILL_NUM)
    [SID_COMBAT_Encloser] = CID_Encloser,
#endif

#if defined(SID_COMBAT_EncloserPlus) && (SID_COMBAT_EncloserPlus < MAX_SKILL_NUM)
    [SID_COMBAT_EncloserPlus] = CID_EncloserPlus,
#endif

#if defined(SID_COMBAT_MonsterBlast) && (SID_COMBAT_MonsterBlast < MAX_SKILL_NUM)
    [SID_COMBAT_MonsterBlast] = CID_MonsterBlast,
#endif

#if defined(SID_COMBAT_SchismShot) && (SID_COMBAT_SchismShot < MAX_SKILL_NUM)
    [SID_COMBAT_SchismShot] = CID_SchismShot,
#endif

#if defined(SID_COMBAT_BreakShot) && (SID_COMBAT_BreakShot < MAX_SKILL_NUM)
    [SID_COMBAT_BreakShot] = CID_BreakShot,
#endif

#if defined(SID_COMBAT_BreakShotPlus) && (SID_COMBAT_BreakShotPlus < MAX_SKILL_NUM)
    [SID_COMBAT_BreakShotPlus] = CID_BreakShotPlus,
#endif

#if defined(SID_COMBAT_WaningShot) && (SID_COMBAT_WaningShot < MAX_SKILL_NUM)
    [SID_COMBAT_WaningShot] = CID_WaningShot,
#endif

#if defined(SID_COMBAT_WaningShotPlus) && (SID_COMBAT_WaningShotPlus < MAX_SKILL_NUM)
    [SID_COMBAT_WaningShotPlus] = CID_WaningShotPlus,
#endif

#if defined(SID_COMBAT_HuntersVolley) && (SID_COMBAT_HuntersVolley < MAX_SKILL_NUM)
    [SID_COMBAT_HuntersVolley] = CID_HuntersVolley,
#endif

};
