#ifndef CMSIS_UTIL_H
#define CMSIS_UTIL_H

#define LEN(ar) (sizeof (ar) / sizeof *(ar))

#include <stdint.h>

void sysclock_pll_hse_100mhz(void);
void set_microsecond_downcount(TIM_TypeDef *t);
void spin_delay_ms(TIM_TypeDef *t, uint32_t N);

#endif
