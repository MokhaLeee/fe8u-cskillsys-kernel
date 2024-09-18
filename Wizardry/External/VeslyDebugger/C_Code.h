
#include "include/gbafe.h"

#define FORCE_DECLARE __attribute__((unused))

#define LYN_REPLACE_CHECK(name) static void const * const __attribute__((unused)) lyn_exists_check_ ## name = &name