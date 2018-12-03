#include "common.h"

void bsp_toggle_led_forever(void)
{
#if 0
	RCC->APB2ENR|=0X0000001c;//先使能外设IO PORTa,b,c时钟

	GPIOB->CRH=0X33333333;    //推挽输出
	GPIOB->CRL=0X33333333;    //推挽输出
	GPIOC->CRH=0X33333333;    //推挽输出
	GPIOC->CRL=0X33333333;    //推挽输出
	GPIOD->CRH=0X33333333;    //推挽输出
	GPIOD->CRL=0X33333333;    //推挽输出
	GPIOA->CRH=0X33333333;    //推挽输出
	GPIOA->CRL=0X33333333;    //推挽输出

	while (1)
	{			
		delay_ms(100);     	 
		GPIOB->ODR=0;	  		 //全部输出0
		GPIOA->ODR=0;		
		GPIOC->ODR=0;
		GPIOD->ODR=0;
		delay_ms(100);
		GPIOB->ODR=0xffffffff;	 //全部输出1 
		GPIOA->ODR=0xffffffff;	  
		GPIOC->ODR=0xffffffff;
		GPIOD->ODR=0xffffffff;
	}
#else
#if 1
	while (1)
	{			
		delay_ms(100);     	 
		bsp_onboard_led(0);
		delay_ms(100);     	 
		bsp_onboard_led(1);
	}
#else
	int i;

	pr_log("RCC->APB2ENR: %x\n", RCC->APB2ENR);
	for (i = PA_0; i <= PC_15; i++) {
		if (i == UART1_TX_PIN)
			continue;
		bsp_gpio_direction(i, 0);
		bsp_gpio_set(i, 0);
	}

	pr_log("RCC->APB2ENR: %x\n", RCC->APB2ENR);
	pr_log("GPIOA->CRH: %x\n", GPIOA->CRH);
	pr_log("GPIOA->CRL: %x\n", GPIOA->CRL);
	pr_log("GPIOB->CRH: %x\n", GPIOB->CRH);
	pr_log("GPIOB->CRL: %x\n", GPIOB->CRL);
	pr_log("GPIOC->CRH: %x\n", GPIOC->CRH);
	pr_log("GPIOC->CRL: %x\n", GPIOC->CRL);

	while (1) {
		delay_ms(100);
		for (i = PA_0; i <= PC_15; i++) {
			if (i == UART1_TX_PIN)
				continue;
			bsp_gpio_set(i, 0);
		}
		delay_ms(100);
		for (i = PA_0; i <= PC_15; i++) {
			if (i == UART1_TX_PIN)
				continue;
			bsp_gpio_set(i, 0);
		}
	}
#endif
#endif
}

static struct soft_uart suart1 = {
	.speed = 9600,
	.tx_pin = PB_10,
	.pin_set_func = bsp_gpio_set,
};

static struct soft_uart suart2 = {
	.speed = 9600,
	.tx_pin = PB_11,
	.pin_set_func = bsp_gpio_set,
};

static void soft_uart_test(void)
{
	bsp_gpio_direction((int)suart1.tx_pin, 0);
	bsp_gpio_direction((int)suart2.tx_pin, 0);

	while (1) {
		soft_uart_write(&suart1, "U\r\n", 3);
		soft_uart_write(&suart2, "U\r\n", 3);
	}
}

static struct soft_i2c i2c1 = {
	.pin_mode_od = 1,
	.scl_pin = PB_6,
	.sda_pin = PB_7,
	.pin_set_func = bsp_gpio_set,
	.pin_get_func = bsp_gpio_get,
};

static void soft_i2c_test(void)
{
	bsp_gpio_mode(i2c1.scl_pin, GPIO_Mode_Out_OD);
	bsp_gpio_mode(i2c1.sda_pin, GPIO_Mode_Out_OD);

	while (1) {
		char rbuf[4];
		int ret;
		int addr;

		pr_log("soft i2c1 scan ...\n");
		for (addr = 0x3; addr < 0x7f; addr ++) {
			if (soft_i2c_xfer(&i2c1, addr, NULL, 0, NULL, 0) == 0)
				pr_log("found device, slave addr = 0x%x\n", addr);
		}
		delay_ms(1);
	}
}

static void soft_i2c_pin_direction(int pin, int input)
{
	if (input)
		bsp_gpio_mode(pin, GPIO_Mode_IPU);
	else
		bsp_gpio_mode(pin, GPIO_Mode_Out_PP);
}

static struct soft_i2c i2c2 = {
	.scl_pin = PB_6,
	.sda_pin = PB_7,
	.pin_direction_func = soft_i2c_pin_direction,
	.pin_set_func = bsp_gpio_set,
	.pin_get_func = bsp_gpio_get,
};

static void soft_i2c_test2(void)
{
	while (1) {
		char rbuf[4];
		int ret;
		int addr;

		pr_log("soft i2c2 scan ...\n");
		for (addr = 0x3; addr < 0x7f; addr ++) {
			if (soft_i2c_xfer(&i2c2, addr, NULL, 0, NULL, 0) == 0)
				pr_log("found device, slave addr = 0x%x\n", addr);
		}
		delay_ms(1);
	}
}

static void pwm_test(void)
{
	bsp_pwm_set(PWM_1_1_PA_8, 20*1000, 1.5*1000);
	bsp_pwm_set(PWM_1_4_PA_11, 20*1000, 2*1000);

	bsp_pwm_set(PWM_2_1_PA_0, 20*1000, 5*1000);
	bsp_pwm_set(PWM_2_2_PA_1, 20*1000, 15*1000);
	bsp_pwm_set(PWM_2_3_PA_2, 20*1000, 18*1000);
	bsp_pwm_set(PWM_2_4_PA_3, 20*1000, 1*1000);

	bsp_pwm_set(PWM_3_1_PA_6, 123*1000, 5*1000);
	bsp_pwm_set(PWM_3_2_PA_7, 123*1000, 100*1000);
	bsp_pwm_set(PWM_3_3_PB_0, 123*1000, 120*1000);
	bsp_pwm_set(PWM_3_4_PB_1, 123*1000, 11*1000);

	bsp_pwm_set(PWM_4_1_PB_6, 20*1000, 0*1000);
	bsp_pwm_set(PWM_4_2_PB_7, 20*1000, 2*1000);
	bsp_pwm_set(PWM_4_3_PB_8, 20*1000, 1*1000);
	bsp_pwm_set(PWM_4_4_PB_9, 20*1000, 10*1000);

	pr_log("pwm init done!!!\n");
}

static void servo_test(void)
{
	while (1) {
		pr_log("move 0.5!!!\n");
		bsp_pwm_set(PWM_4_4_PB_9, 20*1000, 0.5*1000);
		delay_ms(2000);
		pr_log("move 0.8!!!\n");
		bsp_pwm_set(PWM_4_4_PB_9, 20*1000, 0.8*1000);
		delay_ms(2000);
		pr_log("move 1.5!!!\n");
		bsp_pwm_set(PWM_4_4_PB_9, 20*1000, 1.5*1000);
		delay_ms(2000);
		pr_log("move 2.0!!!\n");
		bsp_pwm_set(PWM_4_4_PB_9, 20*1000, 2*1000);
		delay_ms(2000);
		pr_log("move 2.2!!!\n");
		bsp_pwm_set(PWM_4_4_PB_9, 20*1000, 2.2*1000);
		delay_ms(2000);
		pr_log("move 2.5!!!\n");
		bsp_pwm_set(PWM_4_4_PB_9, 20*1000, 2.5*1000);
		delay_ms(2000);
	}
}

