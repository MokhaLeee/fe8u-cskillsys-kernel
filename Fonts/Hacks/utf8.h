#pragma once

#include "common-chax.h"

int GetChLenUtf8(const char * str);
int DecodeUtf8(const char * str, u32 * unicode_out, int * len);
