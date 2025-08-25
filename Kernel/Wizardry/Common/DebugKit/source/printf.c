#include "common-chax.h"
#include <stdarg.h>
#include "kernel-lib.h"

typedef int (*kprintf_putc_func)(const char *str, size_t len, void *arg);

#define LONGFLAG       0x00000001
#define LONGLONGFLAG   0x00000002
#define HALFFLAG       0x00000004
#define HALFHALFFLAG   0x00000008
#define SIZETFLAG      0x00000010
#define INTMAXFLAG     0x00000020
#define PTRDIFFFLAG    0x00000040
#define ALTFLAG        0x00000080
#define CAPSFLAG       0x00000100
#define SHOWSIGNFLAG   0x00000200
#define SIGNEDFLAG     0x00000400
#define LEFTFORMATFLAG 0x00000800
#define LEADZEROFLAG   0x00001000
#define BLANKPOSFLAG   0x00002000

#if 0
#define TRACE(x, ...) printf("[TRACE] "x, __VA_ARGS__)
#else
#define TRACE(x, ...)
#endif

static char *longlong_to_string(char *buf, unsigned long long u, size_t len, uint32_t flag, char *sign_char)
{
	size_t pos = len;
	bool negative = false;

	if ((flag & SIGNEDFLAG) && (long long)u < 0) {
		negative = true;
		u = -u;
	}

	buf[--pos] = 0;

	while (u >= 10) {
		buf[--pos] = '0' + k_umod64(u, 10);
		u = k_udiv64(u, 10);
	}

	buf[--pos] = u + '0';

	if (negative)
		*sign_char = '-';
	else if ((flag & SHOWSIGNFLAG))
		*sign_char = '+';
	else if ((flag & BLANKPOSFLAG))
		*sign_char = ' ';
	else
		*sign_char = '\0';

    return &buf[pos];
}

static const char hextable[16]      = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
static const char hextable_caps[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static char *longlong_to_hexstring(char *buf, unsigned long long u, size_t len, uint32_t flag)
{
	size_t pos = len;
	const char *table = (flag & CAPSFLAG) ? hextable_caps : hextable;

	buf[--pos] = '\0';

	do {
		buf[--pos] = table[u % 0x10];
		u = u / 0x10;
	} while (u != 0);

	return &buf[pos];
}

int kprintf_engine(kprintf_putc_func func, void *arg, const char *fmt, va_list ap)
{
	int err = 0;
	int chars_written = 0;

	#define OUTPUT_STR(__str, __len) \
	do { \
		err = func(__str, __len, arg); \
		if (err < 0) { goto exit; } \
		else { chars_written += err; } \
	} while (0)

	#define OUTPUT_CHR(__ch) \
	do { \
		char __out_chr_str[1] = { __ch }; \
		{ OUTPUT_STR(__out_chr_str, 1); } \
	} while (0)

	for (;;) {
		char c;
		char num_buffer[32];
		char sign_char = '\0';
		const char *s = fmt;
		int string_len = 0;
		int format_num = 0;
		uint32_t flags = 0;

		unsigned char arg_uc;
		const char *arg_s;
		unsigned long long arg_n;


		while ((c = *fmt++) != '\0') {
			if (c == '%')
				break;

			string_len++;
		}

		/* output the pure string */
		if (string_len > 0)
			OUTPUT_STR(s, string_len);

		if (c == '\0')
			break;

	next_format:
		c = *fmt++;
		if (c == '\0')
			break;

		TRACE("c=%c\n", c);

		switch (c) {
		case '0' ... '9':
			if (c == '0' && format_num == 0)
				flags |= LEADZEROFLAG;

			format_num *= 10;
			format_num += c - '0';
			goto next_format;

		case '.':
			goto next_format;

		case '%':
			OUTPUT_CHR('%');
			break;

		case 'c':
			arg_uc = va_arg(ap, unsigned int);
			OUTPUT_CHR(arg_uc);
			break;

		case 's':
			arg_s = va_arg(ap, const char *);
			if (arg_s == NULL)
				arg_s = "<null>";
			flags &= ~LEADZEROFLAG;
			goto _output_string;

		case '-':
			flags |= LEFTFORMATFLAG;
			goto next_format;

		case '+':
			flags |= SHOWSIGNFLAG;
			goto next_format;

		case ' ':
			flags |= BLANKPOSFLAG;
			goto next_format;

		case '#':
			flags |= ALTFLAG;
			goto next_format;

		case 'l':
			if (flags & LONGFLAG)
				flags |= LONGLONGFLAG;
			flags |= LONGFLAG;
			goto next_format;

		case 'h':
			if (flags & HALFFLAG)
				flags |= HALFHALFFLAG;
			flags |= HALFFLAG;
			goto next_format;

		case 'z':
			flags |= SIZETFLAG;
			goto next_format;

		case 'j':
			flags |= INTMAXFLAG;
			goto next_format;

		case 't':
			flags |= PTRDIFFFLAG;
			goto next_format;

		case 'i':
		case 'd':
			arg_n = (flags & LONGLONGFLAG) ? va_arg(ap, long long) :
			        (flags & LONGFLAG) ? va_arg(ap, long) :
			        (flags & HALFHALFFLAG) ? (signed char)va_arg(ap, int) :
			        (flags & HALFFLAG) ? (short)va_arg(ap, int) :
			        (flags & SIZETFLAG) ? va_arg(ap, size_t) :
			        (flags & INTMAXFLAG) ? va_arg(ap, intmax_t) :
			        (flags & PTRDIFFFLAG) ? va_arg(ap, ptrdiff_t) :
			        va_arg(ap, int);
			flags |= SIGNEDFLAG;
			arg_s = longlong_to_string(num_buffer, arg_n, sizeof(num_buffer), flags, &sign_char);
			goto _output_string;

		case 'u':
			arg_n = (flags & LONGLONGFLAG) ? va_arg(ap, unsigned long long) :
			        (flags & LONGFLAG) ? va_arg(ap, unsigned long) :
			        (flags & HALFHALFFLAG) ? (unsigned char)va_arg(ap, unsigned int) :
			        (flags & HALFFLAG) ? (unsigned short)va_arg(ap, unsigned int) :
			        (flags & SIZETFLAG) ? va_arg(ap, size_t) :
			        (flags & INTMAXFLAG) ? va_arg(ap, uintmax_t) :
			        (flags & PTRDIFFFLAG) ? (uintptr_t)va_arg(ap, ptrdiff_t) :
			        va_arg(ap, unsigned int);
			arg_s = longlong_to_string(num_buffer, arg_n, sizeof(num_buffer), flags, &sign_char);
			goto _output_string;

		case 'p':
			flags |= SIZETFLAG | ALTFLAG;
			goto hex;

		case 'X':
			flags |= CAPSFLAG;
			/* fallthrough */

		case 'x':
		hex:
			arg_n = (flags & LONGLONGFLAG) ? va_arg(ap, unsigned long long) :
			        (flags & LONGFLAG) ? va_arg(ap, unsigned long) :
			        (flags & HALFHALFFLAG) ? (unsigned char)va_arg(ap, unsigned int) :
			        (flags & HALFFLAG) ? (unsigned short)va_arg(ap, unsigned int) :
			        (flags & SIZETFLAG) ? va_arg(ap, size_t) :
			        (flags & INTMAXFLAG) ? va_arg(ap, uintmax_t) :
			        (flags & PTRDIFFFLAG) ? (uintptr_t)va_arg(ap, ptrdiff_t) :
			        va_arg(ap, unsigned int);
			arg_s = longlong_to_hexstring(num_buffer, arg_n, sizeof(num_buffer), flags);

			/* Normalize c, since code in _output_string needs to know that this is printing hex */
			c = 'x';

			/* Altflag processing should be bypassed when n == 0 so that 0x is not prepended to it */
			if (arg_n == 0)
				flags &= ~ALTFLAG;

			goto _output_string;

		default:
			OUTPUT_CHR('%');
			OUTPUT_CHR(c);
			break;
		}

		/* move on to the next field */
		continue;

	_output_string:
		string_len = strlen(arg_s);

		if (flags & LEFTFORMATFLAG) {
			/* left justify the text */
			int written;

			OUTPUT_STR(arg_s, string_len);

			written = err;
			for (; format_num > written; format_num--)
				OUTPUT_CHR(' ');
		} else {
			/* right justify the text (digits) */

			/* print a sign digit */
			if (sign_char != '\0' && format_num > 0)
				format_num--;

			if (sign_char != '\0' && (flags & LEADZEROFLAG))
				OUTPUT_CHR(sign_char);

			/* print for %#X */
			if (c == 'x' && (flags & ALTFLAG)) {
				OUTPUT_CHR('0');
				OUTPUT_CHR((flags & CAPSFLAG) ? 'X' : 'x');

				if (format_num >= 2)
					format_num -= 2;
			}

			/* pad according to the format string */
			for (; format_num > string_len; format_num--)
				OUTPUT_CHR((flags & LEADZEROFLAG) ? '0' : ' ');

			/* if not leading zeros, output the sign char just before the number */
			if (sign_char != '\0' && !(flags & LEADZEROFLAG))
				OUTPUT_CHR(sign_char);

			/* output the string */
			OUTPUT_STR(arg_s, string_len);
		}
	}

#undef OUTPUT_STR
#undef OUTPUT_CHR
exit:
	return (err < 0) ? err : chars_written;
}

/**
 * puts
 */
struct puts_args {
	char *out;
	size_t len;
	size_t pos;
};

static int k_vsnprintf_puts(const char *str, size_t len, void *_arg)
{
	struct puts_args *arg = _arg;
	size_t count = 0;

	while (count < len) {
		if (arg->pos >= arg->len)
			break;

		arg->out[arg->pos++] = *str;
		TRACE("%s: %c\n", __func__, *str);

		str++;
		count++;
	}
	return count;
}

int k_vsnprintf(char *str, size_t len, const char *fmt, va_list ap)
{
	int wlen;

	struct puts_args arg = {
		.out = str,
		.len = len,
		.pos = 0,
	};

	wlen = kprintf_engine(k_vsnprintf_puts, &arg, fmt, ap);
	if (len == 0)
		;
	else if (arg.pos >= len)
		str[len - 1] = '\0';
	else
		str[wlen] = '\0';

	return wlen;
}

int k_vsprintf(char *str, const char *fmt, va_list ap)
{
	return k_vsnprintf(str, INT_MAX, fmt, ap);
}

int k_sprintf(char *str, const char *fmt, ...)
{
	int err;

	va_list ap;
	va_start(ap, fmt);
	err = k_vsprintf(str, fmt, ap);
	va_end(ap);

	return err;
}

int k_snprintf(char *str, size_t len, const char *fmt, ...)
{
	int err;

	va_list ap;
	va_start(ap, fmt);
	err = k_vsnprintf(str, len, fmt, ap);
	va_end(ap);

	return err;
}

static int k_fprintf_puts(const char *str, size_t len, void *_arg)
{
	FILE *fp = _arg;

	return fp->io->write(fp, str, len);
}

int k_vfprintf(FILE *fp, const char *fmt, va_list ap)
{
	return kprintf_engine(k_fprintf_puts, fp, fmt, ap);
}

int k_fprintf(FILE *fp, const char *fmt, ...)
{
	int err;
	va_list ap;

	va_start(ap, fmt);
	err = k_vfprintf(fp, fmt, ap);
	va_end(ap);

	return err;
}

#if 0
/* NO$GBA not support for putc, sad :( */
int k_printf(const char *fmt, ...)
{
	int err;
	va_list ap;

	va_start(ap, fmt);
	err = k_vfprintf(stdout, fmt, ap);
	va_end(ap);

	return err;
}
#else
int k_printf(const char *fmt, ...)
{
	int err;
	va_list ap;
	char buf[0x100];

	va_start(ap, fmt);
	err = k_vsprintf(buf, fmt, ap);
	va_end(ap);

	k_fprintf_puts(buf, err, stdout);
	return err;
}
#endif

/**
 * APIs
 */
void kprintf_test(u16 arg)
{
	uint32_t sp[2] = { 0x08090A0B, 0x03040201 };

	k_printf("helloworld");
	k_printf("helloworld %09d %s", 2, "hello");
	k_printf("local sp:%p, 0x%08lX, %#lX", sp, sp[0], sp[1]);
	k_printf("%% i: %i", 0x090807);
	k_printf("%% -: %08d", -980);

	fmt_printf("helloworld");
	fmt_printf("helloworld %09d %s", 2, "hello");
}
