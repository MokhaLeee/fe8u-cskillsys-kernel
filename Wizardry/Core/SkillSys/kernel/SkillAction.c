#include "common-chax.h"
#include "skill-system.h"

bool ActionExecSkill(ProcPtr proc)
{
	u16 sid = gActionData.unk08;

	if (!COMMON_SKILL_VALID(sid) || gpSkillActionFuncTable[sid] == NULL)
		return false;

	return gpSkillActionFuncTable[sid](proc);
}
