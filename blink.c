#include CMSIS_device_header

#include "util.h"

int main(void)
{
	sysclock_pll_hse_100mhz();

	// turn on clock for TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	// configure TIM2
	set_microsecond_downcount(TIM2);

	// turn on clock for GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (0b01 << (5 << 1));

	while (1)
	{
		GPIOA->BSRR = (1 << 5);
		spin_delay_ms(TIM2, 1000);
		GPIOA->BSRR = (1 << (5 + 16));
		spin_delay_ms(TIM2, 1000);
	}
}
