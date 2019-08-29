#include "common.h"
#include "bsp.h"

static char interrupt_disable_depth;
static unsigned long ticks_count;

void abort(void)
{
	if (default_shell.shell_putc) {
		default_shell.shell_putc('\r');
		default_shell.shell_putc('\n');
		default_shell.shell_putc('a');
		default_shell.shell_putc('b');
		default_shell.shell_putc('o');
		default_shell.shell_putc('r');
		default_shell.shell_putc('t');
	}
	__asm__ __volatile__ ("bkpt #0");
	while(1);
}

void assert_param(int cond)
{
	if (!cond)
		abort();
}

unsigned long bsp_get_uptime_us(void)
{
	return ticks_count * (1000000 / HZ) + SysTick->VAL / (SystemCoreClock / 1000000);
}

unsigned long micros(void)
{
	return bsp_get_uptime_us();
}

unsigned long bsp_get_uptime_ms(void)
{
	return ticks_count * (1000 / HZ) + SysTick->VAL / (SystemCoreClock / 1000);
}

unsigned long millis(void)
{
	return bsp_get_uptime_ms();
}

__attribute__((optimize("-O0"))) void delay_us(unsigned int us)
{
#if 1
	uint32_t cur = SysTick->VAL;
	uint32_t delay = us * (SystemCoreClock / 1000000);
	uint32_t end;
	uint32_t max = SystemCoreClock / HZ - 1;
	uint32_t win = max / 10;

	assert_param(delay < (max - win));
	if (cur < delay) {
		end = max - (delay - cur);
		while (SysTick->VAL <= cur || SysTick->VAL >= end)
			;
	} else {
		end = cur - delay;
		if (end > win) {
			while (SysTick->VAL > end)
				;
		} else {
			while (SysTick->VAL > end && SysTick->VAL < (max - (win - end)))
				;
		}
	}
#else
	volatile int loop;
	while (us--)
		for (loop = 0; loop < 4; loop++) //72M clock
			;
#endif
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
	if (interrupt_disable_depth == 0) {
		__enable_fault_irq();
		__enable_irq();
	}
}

long interrupt_save(void)
{
	__disable_irq();
	__disable_fault_irq();
	interrupt_disable_depth++;
	return interrupt_disable_depth;
}

void SysTick_Handler(void)
{
	ticks_count++;
}

void bsp_gpio_mode(int pin, int mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if (pin <= PA_15) {
		GPIO_InitStructure.GPIO_Pin = 1 << (pin - PA_0);
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = mode;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	} else if (pin <= PB_15) {
		GPIO_InitStructure.GPIO_Pin = 1 << (pin - PB_0);
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = mode;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_Init(GPIOB, &GPIO_InitStructure);
	} else if (pin <= PC_15) {
		GPIO_InitStructure.GPIO_Pin = 1 << (pin - PC_0);
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = mode;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	} else {
		assert_param(0);
	}
}

void bsp_gpio_direction(int pin, int input)
{
	if (input)
		bsp_gpio_mode(pin, GPIO_Mode_IN_FLOATING);
	else
		bsp_gpio_mode(pin, GPIO_Mode_Out_PP);
}

void bsp_gpio_set(int pin, int high)
{
	if (pin <= PA_15)
		GPIO_WriteBit(GPIOA, 1 << (pin - PA_0), !!high);
	else if (pin <= PB_15)
		GPIO_WriteBit(GPIOB, 1 << (pin - PB_0), !!high);
	else if (pin <= PC_15)
		GPIO_WriteBit(GPIOC, 1 << (pin - PC_0), !!high);
	else
		assert_param(0);
}

int bsp_gpio_get(int pin)
{
	if (pin <= PA_15)
		return (int)GPIO_ReadInputDataBit(GPIOA, 1 << (pin - PA_0));
	else if (pin <= PB_15)
		return (int)GPIO_ReadInputDataBit(GPIOB, 1 << (pin - PB_0));
	else if (pin <= PC_15)
		return (int)GPIO_ReadInputDataBit(GPIOC, 1 << (pin - PC_0));
	else
		assert_param(0);
}

void bsp_uart_init(int id, int rate)
{
	USART_TypeDef* USARTx;
	USART_InitTypeDef USART_InitStructure;

	if (id == 1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		bsp_gpio_mode(UART1_TX_PIN, GPIO_Mode_AF_PP);
		bsp_gpio_mode(UART1_RX_PIN, GPIO_Mode_IN_FLOATING);
		USARTx = USART1;
	} else if (id == 2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		bsp_gpio_mode(UART2_TX_PIN, GPIO_Mode_AF_PP);
		bsp_gpio_mode(UART2_RX_PIN, GPIO_Mode_IN_FLOATING);
		USARTx = USART2;
	} else if (id == 3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		bsp_gpio_mode(UART3_TX_PIN, GPIO_Mode_AF_PP);
		bsp_gpio_mode(UART3_RX_PIN, GPIO_Mode_IN_FLOATING);
		USARTx = USART3;
	} else {
		assert_param(0);
	}

	USART_Cmd(USARTx, ENABLE);  
	USART_InitStructure.USART_BaudRate = rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USARTx, &USART_InitStructure);
}

void bsp_uart_putc(int id, char c)
{
	USART_TypeDef* USARTx;

	if (id == 1)
		USARTx = USART1;
	else if (id == 2)
		USARTx = USART2;
	else if (id == 3)
		USARTx = USART3;
	else
		assert_param(0);

	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
		;
	USART_SendData(USARTx, (uint16_t)c);
}

int bsp_uart_getc(int id)
{
	USART_TypeDef* USARTx;

	if (id == 1)
		USARTx = USART1;
	else if (id == 2)
		USARTx = USART2;
	else if (id == 3)
		USARTx = USART3;
	else
		assert_param(0);

	if (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
		return -1;
	return (int)USART_ReceiveData(USARTx);
}

#if 0 //not test
void stm32f10x_i2c_init(int id, int speed)
{
#define I2C_SPEED               300000
#define I2C_SLAVE_ADDRESS7      0xA0
	I2C_InitTypeDef  I2C_InitStructure;

	// only support I2C1
	assert_param(id == 1);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	bsp_gpio_mode(I2C1_SCL_PIN, GPIO_Mode_AF_OD);
	bsp_gpio_mode(I2C1_SDA_PIN, GPIO_Mode_AF_OD);

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}
#endif

static uint32_t tim_period_us[5];
static uint32_t tim_period[5];
static uint32_t tim_scaler[5];
void bsp_tim_init(int id, int period_us)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	uint32_t TimerPeriod = SystemCoreClock / (1000000 / period_us) - 1;

	assert_param(id == 1 || id == 2 || id == 3 || id == 4);
	assert_param(tim_period_us[id] == 0); //don't double init
	if (TimerPeriod > 0xffff)
		tim_scaler[id] = 1;
	while (TimerPeriod > 0xffff) {
		TimerPeriod /= 10;
		tim_scaler[id] *= 10;
	}
	if (TimerPeriod == 0)
		TimerPeriod = 1;
	tim_period[id] = TimerPeriod;
	tim_period_us[id] = period_us;
	assert_param(TimerPeriod < 0xffff);
	assert_param(tim_scaler[id] < 0xffff);
	TIM_TimeBaseStructure.TIM_Prescaler = tim_scaler[id];
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = (uint16_t)TimerPeriod;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	if (id == 1) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
		TIM_Cmd(TIM1, ENABLE);
	}else if (id == 2) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_Cmd(TIM2, ENABLE);
	}else if (id == 3) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_Cmd(TIM3, ENABLE);
	}else if (id == 4) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
		TIM_Cmd(TIM4, ENABLE);
	}else {
		assert_param(0);
	}
}

void bsp_pwm_set(int pwm, int period_us, int pulse_us)
{
	static uint32_t period[5];
	int tim_id = (pwm >> 8) & 0xf;
	int tim_ch = (pwm >> 16) & 0xf;
	int pin = pwm & 0xff;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint32_t pulse;
	TIM_TypeDef *TIMx;

	assert_param(tim_id == 1 || tim_id == 2 || tim_id == 3 || tim_id == 4);
	assert_param(tim_ch == 1 || tim_ch == 2 || tim_ch == 3 || tim_ch == 4);

	if (!period[tim_id]) {
		bsp_tim_init(tim_id, period_us);
		period[tim_id] = tim_period[tim_id];
	}
	assert_param(period_us == tim_period_us[tim_id]);
	pulse = (pulse_us * 1000 / period_us) * (period[tim_id] - 1) / 1000;

	TIM_OCInitStructure.TIM_Pulse = pulse;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

	if (tim_id == 1)
		TIMx = TIM1;
	else if (tim_id == 2)
		TIMx = TIM2;
	else if (tim_id == 3)
		TIMx = TIM3;
	else if (tim_id == 4)
		TIMx = TIM4;
	else
		assert_param(0);

	if (tim_ch == 1)
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);
	else if (tim_ch == 2)
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);
	else if (tim_ch == 3)
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);
	else if (tim_ch == 4)
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);
	else
		assert_param(0);

	bsp_gpio_mode(pin, GPIO_Mode_AF_PP);
}

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

void bsp_usb_init(void)
{
	bsp_gpio_mode(PA_12, GPIO_Mode_AF_PP);
	bsp_gpio_set(PA_12, 0);
	delay_us(100);
	bsp_gpio_mode(PA_12, GPIO_Mode_IN_FLOATING);

	/* Select USBCLK source */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

	/* Enable the USB clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);

	USB_Interrupts_Config();
	USB_Init();
}

void bsp_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	if (SysTick_Config((SystemCoreClock) / HZ))
		assert_param(0);
	NVIC_SetPriority(SysTick_IRQn, 0x0);
}
