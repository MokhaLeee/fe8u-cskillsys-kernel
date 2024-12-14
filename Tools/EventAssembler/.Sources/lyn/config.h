#ifndef LYN_SYS_CONFIG
#define LYN_SYS_CONFIG

#define HAVE_BYTESWAP_H
/* #undef WORDS_BIGENDIAN */
/* #undef ENABLE_NLS */

#define PROJECT_NAME    "lyn"
#define PROJECT_VERSION "2.5.3"

#if defined ENABLE_NLS
#	include <libintl.h>
#	include <clocale>
#else
#	include <clocale>
#	define _(aString) (aString)
#endif

#endif // LYN_SYS_CONFIG
