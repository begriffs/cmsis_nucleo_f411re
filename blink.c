#include <stm32f4xx.h> 

void enable_hse(void)
{
	RCC->CR |= RCC_CR_HSEBYP | RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY))
		;

	// record the results in SystemCoreClock
	SystemCoreClockUpdate();
}

int main(void)
{
	enable_hse();
	/* no blinking yet... */
	while (1)
		;
}
