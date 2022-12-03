#include <stm32f4xx.h> 

void max_speed(void)
{
	// enable HSE from attached ST-Link board
	RCC->CR |= RCC_CR_HSEBYP | RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY))
		;

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= (3<<PWR_CR_VOS_Pos);
	// The reference manaul for the STM32F411 series says that for a CPU
	// running at 100MHz on a 2.7-3.6 supply voltage, the flash latency should
	// be 3 wait cycles
	FLASH->ACR |=
		FLASH_ACR_LATENCY_3WS | FLASH_ACR_PRFTEN |
		FLASH_ACR_ICEN | FLASH_ACR_DCEN;

	RCC->CFGR |=
		RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_HPRE_DIV1;

	// disable PLL
	RCC->CR &= ~RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY))
		;

	// set HSE as the PLL input
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

	// set PLL parameters M, N, and P
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLP_Msk;
	RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLQ_Msk;
	RCC->PLLCFGR |= (HSE_VALUE/100000) << RCC_PLLCFGR_PLLM_Pos;
	RCC->PLLCFGR |= 400                << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR |= 4                  << RCC_PLLCFGR_PLLP_Pos;

	// enable PLL
	RCC->CR |=RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY))
		;

	// use PLL as clock source
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
		;

	// record the results in SystemCoreClock
	SystemCoreClockUpdate();
}

void delay(unsigned time) {
	for (unsigned i=0; i<time; i++)
		for (volatile unsigned j=0; j<20000; j++);
}

int main(void)
{
	max_speed();

	// turn on GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (0b01 << (5 << 1));

	while (1)
	{
		GPIOA->BSRR = (1 << 5);
		delay(20);
		GPIOA->BSRR = (1 << (5 + 16));
		delay(20);
	}
}
