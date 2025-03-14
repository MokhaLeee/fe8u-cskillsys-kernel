#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

const SkillActionFunc_t gSkillActionFuncTable[MAX_SKILL_NUM + 1] = {
#if (defined(SID_HealingFocus) && COMMON_SKILL_VALID(SID_HealingFocus))
	[SID_HealingFocus] = Action_HealingFocus,
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
	[SID_Teleportation] = Action_Teleportation,
#endif

#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
	[SID_LightRune] = Action_LightRune,
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
	[SID_Mine] = Action_Mine,
#endif

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
	[SID_RallyDefense] = Action_Rally,
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
	[SID_RallyLuck] = Action_Rally,
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
	[SID_RallyMagic] = Action_Rally,
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
	[SID_RallyMovement] = Action_Rally,
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
	[SID_RallyResistance] = Action_Rally,
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
	[SID_RallySkill] = Action_Rally,
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
	[SID_RallySpeed] = Action_Rally,
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
	[SID_RallyStrength] = Action_Rally,
#endif

#if (defined(SID_RallySpectrum) && COMMON_SKILL_VALID(SID_RallySpectrum))
	[SID_RallySpectrum] = Action_Rally,
#endif

#if (defined(SID_GoddessDance) && COMMON_SKILL_VALID(SID_GoddessDance))
	[SID_GoddessDance] = Action_GoddessDance,
#endif

#if (defined(SID_Stride) && COMMON_SKILL_VALID(SID_Stride))
	[SID_Stride] = Action_Stride,
#endif

#if (defined(SID_Swarp) && COMMON_SKILL_VALID(SID_Swarp))
	[SID_Swarp] = Action_Swarp,
#endif

#if (defined(SID_Blacksmith) && COMMON_SKILL_VALID(SID_Blacksmith))
	[SID_Blacksmith] = Action_Blacksmith,
#endif

#if (defined(SID_DivineReprieve) && COMMON_SKILL_VALID(SID_DivineReprieve))
	[SID_DivineReprieve] = Action_DivineReprieve,
#endif
};
