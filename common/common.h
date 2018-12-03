#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef __ASSEMBLY__
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "bsp.h"
#include "shell.h"
#include "soft_i2c.h"
#include "soft_uart.h"

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
