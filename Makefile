.POSIX:

ABI = arm-none-eabi
CC = $(ABI)-gcc
AR = $(ABI)-ar
OBJCOPY = $(ABI)-objcopy

STMF411 = -mcpu=cortex-m4 -mthumb \
          -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
          -DCMSIS_device_header='"stm32f4xx.h"' \
          -DARMCM4_FP -DSTM32F411xE

NUCLEO = -DHSE_VALUE=8000000L

CFLAGS = -std=c99 -Wall -Wextra -g -O0 $(STMF411) $(NUCLEO) \
         -Icmsis/CMSIS/Core/Include \
         -Icmsis/CMSIS/RTOS2/Include \
         -Icmsis/Device/ARM/ARMCM4/Include \
         -Icmsis-dfp-stm32f4/Include \
         -Icmsis-freertos/CMSIS/RTOS2/FreeRTOS/Include \
         -Icmsis-freertos/CMSIS/RTOS2/FreeRTOS/Config/ARMCM \
         -Icmsis-freertos/Source/portable/GCC/ARM_CM4F \
         -Icmsis-freertos/Source/include \
         -I.

SRCS = startup_ARMCM4.c system_stm32f4xx.c \
       cmsis_os2.c os_systick.c freertos_evr.c \
	   croutine.c event_groups.c list.c \
	   queue.c stream_buffer.c tasks.c \
	   timers.c port.c heap_4.c
OBJS = $(SRCS:.c=.o)

blink.axf blink.bin : blink.c util.o libcmsis_stm32f411xe.a stm32f411ceu6.ld
	$(CC) $(CFLAGS) blink.c util.o $(LDFLAGS) -o blink.axf $(LDLIBS)
	$(OBJCOPY) -O binary blink.axf blink.bin

util.o : util.c util.h
	$(CC) $(CFLAGS) -c util.c

VPATH = cmsis/Device/ARM/ARMCM4/Source/GCC:cmsis-dfp-stm32f4/Source/Templates:cmsis-freertos/CMSIS/RTOS2/FreeRTOS/Source:cmsis-freertos/Source:cmsis-freertos/Source/portable/MemMang:cmsis-freertos/Source/portable/GCC/ARM_CM4F

libcmsis_stm32f411xe.a : $(OBJS)
	$(AR) r $@ $?

LDFLAGS = -nostdlib -nostartfiles -L. \
          -L/usr/local/$(ABI)/lib/fpu \
          -Wl,--print-memory-usage \
          -Tstm32f411ceu6.ld

LDLIBS = -lcmsis_stm32f411xe -lg

clean :
	rm -f *.[ao]
