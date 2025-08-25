#include "common-chax.h"

static size_t __debug_stdio_write(FILE *fp, const char *buf, size_t len)
{
	char out_buf[len + 1];

	memcpy(out_buf, buf, len);
	out_buf[len] = '\0';

	mgba_print(out_buf);
	NoCashGBAPrint(out_buf);
	return len;
}

static size_t __debug_stdio_read(FILE *fp, char *buf, size_t len)
{
	return len;
}

static const struct io_handle console_io = {
	.write = __debug_stdio_write,
	.read  = __debug_stdio_read,
};

static FILE const ___stdio_FILEs[3] = {
	[0] = { .io = &console_io }, // stdin
	[1] = { .io = &console_io }, // stdout
	[2] = { .io = &console_io }, // stderr
};

void io_init(void)
{
	memcpy(__stdio_FILEs, ___stdio_FILEs, sizeof(___stdio_FILEs));

	mgba_open();
}
