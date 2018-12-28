#ifndef __BSP_H__
#define __BSP_H__

#define ROM_START 0x08000000
#define ROM_LEN 0x00010000

#define RAM_START 0x20000000
#define RAM_LEN 0x00005000

#define STACK_SIZE 0x00001000

#ifndef HZ
#define HZ 100
#endif

#ifndef __ASSEMBLY__
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_can.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_cec.h"
#include "stm32f10x_dbgmcu.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_wwdg.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_fsmc.h"
#include "stm32f10x_iwdg.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x_tim.h"

enum {
	PA_0 = 0, PWM_TIM2_CH1_PIN = PA_0,
	PA_1, PWM_TIM2_CH2_PIN = PA_1,
	PA_2, UART2_TX_PIN = PA_2, PWM_TIM2_CH3_PIN = PA_2,
	PA_3, UART2_RX_PIN = PA_3, PWM_TIM2_CH4_PIN = PA_3,
	PA_4, PA_5,
	PA_6, PWM_TIM3_CH1_PIN = PA_6,
	PA_7, PWM_TIM3_CH2_PIN = PA_7,
	PA_8, PWM_TIM1_CH1_PIN = PA_8,
	PA_9, UART1_TX_PIN = PA_9, PWM_TIM1_CH2_PIN = PA_9,
	PA_10, UART1_RX_PIN = PA_10, PWM_TIM1_CH3_PIN = PA_10,
	PA_11, PWM_TIM1_CH4_PIN = PA_11,
	PA_12,
	PA_13, SWIO_PIN = PA_13, //should not use if debug enabled
	PA_14, SWCLK_PIN = PA_14, //should not use if debug enabled
	PA_15,

	PB_0, PWM_TIM3_CH3_PIN = PB_0,
	PB_1, PWM_TIM3_CH4_PIN = PB_1,
	PB_2, BOOT1_PIN = PB_2, //should not use if BOOT0 == 1
	PB_3, PB_4, PB_5,
	PB_6, I2C1_SCL_PIN = PB_6, PWM_TIM4_CH1_PIN = PB_6,
	PB_7, I2C1_SDA_PIN = PB_7, PWM_TIM4_CH2_PIN = PB_7,
	PB_8, PWM_TIM4_CH3_PIN = PB_8,
	PB_9, PWM_TIM4_CH4_PIN = PB_9,
	PB_10, UART3_TX_PIN = PB_10,
	PB_11, UART3_RX_PIN = PB_11,
	PB_12, PB_13, PB_14, PB_15,

        PC_0, PC_1, PC_2, PC_3, PC_4, PC_5, PC_6, PC_7, PC_8,
        PC_9, PC_10, PC_11, PC_12, PC_13, PC_14, PC_15,
};

// TIM_CH
#define PWM_1_1_PA_8 (PWM_TIM1_CH1_PIN | 1 << 8 | 1 << 16) //failed ?
#define PWM_1_2_PA_9 (PWM_TIM1_CH2_PIN | 1 << 8 | 2 << 16) //not test
#define PWM_1_3_PA_10 (PWM_TIM1_CH3_PIN | 1 << 8 | 3 << 16) //not test
#define PWM_1_4_PA_11 (PWM_TIM1_CH4_PIN | 1 << 8 | 4 << 16) //failed ?

#define PWM_2_1_PA_0 (PWM_TIM2_CH1_PIN | 2 << 8 | 1 << 16)
#define PWM_2_2_PA_1 (PWM_TIM2_CH2_PIN | 2 << 8 | 2 << 16)
#define PWM_2_3_PA_2 (PWM_TIM2_CH3_PIN | 2 << 8 | 3 << 16) //failed?
#define PWM_2_4_PA_3 (PWM_TIM2_CH4_PIN | 2 << 8 | 4 << 16)

#define PWM_3_1_PA_6 (PWM_TIM3_CH1_PIN | 3 << 8 | 1 << 16)
#define PWM_3_2_PA_7 (PWM_TIM3_CH2_PIN | 3 << 8 | 2 << 16)
#define PWM_3_3_PB_0 (PWM_TIM3_CH3_PIN | 3 << 8 | 3 << 16)
#define PWM_3_4_PB_1 (PWM_TIM3_CH4_PIN | 3 << 8 | 4 << 16)

#define PWM_4_1_PB_6 (PWM_TIM4_CH1_PIN | 4 << 8 | 1 << 16)
#define PWM_4_2_PB_7 (PWM_TIM4_CH2_PIN | 4 << 8 | 2 << 16)
#define PWM_4_3_PB_8 (PWM_TIM4_CH3_PIN | 4 << 8 | 3 << 16)
#define PWM_4_4_PB_9 (PWM_TIM4_CH4_PIN | 4 << 8 | 4 << 16)

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
