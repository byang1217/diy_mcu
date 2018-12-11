#include "common.h"

#if 0 //use sprintf.c
typedef unsigned char bool;
#define false 0
#define true 1

int vsnprintf(char *dest, size_t size, const char *fmt, va_list args)
{
	int might_format = 0;
	int len = 0;
	char lastchar = 0;
	bool binary_format = false;

	if (!dest || !size)
		return 0;

	while (*fmt && len < size) {
		if (!might_format) {
			if (*fmt == '\n' && lastchar != '\r') {
				if (len < size) {
					lastchar = *dest++ = '\r', len++;
					continue;
				} else
					break;
			} else if (*fmt != '%') {
				if (len < size)
					lastchar = *dest++ = *fmt, len++;
				else
					break;
			} else
				might_format = 1;
		} else {
			if (*fmt == '%') {
				if (len < size)
					*dest++ = '%', len++;
				else
					break;
				might_format = 0;
			} else {
				switch (*fmt) {
				case '0':
				case '1':
					might_format |= 2;
					goto still_format;
					break;
				case '2':
					might_format |= 4;
					goto still_format;
					break;
				case '4':
					might_format |= 8;
					goto still_format;
					break;
				case 'b':
					binary_format = true;
					goto still_format;
					break;
				case 'd':
				case 'i':
				case 'u':
					if (!binary_format) {
						unsigned long num =
							va_arg(args,
							       unsigned long);
						unsigned long pos = 999999999;
						unsigned long remainder = num;
						int found_largest_digit = 0;

						if (*fmt != 'u'
						    && (num & (1 << 31))) {
							if (len < size)
								*dest++ =
									'-',
								len++;
							num = (~num) + 1;
							remainder = num;
						}
						while (pos >= 9) {
							if (found_largest_digit
							    || remainder >
							    pos) {
								found_largest_digit
									= 1;
								if (len < size)
									*dest++
										=
											(
												char)
											((
												 remainder
												 / (
													 pos
													 +
													 1))
											 +
											 48),
									len++;
								else
									break;
							}
							remainder %= (pos + 1);
							pos /= 10;
						}
						if (len < size)
							*dest++ =
								(char)(
									remainder
									+
									48),
							len++;
						break;
					}
				case 'x':
				case 'X':
				case 'p': {
					unsigned long num =
						va_arg(args, unsigned long);
					int sz = sizeof(num) * 2;

					if (might_format & 8) {
						sz = 4;
					} else if (might_format & 4) {
						sz = 2;
					} else if (might_format & 2) {
						sz = 1;
					}
					for (; sz; sz--) {
						char nibble;
						if (!binary_format) {
							nibble =
								(num >>
								 ((sz -
								   1) <<
								  2) & 0xf);
							nibble +=
								nibble >
								9 ? 87 : 48;
						} else {
							nibble =
								(num >>
								 ((sz -
								   1) <<
								  3) & 0xff);
						}
						if (len < size)
							*dest++ =
								nibble,
							len++;
						else
							break;
					}
					break;
				}
				case 's': {
					char *s = va_arg(args, char *);
					while (*s)
						if (len < size)
							*dest++ =
								*s++, len++;
						else
							break;
					break;
				}
				case 'c': {
					char c = va_arg(args, int);
					if (len < size)
						*dest++ = c, len++;
					break;
				}
				default:
					if (len < size)
						*dest++ = '%', len++;
					if (len < size)
						*dest++ = *fmt, len++;
					break;
				}
				might_format = 0;
still_format:
				(void)might_format;
			}
		}
		++fmt;
	}
	*dest = '\0';
	return len;
}

int snprintf(char *dest, size_t size, const char *fmt, ...)
{
	int len;
	va_list args;

	va_start(args, fmt);
	len = vsnprintf(dest, size, fmt, args);
	va_end(args);
	return len;
}

int sprintf(char *dest, const char *fmt, ...)
{
	int len;
	va_list args;

	va_start(args, fmt);
	len = vsnprintf(dest, 0x7fffffff, fmt, args);
	va_end(args);
	return len;
}
#endif

void pr_log(const char *fmt, ...)
{
	va_list args;
	int i;
	char tmp[128];

	va_start(args, fmt);
	vsnprintf(tmp, sizeof(tmp), fmt, args);
	va_end(args);

	for (i = 0; tmp[i] != 0 && i < sizeof(tmp); i++) {
		default_shell.shell_putc(tmp[i]);
		if (tmp[i] == '\n' && (i == 0 || tmp[i - 1] != '\r'))
			default_shell.shell_putc('\r');
	}
}

