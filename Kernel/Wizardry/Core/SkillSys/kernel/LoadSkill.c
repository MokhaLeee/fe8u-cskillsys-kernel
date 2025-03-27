#include "common-chax.h"
#include "bwl.h"
#include "kernel-lib.h"
#include "skill-system.h"

#define LOCAL_TRACE 0

/**
 * Slot ops
 */
STATIC_DECLAR void SortRamSkillList(struct Unit *unit)
{
	int i, cnt = 0;
	u8 *list = UNIT_RAM_SKILLS(unit);
	u8 *buf = gGenericBuffer;

	WARN_GENERIC_BUF_USED;

	memset(buf, 0, UNIT_RAM_SKILLS_LEN);

	for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
		if (EQUIPE_SKILL_VALID(list[i]))
			buf[cnt++] = list[i];

	memcpy(list, buf, UNIT_RAM_SKILLS_LEN);

	WARN_GENERIC_BUF_RELEASED;
}

inline int GetSkillSlot(struct Unit *unit, int sid)
{
	int i;
	const int cnt = RAM_SKILL_LEN_EXT;
	u8 *list = UNIT_RAM_SKILLS(unit);

	for (i = 0; i < cnt; i++)
		if (list[i] == sid)
			return i;

	return -1;
}

inline int GetFreeSkillSlot(struct Unit *unit)
{
	int i;
	const int cnt = RAM_SKILL_LEN_EXT;
	u8 *list = UNIT_RAM_SKILLS(unit);

	for (i = 0; i < cnt; i++)
		if (!EQUIPE_SKILL_VALID(list[i]))
			return i;

	return -1;
}

bool CanRemoveSkill(struct Unit *unit, const u16 sid)
{
	int i;
	u8 *list = UNIT_RAM_SKILLS(unit);

	if (!EQUIPE_SKILL_VALID(sid))
		return false;

	for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++)
		if (sid == list[i])
			return true;

	return false;
}

int RemoveSkill(struct Unit *unit, const u16 sid)
{
	int i;
	u8 *list = UNIT_RAM_SKILLS(unit);

	if (!EQUIPE_SKILL_VALID(sid))
		return -1;

	for (i = 0; i < UNIT_RAM_SKILLS_LEN; i++) {
		if (sid == list[i]) {
			list[i] = 0;
			SortRamSkillList(unit);
			ResetSkillLists();
			return 0;
		}
	}
	return -1;
}

int AddSkill(struct Unit *unit, const u16 sid)
{
	int slot;
	u8 *list = UNIT_RAM_SKILLS(unit);

	if (sid >= MAX_GENERIC_SKILL_NUM)
		return -1;

	LearnSkill(unit, sid);

	slot = GetSkillSlot(unit, sid);
	if (slot != -1)
		return 0;

	slot = GetFreeSkillSlot(unit);
	if (slot == -1)
		return -1;

	list[slot] = sid;
	ResetSkillLists();
	return 0;
}

void UnitAutoLoadSkills(struct Unit *unit)
{
	int i;
	int level_p, level_j;
	const struct SkillPreloadPConf *pConf = &gpSkillPreloadPData[UNIT_CHAR_ID(unit)];
	const struct SkillPreloadJConf *jConf = &gpSkillPreloadJData[UNIT_CLASS_ID(unit)];

	if (!UNIT_IS_VALID(unit))
		return;

	level_j = k_udiv(unit->level, 5) * 5;
	level_p = k_udiv(unit->level + GetUnitHiddenLevel(unit), 5) * 5;

	LIMIT_AREA(level_j, 0, UNIT_LEVEL_MAX_RE);
	LIMIT_AREA(level_p, 0, UNIT_RECORDED_LEVEL_MAX);

	while (level_p >= 0) {
		for (i = 0; i < 5; i++)
			AddSkill(unit, pConf->skills[level_p + i]);

		level_p = level_p - 5;
	}

	while (level_j >= 0) {
		for (i = 0; i < 5; i++)
			AddSkill(unit, jConf->skills[level_j + i]);

		level_j = level_j - 5;
	}

#ifdef CONFIG_FIT_OLD_SKILLSYS_LIST
	FebList_LoadUnitSkill(unit);
#endif /* FIT_OLD_SKILLSYS_LIST */

	/* For debug, we enable unit learn all of skills */
	if (gpKernelDesigerConfig->debug_autoload_skills) {
		for (i = 1; i < 254; i++)
			LearnSkill(unit, i);
	}
}

/**
 * Lvup
 */
STATIC_DECLAR void TryAddSkillLvupPConf(struct Unit *unit, int level)
{
	int i;
	u16 sid;

	const struct SkillPreloadPConf *pConf = &gpSkillPreloadPData[UNIT_CHAR_ID(unit)];
	int _level = k_udiv(level, 5) * 5;

	for (i = 0; i < 5; i++) {
		sid = pConf->skills[_level + i];

		if (EQUIPE_SKILL_VALID(sid)) {
			AddSkill(unit, sid);
			PushSkillListStack(sid);
		}
	}
}

STATIC_DECLAR void TryAddSkillLvupJConf(struct Unit *unit, int level)
{
	int i;
	u16 sid;

	const struct SkillPreloadJConf *jConf = &gpSkillPreloadJData[UNIT_CLASS_ID(unit)];
	int _level = k_udiv(level, 5) * 5;

	for (i = 0; i < 5; i++) {
		sid = jConf->skills[_level + i];

		if (EQUIPE_SKILL_VALID(sid)) {
			AddSkill(unit, sid);
			PushSkillListStack(sid);
		}
	}
}

void TryAddSkillLvup(struct Unit *unit, int level)
{
	int _level;

	if (!UNIT_IS_VALID(unit))
		return;

#ifdef CONFIG_FIT_OLD_SKILLSYS_LIST
	FebList_LvupAddSkill(unit, level);
#endif

	_level = level;
	if (k_umod(_level, 5) == 0)
		TryAddSkillLvupJConf(unit, _level);

	_level = level + GetUnitHiddenLevel(unit);
	if (k_umod(_level, 5) == 0)
		TryAddSkillLvupPConf(unit, _level);
}

/**
 * Promotion
 */
void TryAddSkillPromotion(struct Unit *unit, int jid)
{
	int i;
	u16 sid;

	const struct SkillPreloadJConf *jConf = &gpSkillPreloadJData[jid];

	ResetPopupSkillStack();

	if (!UNIT_IS_VALID(unit))
		return;

#ifdef CONFIG_FIT_OLD_SKILLSYS_LIST
	FebList_PromotionAddSkill(unit);
#endif

	for (i = 0; i < 5; i++) {
		sid = jConf->skills[0 + i];

		if (EQUIPE_SKILL_VALID(sid)) {
			AddSkill(unit, sid);
			PushSkillListStack(sid);
		}
	}

	/**
	 * Append const list
	 */
	sid = gpConstSkillTable_Job[jid * 2 + 0];
	if (COMMON_SKILL_VALID(sid))
		PushSkillListStack(sid);

	sid = gpConstSkillTable_Job[jid * 2 + 1];
	if (COMMON_SKILL_VALID(sid))
		PushSkillListStack(sid);
}
