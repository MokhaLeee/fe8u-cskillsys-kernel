#include "common-chax.h"
#include "combat-art.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/combat-arts.h"

const u8 gCombatArtSkillTable[MAX_SKILL_NUM + 1] = {
#if defined(SID_COMBAT_Grounder) && (COMMON_SKILL_VALID(SID_COMBAT_Grounder))
	[SID_COMBAT_Grounder] = CID_Grounder,
#endif

#if defined(SID_COMBAT_BaneOfMonsters) && (COMMON_SKILL_VALID(SID_COMBAT_BaneOfMonsters))
	[SID_COMBAT_BaneOfMonsters] = CID_BaneOfMonsters,
#endif

#if defined(SID_COMBAT_Hexblade) && (COMMON_SKILL_VALID(SID_COMBAT_Hexblade))
	[SID_COMBAT_Hexblade] = CID_Hexblade,
#endif

#if defined(SID_COMBAT_FoudroyantStrike) && (COMMON_SKILL_VALID(SID_COMBAT_FoudroyantStrike))
	[SID_COMBAT_FoudroyantStrike] = CID_FoudroyantStrike,
#endif

#if defined(SID_COMBAT_TempestLance) && (COMMON_SKILL_VALID(SID_COMBAT_TempestLance))
	[SID_COMBAT_TempestLance] = CID_TempestLance,
#endif

#if defined(SID_COMBAT_SwiftStrikes) && (COMMON_SKILL_VALID(SID_COMBAT_SwiftStrikes))
	[SID_COMBAT_SwiftStrikes] = CID_SwiftStrikes,
#endif

#if defined(SID_COMBAT_FrozenLance) && (COMMON_SKILL_VALID(SID_COMBAT_FrozenLance))
	[SID_COMBAT_FrozenLance] = CID_FrozenLance,
#endif

#if defined(SID_COMBAT_Vengeance) && (COMMON_SKILL_VALID(SID_COMBAT_Vengeance))
	[SID_COMBAT_Vengeance] = CID_Vengeance,
#endif

#if defined(SID_COMBAT_HelmSplitter) && (COMMON_SKILL_VALID(SID_COMBAT_HelmSplitter))
	[SID_COMBAT_HelmSplitter] = CID_HelmSplitter,
#endif

#if defined(SID_COMBAT_LightningAxe) && (COMMON_SKILL_VALID(SID_COMBAT_LightningAxe))
	[SID_COMBAT_LightningAxe] = CID_LightningAxe,
#endif

#if defined(SID_COMBAT_WarMasterStrike) && (COMMON_SKILL_VALID(SID_COMBAT_WarMasterStrike))
	[SID_COMBAT_WarMasterStrike] = CID_WarMasterStrike,
#endif

#if defined(SID_COMBAT_CurvedShot) && (COMMON_SKILL_VALID(SID_COMBAT_CurvedShot))
	[SID_COMBAT_CurvedShot] = CID_CurvedShot,
#endif

#if defined(SID_COMBAT_Deadeye) && (COMMON_SKILL_VALID(SID_COMBAT_Deadeye))
	[SID_COMBAT_Deadeye] = CID_Deadeye,
#endif

#if defined(SID_COMBAT_Encloser) && (COMMON_SKILL_VALID(SID_COMBAT_Encloser))
	[SID_COMBAT_Encloser] = CID_Encloser,
#endif

#if defined(SID_COMBAT_EncloserPlus) && (COMMON_SKILL_VALID(SID_COMBAT_EncloserPlus))
	[SID_COMBAT_EncloserPlus] = CID_EncloserPlus,
#endif

#if defined(SID_COMBAT_MonsterBlast) && (COMMON_SKILL_VALID(SID_COMBAT_MonsterBlast))
	[SID_COMBAT_MonsterBlast] = CID_MonsterBlast,
#endif

#if defined(SID_COMBAT_SchismShot) && (COMMON_SKILL_VALID(SID_COMBAT_SchismShot))
	[SID_COMBAT_SchismShot] = CID_SchismShot,
#endif

#if defined(SID_COMBAT_BreakShot) && (COMMON_SKILL_VALID(SID_COMBAT_BreakShot))
	[SID_COMBAT_BreakShot] = CID_BreakShot,
#endif

#if defined(SID_COMBAT_BreakShotPlus) && (COMMON_SKILL_VALID(SID_COMBAT_BreakShotPlus))
	[SID_COMBAT_BreakShotPlus] = CID_BreakShotPlus,
#endif

#if defined(SID_COMBAT_WaningShot) && (COMMON_SKILL_VALID(SID_COMBAT_WaningShot))
	[SID_COMBAT_WaningShot] = CID_WaningShot,
#endif

#if defined(SID_COMBAT_WaningShotPlus) && (COMMON_SKILL_VALID(SID_COMBAT_WaningShotPlus))
	[SID_COMBAT_WaningShotPlus] = CID_WaningShotPlus,
#endif

#if defined(SID_COMBAT_HuntersVolley) && (COMMON_SKILL_VALID(SID_COMBAT_HuntersVolley))
	[SID_COMBAT_HuntersVolley] = CID_HuntersVolley,
#endif

#if defined(SID_COMBAT_Galeforce) && (COMMON_SKILL_VALID(SID_COMBAT_Galeforce))
	[SID_COMBAT_Galeforce] = CID_Galeforce,
#endif

#if defined(SID_COMBAT_Gamble) && (COMMON_SKILL_VALID(SID_COMBAT_Gamble))
	[SID_COMBAT_Gamble] = CID_Gamble,
#endif

#if defined(SID_COMBAT_Detonate) && (COMMON_SKILL_VALID(SID_COMBAT_Detonate))
	[SID_COMBAT_Detonate] = CID_Detonate,
#endif

#if defined(SID_COMBAT_BloodTribute) && (COMMON_SKILL_VALID(SID_COMBAT_BloodTribute))
	[SID_COMBAT_BloodTribute] = CID_BloodTribute,
#endif

#if defined(SID_COMBAT_CrimsonStrike) && (COMMON_SKILL_VALID(SID_COMBAT_CrimsonStrike))
	[SID_COMBAT_CrimsonStrike] = CID_CrimsonStrike,
#endif

#if defined(SID_COMBAT_VitalReckoning) && (COMMON_SKILL_VALID(SID_COMBAT_VitalReckoning))
	[SID_COMBAT_VitalReckoning] = CID_VitalReckoning,
#endif
};
