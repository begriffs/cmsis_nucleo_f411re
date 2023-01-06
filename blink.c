#include CMSIS_device_header

#include <FreeRTOS.h>
#include <task.h>

#include "util.h"


void app(void *args)
{
	(void)args;
	while (1)
	{
		GPIOA->BSRR = (1 << 5);
		vTaskDelay(pdMS_TO_TICKS(500));
		GPIOA->BSRR = (1 << (5 + 16));
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

StaticTask_t appTaskBuf;
StackType_t  appTaskStack[configMINIMAL_STACK_SIZE];

int main(void)
{
	sysclock_pll_hse_100mhz();

	// turn on clock for GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (1 << (5 << 1));

	xTaskCreateStatic(
		app, "app", configMINIMAL_STACK_SIZE, NULL,
		configMAX_PRIORITIES-1, appTaskStack, &appTaskBuf);

	vTaskStartScheduler(); // FreeRTOS, take the wheel!
	configASSERT(0); // shouldn't get here
}
