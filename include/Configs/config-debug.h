#ifndef CONFIG_DEBUG_H
#define CONFIG_DEBUG_H

/* Basic switch to debug */
// #define CONFIG_USE_DEBUG

#ifdef CONFIG_USE_DEBUG
	/* Enable a skill anim at all time */
	// #define CONFIG_DEBUG_EFXSKILL

	/* Auto load 254 skills on loading unit */
	#define CONFIG_DEBUG_UNIT_LOAD_SKILL

	#define CONFIG_DEBUG_CHAPTER

#endif /* CONFIG_USE_DEBUG */

#define CONFIG_FORCE_PRIENT_ERROR 1

#endif /* CONFIG_DEBUG_H */
