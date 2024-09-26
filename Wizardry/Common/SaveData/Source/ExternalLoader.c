#include "common-chax.h"
#include "save-data.h"

void MSA_ExternalSaver(u8 *dst, const u32 size)
{
	if (gpMsa_Saver)
		gpMsa_Saver(dst, size);
}

void MSA_ExternalLoader(u8 *src, const u32 size)
{
	if(gpMsa_Loader)
		gpMsa_Loader(src, size);
}

void MSU_ExternalSaver(u8 *dst, const u32 size)
{
	if (gpMsu_Saver)
		gpMsu_Saver(dst, size);
}

void MSU_ExternalLoader(u8 *src, const u32 size)
{
	if(gpMsu_Loader)
		gpMsu_Loader(src, size);
}
