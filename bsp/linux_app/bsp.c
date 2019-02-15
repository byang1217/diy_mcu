#include "common.h"
#include "bsp.h"
#include <unistd.h>
#include <sys/time.h>

static char interrupt_disable_depth;
static unsigned long ticks_count;

void assert_param(int cond)
{
	if (!cond)
		abort();
}

unsigned long bsp_get_uptime_us(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000 + tv.tv_usec);
}
unsigned long micros(void)
{
	return bsp_get_uptime_us();
}

unsigned long bsp_get_uptime_ms(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

unsigned long millis(void)
{
	return bsp_get_uptime_ms();
}

void delay_us(unsigned int us)
{
	usleep(us);
}

void delay_ms(unsigned int ms)
{
	while (ms--)
		delay_us(1000);
}

void interrupt_restore(long flags)
{
	interrupt_disable_depth--;
	assert_param(interrupt_disable_depth >= 0);
}

long interrupt_save(void)
{
	interrupt_disable_depth++;
	return interrupt_disable_depth;
}

void bsp_gpio_mode(int pin, int mode)
{
}

void bsp_gpio_direction(int pin, int input)
{
}

void bsp_gpio_set(int pin, int high)
{
}

int bsp_gpio_get(int pin)
{
}

void bsp_uart_init(int id, int rate)
{
}

void bsp_uart_putc(int id, char c)
{
}

int bsp_uart_getc(int id)
{
}

void bsp_pwm_set(int pwm, int period_us, int pulse_us)
{
}

void bsp_init(void)
{
}
