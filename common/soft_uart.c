#include "common.h"
#include "soft_uart.h"

void soft_uart_write(struct soft_uart *su, char *buf, int len)
{
	int i;
	int us = 1000000 / su->speed;

	while (len --) {
		long flags = interrupt_save();

		su->pin_set_func(su->tx_pin, 0); //start
		delay_us(us);

		for (i = 0; i < 8; i++) {
			su->pin_set_func(su->tx_pin, *buf & (1 << i));
			delay_us(us);
		}
		su->pin_set_func(su->tx_pin, 1); //stop

		interrupt_restore(flags);

		delay_us(us);
		delay_us(us);
		buf ++;
	}
}
