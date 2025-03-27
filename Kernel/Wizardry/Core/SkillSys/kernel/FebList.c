#include "common-chax.h"
#include "skill-system.h"

#define PROMOTION_LEVEL_MAX 20


/**
 * Arguments: r0 = Unit, r1 = level from, r2 = level to, r3 = output buffer, [sp] = max length of out buffer
 * Returns:   r0 = count
 *
 * Given a Unit (or Battle Unit) and a level range, gets what skills that unit would learn within that range.
 * Skills are stored in a null-termnated list of address given in r3.
 *
 * Used for, Skill learning on level, Skill learning on unit loading, and Skill learning on promotion.
 *
 * Skill learning on unit loading gives ranges [0xFE, 0xFF] and [0, level], and cares for all returned skills.
 * Skill learning on level up gives range [old level, new level] and only cares for the first returned skill.
 * Skill learning on promotion gives range [0, 1] and only cares for the first returned skill.
 *
 * This is an improvement over the skill system before march 2019, where each cases were handled separately
 * As it makes it easier to hack in (or out) methods of defining level-up skills.
 */
int GetUnitLevelSkills(struct Unit *unit, int level_from, int level_to, u8 *out_buffer, int max_len)
{
	const struct FebListEnt *it;
	bool promoted = !!(UNIT_CATTRIBUTES(unit) & CA_PROMOTED);
	int count = 0;

	#define APPEND_LIST(_sid) \
	do { \
		out_buffer[count++] = _sid; \
		if (count >= max_len) \
			goto goto_end; \
	} while (0)

	/**
	 * Character skills
	 */
	it = gpCharLevelUpSkillTable[UNIT_CHAR_ID(unit)];
	if (it) {
		for (; !(it->sid == 0 && it->level == 0); it++) {
			int it_level = it->level;

			if (promoted) {
				/**
				 * if char is promoted, check if level is 0xFF (on-load) and if skill is learned before promotion level
				 * this is to allow prepromotes to load skills they would have learned as a non promoted class
				 */
				if (it_level <= PROMOTION_LEVEL_MAX) {
					APPEND_LIST(it->sid);
					continue;
				} else
					it_level = it->level - PROMOTION_LEVEL_MAX;
			}

			if (it_level > level_from && it_level <= level_to)
				APPEND_LIST(it->sid);
		}
	}

	/**
	 * Class skills
	 */
	it = gpClassLevelUpSkillTable[UNIT_CLASS_ID(unit)];
	if (it) {
		for (; !(it->sid == 0 && it->level == 0); it++) {
			int it_level = it->level;

			if (it_level != 0xFF)
				it_level = LOAD_JOB_SKILL_LEVEL(it_level);

			if (it_level > level_from && it_level <= level_to) {
				switch (LOAD_JOB_SKILL_CONFIG(it->level)) {
				case LOAD_JOB_SKILL_CONFIG_PLAYER_ONLY:
					if (UNIT_FACTION(unit) == FACTION_BLUE)
						APPEND_LIST(it->sid);
					break;

				case LOAD_JOB_SKILL_CONFIG_ENEMY_ONLY:
					if (UNIT_FACTION(unit) == FACTION_RED)
						APPEND_LIST(it->sid);
					break;

				case LOAD_JOB_SKILL_CONFIG_WO_TUTORIAL:
					if (TUTORIAL_MODE())
						APPEND_LIST(it->sid);
					break;

				case LOAD_JOB_SKILL_CONFIG_HARD_MODE_ONLY:
					if (gPlaySt.chapterStateBits & PLAY_FLAG_HARD)
						APPEND_LIST(it->sid);
					break;

				case LOAD_JOB_SKILL_CONFIG_NONE:
				default:
					APPEND_LIST(it->sid);
					break;
				}
			}
		}
	}

goto_end:
	out_buffer[count] = 0;
	return count;

	#undef APPEND_LIST
}

int GetInitialSkillList(struct Unit *unit, u8 *out_buffer, int max_len)
{
	int len1, len2;

	len1 =  GetUnitLevelSkills(unit, 0xFE, 0xFF, out_buffer, max_len);
	len2 = len1 + GetUnitLevelSkills(unit, 0, unit->level, out_buffer + len1, max_len - len1);

	return len2;
}

int GetLevelUpSkillList(struct Unit *unit, int level, u8 *out_buffer, int max_len)
{
	return GetUnitLevelSkills(unit, level - 1, level, out_buffer, max_len);
}

int GetPromotionSkillList(struct Unit *unit, u8 *out_buffer, int max_len)
{
	return GetUnitLevelSkills(unit, 0, 1, out_buffer, max_len);
}

/**
 * API
 */
void FebList_LoadUnitSkill(struct Unit *unit)
{
	u8 buf[DEFAULT_LEVEL_SKILLS_BUF_MAX_LEN];
	int i, len = GetInitialSkillList(unit, buf, DEFAULT_LEVEL_SKILLS_BUF_MAX_LEN);

	for (i = 0; i < len; i++)
		AddSkill(unit, buf[i]);
}

void FebList_LvupAddSkill(struct Unit *unit, int level)
{
	u8 buf[DEFAULT_LEVEL_SKILLS_BUF_MAX_LEN];
	int i, len = GetLevelUpSkillList(unit, level, buf, DEFAULT_LEVEL_SKILLS_BUF_MAX_LEN);

	for (i = 0; i < len; i++) {
		AddSkill(unit, buf[i]);
		PushSkillListStack(buf[i]);
	}
}

void FebList_PromotionAddSkill(struct Unit *unit)
{
	u8 buf[DEFAULT_LEVEL_SKILLS_BUF_MAX_LEN];
	int i, len = GetPromotionSkillList(unit, buf, DEFAULT_LEVEL_SKILLS_BUF_MAX_LEN);

	for (i = 0; i < len; i++) {
		AddSkill(unit, buf[i]);
		PushSkillListStack(buf[i]);
	}
}
