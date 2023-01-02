#include <stm32f4xx.h>

#include "util.h"

void sysclock_pll_hse_100mhz(void)
{
	// enable HSE from attached ST-Link board
	RCC->CR |= RCC_CR_HSEBYP | RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY));

	// enable power-interface clock
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	// voltage scaling, 0x11 = scale 1 for >= 100mhz system clock
	PWR->CR |= (3<<PWR_CR_VOS_Pos);

	// The reference manaul for the STM32F411 series, section 3.4.1, says that
	// for a CPU running at 100MHz on a 2.7 - 3.6 supply voltage, the flash
	// latency should be 3 wait cycles
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS
			   // enable memory prefetching to mitigate the wait cycles
	           | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

	RCC->CFGR |=
		RCC_CFGR_PPRE1_DIV2 | // 100mhz/2 <= 50mhz APB1, as required
		RCC_CFGR_PPRE2_DIV1 | // 100mhz/1 <= 100mhz APB2, as required
		RCC_CFGR_HPRE_DIV1;   // don't know of any constraints

	// disable PLL
	RCC->CR &= ~RCC_CR_PLLON;
	while ((RCC->CR & RCC_CR_PLLRDY));

	// set HSE as the PLL input
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

	// set PLL parameters M, N, and P
	RCC->PLLCFGR &= (~RCC_PLLCFGR_PLLM_Msk) & (~RCC_PLLCFGR_PLLN_Msk) &
	                (~RCC_PLLCFGR_PLLP_Msk) & (~RCC_PLLCFGR_PLLQ_Msk);
	RCC->PLLCFGR |= (HSE_VALUE/100000) << RCC_PLLCFGR_PLLM_Pos;
	RCC->PLLCFGR |= 400                << RCC_PLLCFGR_PLLN_Pos;
	RCC->PLLCFGR |= 4                  << RCC_PLLCFGR_PLLP_Pos;

	// enable PLL
	RCC->CR |=RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	// use PLL as clock source
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	// record the results in SystemCoreClock
	SystemCoreClockUpdate();
}

void set_microsecond_downcount(TIM_TypeDef *t)
{
	// APB1 clock is set to half of system clock
	uint32_t prescale = (SystemCoreClock/2)/1000000U;

	// register holds at most 16-bit values
	if (prescale < 1 || prescale > UINT16_MAX)
		while (1) ;

	// prescale timer so each tick is 1 microsecond
	t->PSC = (uint16_t)prescale;

	// Downcounting mode is enabled in TIMx_CR1 by setting CMS to 0
	// and DIR to 1.
	t->CR1 &= ~TIM_CR1_CMS;
	t->CR1 |=  TIM_CR1_DIR;

	// auto preload reload enable
	//t->CR1 |=  TIM_CR1_ARPE;
}

void spin_delay_ms(TIM_TypeDef *t, uint32_t N)
{

	// must achieve N ms by repetitions of UINT16_MAX microseconds
	// plus a final (N*1000)%UINT16_MAX microseconds

	/*
	if ((uint32_t)N*1000U > UINT16_MAX)
	{
		t->CR1 &= (~TIM_CR1_CEN);                   // disable timer
		t->ARR  = UINT16_MAX;                       // count from
		t->RCR  = ((uint32_t)N*1000) / UINT16_MAX;  // repetitions
		t->SR  &= ~TIM_SR_UIF;                      // clear UIF

		t->CR1 |= TIM_CR1_CEN;        // enable timer, start counting
		while(!(t->SR & TIM_SR_UIF)); // wait for UIF to go high
	}
	*/
	t->CR1 &= ~TIM_CR1_CEN;

	t->ARR  = N*1000U;  // 32-bit countdown on general purpose tiemrs
	t->SR  &= ~TIM_SR_UIF;

	t->CR1 |= TIM_CR1_CEN;
	while(!(t->SR & TIM_SR_UIF));
}

#include <stddef.h>

void *
memcpy (void *dest, const void *src, size_t len)
{
	char *d = dest;
	const char *s = src;
	while (len--)
		*d++ = *s++;
	return dest;
}

void *
memset (void *dest, int val, size_t len)
{
	unsigned char *ptr = dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}
