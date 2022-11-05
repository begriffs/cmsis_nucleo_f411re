.POSIX:

ABI = arm-none-eabi
CC = $(ABI)-gcc
AR = $(ABI)-ar

STMF411 = -mcpu=cortex-m4 -mthumb \
          -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -DARMCM4_FP -DSTM32F411xE \
          -DCMSIS_device_header='"stm32f411xe.h"'

CFLAGS = -std=c99 -Wall \
         -Wno-unused-function -Wno-unused-parameter \
         -g -Os $(STMF411) \
         -D_RTE_ -DRTE_DEVICE_FRAMEWORK_CLASSIC \
         -Icmsis_5/CMSIS/Core/Include \
         -Icmsis_5/CMSIS/Driver/Include \
         -Icmsis_5/Device/ARM/ARMCM4/Include \
         -Icmsis_5/CMSIS/RTOS2/Include \
         -Istm32f4xx-dfp-2.16.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
         -Istm32f4xx-dfp-2.16.0/Drivers/STM32F4xx_HAL_Driver/Inc \
         -Istm32f4xx-dfp-2.16.0/MDK/Templates/Inc \
         -Icmsis-freertos-10.4.3/CMSIS/RTOS2/FreeRTOS/Include \
         -Icmsis-freertos-10.4.3/Source/portable/GCC/ARM_CM4F \
         -Icmsis-freertos-10.4.3/Source/include \
         -I.

SRCS = startup_ARMCM4.c system_stm32f4xx.c \
       cmsis_os2.c os_systick.c freertos_evr.c \
       I2C_STM32F4xx.c SPI_STM32F4xx.c \
       USART_STM32F4xx.c USBD_FS_STM32F4xx.c

OBJS = $(SRCS:.c=.o)

VPATH = cmsis_5/Device/ARM/ARMCM4/Source:stm32f4xx-dfp-2.16.0/CMSIS/Driver:stm32f4xx-dfp-2.16.0/Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates:cmsis-freertos-10.4.3/CMSIS/RTOS2/FreeRTOS/Source

cmsis_stm32f411xe.a : $(OBJS)
	$(AR) r $@ $?

clean :
	rm -f *.[ao]
