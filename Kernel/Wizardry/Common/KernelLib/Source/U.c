#include "common-chax.h"
#include "kernel-lib.h"

bool IsPointer(uintptr_t a)
{
	return a >= 0x08000000 && a < 0x0A000000;
}

bool IsPointerOrNULL(uintptr_t a)
{
	return a == 0 || IsPointer(a);
}
