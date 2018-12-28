#ifndef __COMMON_H__
#define __COMMON_H__

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

#ifndef __ASSEMBLY__
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "shell.h"

#ifndef __SHELL_TEST
#include "bsp.h"
#include "soft_i2c.h"
#include "soft_uart.h"
#endif

#define pr_err pr_log
#define pr_dbg pr_log
void pr_log(const char *fmt, ...);
int vsnprintf(char *dest, size_t size, const char *fmt, va_list args);
int snprintf(char *dest, size_t size, const char *fmt, ...);
int sprintf(char *dest, const char *fmt, ...);

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
void interrupt_restore(long flags);
long interrupt_save(void);

#endif

#endif /* __COMMON_H__ */
