#ifndef CONFIG_DEBUG_H
#define CONFIG_DEBUG_H

/* Basic switch to debug */
// #define CONFIG_USE_DEBUG

#ifdef CONFIG_USE_DEBUG
    /* Enable a skill anim at all time */
    // #define CONFIG_DEBUG_EFXSKILL

    /* Enable a debug chapter on prologue */
    #define CONFIG_USE_DEBUG_CHAPTER
#endif /* CONFIG_USE_DEBUG */

#define CONFIG_FORCE_PRIENT_ERROR 1

#endif /* CONFIG_DEBUG_H */
