#ifndef __BSP_H__
#define __BSP_H__

#ifndef HZ
#define HZ 100
#endif

#ifndef __ASSEMBLY__
void assert_param(int cond);

void delay_us(unsigned int us);
void delay_ms(unsigned int ms);
unsigned long bsp_get_uptime_us(void);

void bsp_init(void);

void bsp_uart_init(int id, int rate);
void bsp_uart_putc(int id, char c);
int bsp_uart_getc(int id);

void bsp_gpio_direction(int pin, int input);
void bsp_gpio_mode(int pin, int mode);
void bsp_gpio_set(int pin, int high);
int bsp_gpio_get(int pin);

void bsp_tim_init(int id, int period_us);
void bsp_pwm_set(int pwm, int period_us, int pulse_us);
#endif

#endif /* __BSP_H__ */
