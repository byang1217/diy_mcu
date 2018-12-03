#ifndef __stm32f10x_lib_H__
#define __stm32f10x_lib_H__

void stm32f10x_init(void);
void stm32f10x_gpio_init(int pin, int mode);
int stm32f10x_gpio_get(int pin);
void stm32f10x_gpio_set(int pin, int high);
void stm32f10x_uart_init(int id, int rate);
void stm32f10x_uart_putc(int id, char c);
int stm32f10x_uart_getc(int id);

#endif /* __stm32f10x_lib_H__ */
