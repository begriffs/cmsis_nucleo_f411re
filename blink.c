#include CMSIS_device_header
#include <cmsis_os2.h>

#include "util.h"

void app_main(void *arg)
{
	(void)arg;
	while (1)
	{
		GPIOA->BSRR = (1 << 5);
		osDelay(1000);
		GPIOA->BSRR = (1 << (5 + 16));
		osDelay(1000);
	}
}

int main(void)
{
	// go fast
	sysclock_pll_hse_100mhz();

	// turn on clock for GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (0b01 << (5 << 1));

	// Initialize CMSIS-RTOS
	osKernelInitialize();
	// create app main thread
	osThreadNew(app_main, NULL, NULL);
	// RTOS take the wheel!
	osKernelStart();

	// should not get here
	while (1);
}
