#include "global.h"
#include "common-chax.h"
#include "combat-art.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

const u8 gCombatArtSkillTable[0x100] = {
    [SID_COMBAT_Grounder] = CID_Grounder,
    [SID_COMBAT_BaneOfMonsters] = CID_BaneOfMonsters,
    [SID_COMBAT_Hexblade] = CID_Hexblade,
    [SID_COMBAT_FoudroyantStrike] = CID_FoudroyantStrike,
    [SID_COMBAT_TempestLance] = CID_TempestLance,
    [SID_COMBAT_SwiftStrikes] = CID_SwiftStrikes,
    [SID_COMBAT_FrozenLance] = CID_FrozenLance,
    [SID_COMBAT_Vengeance] = CID_Vengeance,
    [SID_COMBAT_HelmSplitter] = CID_HelmSplitter,
    [SID_COMBAT_LightningAxe] = CID_LightningAxe,
    [SID_COMBAT_WarMasterStrike] = CID_WarMasterStrike,
    [SID_COMBAT_CurvedShot] = CID_CurvedShot,
    [SID_COMBAT_Deadeye] = CID_Deadeye,
    [SID_COMBAT_Encloser] = CID_Encloser,
    [SID_COMBAT_EncloserPlus] = CID_EncloserPlus,
    [SID_COMBAT_MonsterBlast] = CID_MonsterBlast,
    [SID_COMBAT_SchismShot] = CID_SchismShot,
    [SID_COMBAT_BreakShot] = CID_BreakShot,
    [SID_COMBAT_BreakShotPlus] = CID_BreakShotPlus,
    [SID_COMBAT_WaningShot] = CID_WaningShot,
    [SID_COMBAT_WaningShotPlus] = CID_WaningShotPlus,
    [SID_COMBAT_HuntersVolley] = CID_HuntersVolley,
};

FEB_IDENTIFIER(gCombatArtSkillTable);
