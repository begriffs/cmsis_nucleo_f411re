#include "RTE_Components.h"
#include CMSIS_device_header

#include "cmsis_os2.h"

void app_main(void *arg)
{
	(void)arg;
	while (1)
	{
		//XOR to toggle
		GPIOC->ODR ^=GPIO_ODR_OD13; 
		osDelay(500);
	}
}

int main(void)
{
	SystemCoreClockUpdate();

	// Turn on the GPIOC peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	// Set GPIOC Pin13 Output
	GPIOC->MODER|= GPIO_MODER_MODE13_0;
	GPIOC->MODER&=~GPIO_MODER_MODE13_1;

	osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(app_main, NULL, NULL);    // Create application main thread
	osKernelStart();

	while(1);
}
