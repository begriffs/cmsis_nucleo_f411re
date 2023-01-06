#include CMSIS_device_header

#include <FreeRTOS.h>
#include <task.h>

#include <stdbool.h>
#include "util.h"

void blink(void *args)
{
	(void)args;

	// turn on clock for GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// set PA5 (the LED) to general purpose output mode
	GPIOA->MODER |= GPIO_MODER_MODER5_0;

	// Use atomic bit-setting register (BSRR), rather than ODR, for good
	// practice. We have to maintain our own state that way though.
	for (bool set = true; ; set = !set)
	{
		GPIOA->BSRR = set ? GPIO_BSRR_BS5 : GPIO_BSRR_BR5;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

StaticTask_t blinkTaskBuf;
StackType_t  blinkTaskStack[configMINIMAL_STACK_SIZE];

int main(void)
{
	// blinky doesn't need high speed, but here's how to enable
	// the higher speed if desired:
	//
	// sysclock_pll_hse_100mhz();

	xTaskCreateStatic(
		blink, "blink", LEN(blinkTaskStack), NULL,
		configMAX_PRIORITIES-1, blinkTaskStack, &blinkTaskBuf);

	// FreeRTOS, take the wheel!
	vTaskStartScheduler();
	// shouldn't get here
	configASSERT(0);
}
