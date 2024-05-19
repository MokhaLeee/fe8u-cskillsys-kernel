#ifndef CONFIG_DEBUG_H
#define CONFIG_DEBUG_H

/* Basic switch to debug */
// #define CONFIG_USE_DEBUG

#ifdef CONFIG_USE_DEBUG
    /* Enable a skill anim at all time */
    #define CONFIG_DEBUG_EFXSKILL
#endif /* CONFIG_USE_DEBUG */

#endif /* CONFIG_DEBUG_H */
