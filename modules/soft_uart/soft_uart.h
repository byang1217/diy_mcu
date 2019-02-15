#ifndef __SOFT_UART_H__
#define __SOFT_UART_H__

struct soft_uart {
	int speed;
	int tx_pin;
	void (*pin_set_func)(int pin, int high);
};

void soft_uart_write(struct soft_uart *su, char *buf, int len);

#endif /* __SOFT_UART_H__ */
