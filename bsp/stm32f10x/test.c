#include "bsp.h"

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
