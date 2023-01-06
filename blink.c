#include CMSIS_device_header

#include <FreeRTOS.h>
#include <task.h>

#include "util.h"

#define LEN(ar) (sizeof (ar) / sizeof *(ar))

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
		app, "app", LEN(appTaskStack), NULL,
		configMAX_PRIORITIES-1, appTaskStack, &appTaskBuf);

	// FreeRTOS, take the wheel!
	vTaskStartScheduler();
	// shouldn't get here
	configASSERT(0);
}
